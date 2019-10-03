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


class DVrouter(Router):
    """Distance vector routing protocol implementation."""

    def __init__(self, addr, heartbeatTime):
        """TODO: add your own class fields and initialization code here"""
        Router.__init__(self, addr)  # initialize superclass - don't remove
        self.heartbeatTime = heartbeatTime
        self.dist_table = {}
        self.port_list = []

    def handlePacket(self, port, packet):
        """TODO: process incoming packet"""
        pkt_link = self.links[port]
        link_src = pkt_link.e1 if pkt_link.e1 != self.addr else pkt_link.e2

        if packet.dstAddr != self.addr:
            if link_src in self.dist_table:
                fastest_port = min(self.dist_table, key=self.dist_table.get)
                self.send(fastest_port, packet)
            else:
                self.dist_table[link_src] = 1
                random_port = self.get_random_port(port)
                self.send(random_port, packet)
        else:
            print(packet)

        self.port_list.append(
            {"PORT_IDX": port,
             "LINK N NOT ME": link_src,
             "PACKET SRC": packet.srcAddr,
             "PACKET DST": packet.dstAddr,
             "DIST TABLE": self.dist_table})

    def handleNewLink(self, port, endpoint, cost):
        """TODO: handle new link"""
        pass

    def handleRemoveLink(self, port):
        """TODO: handle removed link"""
        pass

    def handleTime(self, timeMillisecs):
        """TODO: handle current time"""
        pass

    def debugString(self):
        """TODO: generate a string for debugging in network visualizer"""
        return "{}".format(self.port_list)
        # return "{}".format([(self.links[o].e1, self.links[o].e2) for o in self.links])

    def get_random_port(self, except_port):
        tmp_dict = self.links
        if len(tmp_dict) > 1:
            tmp_dict.pop(except_port, None)
        random_port = random.choice(list(tmp_dict.keys()))
        return random_port
