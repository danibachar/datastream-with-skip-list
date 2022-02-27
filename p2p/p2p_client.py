
from threading import Thread
import time, uuid
import numpy as np

from ..utils.flags import DEBUG

class VCROperation:
  def __init__(self, type, speed, offset) -> None:
      self.type = type
      self.speed = speed
      self.seek_offset = offset

  @property
  def is_forward(self):
    return self.type == "farword"
  @property
  def is_rewind(self):
    return self.type == "rewind"
  @property
  def is_seek(self):
    return self.type == "rewind"



class Client:
  """A node from a skip list"""    
  def __init__(self, playback_offset = 0.0, buffer_size = 10, total_stream_length = 100, vcr_operation = None, vcr_operation_percent_ofstream = 0):
      self._id = uuid.uuid4()
      
      self._vcr_operation = vcr_operation
      self._vcr_operation_percent_ofstream = vcr_operation_percent_ofstream

      self._total_stream_length = total_stream_length
      self._playback_offset = playback_offset
      
      self._buffer_size = buffer_size
      self._buffer = (playback_offset, playback_offset+buffer_size)

      self._speed = 1
      self._is_playing = False
      self._is_forward = True
      self._play_thread = None
      self._buffering_thread = None
      self._misses = {}
      self._hits = {}
      self._vcr_data = {}
      
      self._buffering_duration = 0.01
      self._tik_duration = 0.005

      self.time = 0
      
  def __repr__(self) -> str:
      return str(self._id)

  def __eq__(self, other): 
    if not isinstance(other, Client):
      # don't attempt to compare against unrelated types
      return NotImplemented
    
    return self._id == other._id
  def __str__(self):
    msg = "rewing"
    if self._is_forward:
      msg = "forward"
    return "{}\nwith buffer {}\nplaying {}: {}\nat speed {}".format(self._id, self._buffer, msg, self._playback_offset, self._speed)

  def start_timer(self):
    def _run():
      while True:
        time.sleep(self._tik_duration)
        self.time+=1
    self._timer_theread = Thread(target=_run, daemon=True)
    self._timer_theread.start()

  def _play(self):
    while self._is_playing:
        # Hit / Mis - SMR
        # print("crack")
        if self._playback_offset > self._buffer[1] or self._playback_offset < self._buffer[0]:
          self._misses[self.time] = self._misses.get(self.time,0) + 1
          self.random_seek(self._playback_offset)
          continue
        else: 
          self._hits[self.time] = self._hits.get(self.time,0) + 1

        # mimik play action
        if DEBUG:
          print(self) 

        # Handling VCR Operations
        self._handle_vcr_op_if_needed()

        # Advance to next segment
        if self._is_forward:
          self._playback_offset += 1 * self._speed
        else:
          self._playback_offset -= 1 * self._speed
        
        # Validating end/begining of stream
        if self._playback_offset >= self._total_stream_length:
          if DEBUG:
            print("Got to END of the stream {}".format(self._playback_offset))
          self._is_playing = False
          break
        if self._playback_offset <= 0:
          if DEBUG:
            print("Got to BEGINING of the stream {}".format(self._playback_offset))
          self._is_playing = False
          break

        # Validate end of local buffer, i.e needs to random seek
        if self._buffer[1] <= self._playback_offset and self._is_forward:
          self.random_seek(self._playback_offset)
          if DEBUG:
            print("forward: end of buffer need to go to next node")
        if self._playback_offset <= self._buffer[0] and not self._is_forward:
          self.random_seek(self._playback_offset)
          if DEBUG:
            print("rewind: end of buffer need to go to prev node")
  
  def _handle_vcr_op_if_needed(self):
    # Check if operation exists
    if self._vcr_operation == None:
      return
    # Check if we reach the percentage of the stream
    current_precentage_done = self._playback_offset / self._total_stream_length
    if current_precentage_done >= self._vcr_operation_percent_ofstream:
      return
    # Handle by type
    self._speed = self._vcr_operation.speed
    if self._vcr_operation.is_forward:
      self._speed = self._vcr_operation.speed
      self._is_forward = True
    elif self._vcr_operation.is_rewind:
      self._speed = self._vcr_operation.speed
      self._is_forward = False
    elif self._vcr_operation.is_seek:
      self._playback_offset = self._vcr_operation.offset
    # handled, no need for operation no mo (:
    self._vcr_operation = None

  def _fetch_buffer(self):
    # Sleep - mimik async buffering
    time.sleep(self._buffering_duration)
    # We need to fetch more buffer forward
    if self._playback_offset > self._buffer[-1]:
      begin = self._playback_offset
      end = min(self._playback_offset + self._buffer_size, self._total_stream_length)
      if DEBUG:
        print("Buffering forward from {} to {}".format(begin, end))
      self._buffer = (begin, end)
    
    # We need to fetch more buffer rewind
    if self._playback_offset < self._buffer[0]:
      begin = self._playback_offset
      end = max(0, self._buffer[0] - self._buffer_size)
      if end < begin:
        tmp = begin
        begin = end
        end = tmp
      if DEBUG:
        print("Buffering rewind from {} to {}".format(begin, end))
      self._buffer = (begin, end)

  def play(self):
    assert(self._overlay != None)
    
    self._is_playing = True
    if self._play_thread:
      raise Exception("Trying to play while playing thread is already running")
    self._play_thread = Thread(target=self._play, daemon=True)
    if DEBUG:
      print("start playing")
    self._play_thread.start()

  # Network operations
  def join(self, overlay): # O(log(N/log(N)))
    self._overlay = overlay
    overlay.join(self)
    # Make sure buffer is updated - i.e simulate ask from server or nearby nodes
    if self._playback_offset not in self._buffer:
      # Buffering on a different thread to give the async elusion
      self._buffering_thread = Thread(target=self._fetch_buffer, daemon=True)
      self._buffering_thread.start()

  def leave(self): # O(1)
    assert(self._overlay != None)
    # Before leaving notify the neighbors so they can reconnect
    self._overlay.leave(self)
    self._overlay = None

  # VCR like operations
  def pause(self):
    assert(self._overlay != None)
    if not self._is_playing: 
      return
    self._is_playing = False
    # Nothing to do, only if the buffer overflows we leave
    # We don't support buffer overflow

  def resume(self):
    assert(self._overlay != None)
    # Make sure we not already playing
    if self._is_playing: 
      return
    # Enable palying
    self._is_playing = True
    # Only if we are not part of the overlay we shall re-join
    if self._id not in self._overlay._clients:
        self._overlay.join(self)
    # Start playing
    self.play()

  def random_seek(self, new_offset):
    assert(self._overlay != None)
    overlay = self._overlay
    self.leave()
    self._playback_offset = new_offset # Set new offset
    self.join(overlay)

  # Not supported yet
  def forward(self, speed):
    assert(self._overlay != None)
    self._speed = speed
    self._is_playing = True

  def rewind(self, speed):
    assert(self._overlay != None)
    self._speed = speed
