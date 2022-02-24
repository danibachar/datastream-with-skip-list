from .rule import Rule
from ..utils.skiplist import Skiplist
from .packet import Packet
# Create skip list for each one of the classes we would like to classify packet by:
# source address, destination address, source port number, destination port number, and protocol type
class Classifier:
  def __init__(self) -> None:
    # each node contains a list of rules in which the lower boundary of the
    # corresponding field is less than or equal to the value of the node and the upper boundary
    # of the corresponding field is greater than or equal to the value of the node
    self.source_address_sl = Skiplist()
    self.source_port_sl = Skiplist()
    self.destination_address_sl = Skiplist()
    self.destination_port_sl = Skiplist()
    # protocol skip list, each node contains a list of rules whose protocol field has a value equal to the key of the node
    self.protocol_type_sl = Skiplist() 
  
  def add(rules: List[Rule]) -> None:
    pass

  def classify(self, packet: Packet) -> Rule:
    src_ip_rule = self.source_address_sl._scan(packet.src_ip)
    src_port_rule = self.source_port_sl._scan(packet.src_port)
    dst_ip_rule = self.destination_address_sl._scan(packet.dst_ip)
    dst_port_rule = self.destination_port_sl._scan(packet.dst_port)
    protocol_rule = self.protocol_type_sl._scan(packet.protocol)



# Transform

# Transform IP prefix, into upper and lower boundries binary representaiotns, which than converted into integres
# The integers values will be the keys for the nodes which representes the rules of the system
def transform(rule) -> Rule:
  pass