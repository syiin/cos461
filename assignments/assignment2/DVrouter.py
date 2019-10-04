####################################################
# DVrouter.py
# Names:
# NetIds:
#####################################################

import sys
from collections import defaultdict
from router import Router
from packet import Packet
from json import dumps, loads

import random

# python visualize_network.py 05_pg242_net.json DV


class DVrouter(Router):
    """Distance vector routing protocol implementation."""

    def __init__(self, addr, heartbeatTime):
        """TODO: add your own class fields and initialization code here"""
        Router.__init__(self, addr)  # initialize superclass - don't remove
        self.heartbeatTime = heartbeatTime
        self.dist_table = {}

        self.debug_list = []

    def handlePacket(self, port, packet):
        """TODO: process incoming packet"""
        pkt_link = self.links[port]
        link_src = pkt_link.e1 if pkt_link.e1 != self.addr else pkt_link.e2

        if packet.kind == 2:
            self.handle_traceroute_pkt(port, packet)
        else:
            self.handle_routing_pkt(port, packet)
        self.debug_list.append(self.dist_table)

    def handle_traceroute_pkt(self, port, packet):
        self.debug_list.append(packet.content)
        if packet.content == "neighbour":
            self.dist_table[packet.srcAddr] = {'port': port,
                                               'distance': 1}
        else:
            pass

    def handle_routing_pkt(self, port, packet):
        if packet.dstAddr != self.addr:
            pass
        else:
            print(packet)

    def get_random_port(self, except_port):
        tmp_dict = self.links
        if len(tmp_dict) > 1:
            tmp_dict.pop(except_port, None)
        random_port = random.choice(list(tmp_dict.keys()))
        return random_port

    def handleNewLink(self, port, endpoint, cost):
        """TODO: handle new link"""
        pass

    def handleRemoveLink(self, port):
        """TODO: handle removed link"""
        pass

    def handleTime(self, timeMillisecs):
        """TODO: handle current time"""
        if timeMillisecs % 30 == 0:
            kind = 2
            srcAddr = self.addr
            dstAddr = self.addr
            content = "neighbour"
            routing_pkt = Packet(kind, srcAddr, dstAddr, content)
            for port in range(4):
                try:
                    self.send(port, routing_pkt)
                except:
                    pass

    def debugString(self):
        """TODO: generate a string for debugging in network visualizer"""
        return "{}".format(self.debug_list)
        # return "{}".format([(self.links[o].e1, self.links[o].e2) for o in self.links])
