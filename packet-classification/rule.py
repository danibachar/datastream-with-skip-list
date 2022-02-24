# Each skip list contains a value, a list for storing rules, and a list for storing pointers to subsequent nodes based on the level of each node.

# In the protocol skip list, each
# node contains a list of rules whose protocol field has a value equal to the value of the node,
# but in other skip lists each node contains a list of rules in which the lower boundary of the
# corresponding field is less than or equal to the value of the node and the upper boundary
# of the corresponding field is greater than or equal to the value of the node.

# As the values
# of the fields of source address, destination address, source port number, and destination
# port number have both upper and lower boundaries, they should be inserted into the
# corresponding skip lists in two steps. In the first step, the lower boundary is inserted into
# the skip list. Then the value of the lower boundary node is compared with the upper and
# lower boundary values of all the rules.


# Rule Model
class Rule:
  def __init__(self, priority, src_ip, dst_ip, src_port, dst_port, protocol) -> None:
      self.priority = priority
      self.src_ip = src_ip
      self.dst_ip = dst_ip
      self.src_port = src_port
      self.dst_port = dst_port
      self.protocol = protocol

  def __eq__(self, o: object) -> bool:
      return self.src_ip == o.src_ip \
        and self.dst_ip == o.dst_ip \
        and self.src_port == o.src_port \
        and self.dst_port == o.dst_port \
        and self.protocol == o.protocol \
