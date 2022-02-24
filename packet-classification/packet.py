# Packet Model
class Packet:
  def __init__(self, src_ip, dst_ip, src_port, dst_port, protocol) -> None:
      self.src_ip = src_ip
      self.dst_ip = dst_ip
      self.src_port = src_port
      self.dst_port = dst_port
      self.protocol = protocol