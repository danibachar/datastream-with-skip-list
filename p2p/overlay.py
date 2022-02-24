import imp
import .skiplist
from .flags import DEBUG

import random
class Overlay:
  """The P2P netwrok"""    
  def __init__(self, node_count, stream_length):
      self._skip_list_network = Skiplist() # Underlying implementation of the overlay network
      self._clients = {} # Set of clients, for ease of access we keep it in a dictionary

      # Building the underlying network of the skip list
      for _ in range(node_count):
        # We constructing nodes according to the stream_length, 
        # to randomally assign clients later on to them
        # In reality this cannot happen but we took this approach for the ease simulation
        self._skip_list_network._insert(random.randint(0, stream_length), None)
  
  def join(self, client):
    # Add client to map
    self._clients[client._id] = client
    # Inserts the new client 
    _ = self._skip_list_network._insert(client._playback_offset, client)
    
  def leave(self, client):
    self._clients[client._id] = None # Removes from local map
    node, _ = self._skip_list_network._scan(client._playback_offset) # Looks for connected node
    if node == None:
      if DEBUG:
        print("No node found for key {}".format(client._playback_offset))
      return # Client is not in the list already?
    node.disconnect(client)