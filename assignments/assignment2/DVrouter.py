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

        self.debug_list = []

    def handlePacket(self, port, packet):
        """TODO: process incoming packet"""
        pkt_link = self.links[port]
        link_src = pkt_link.e1 if pkt_link.e1 != self.addr else pkt_link.e2

        if packet.kind == 1:
            req_pkt = self.handle_traceroute_pkt(port, packet)
        else:
            req_pkt = self.handle_routing_pkt(port, packet)
        self.debug_list.append(
            {"pd": packet.dstAddr, "addr2port": self.addr2port()})

        # self.debug_list.append(
        #     {"PORT_IDX": port,
        #      "PACKET KIND": packet.kind,
        #      "PACKET CONTENT": req_pkt,
        #      #  "LINK N NOT ME": link_src,
        #      "PACKET SRC": packet.srcAddr,
        #      "PACKET DST": packet.dstAddr,
        #      "DIST TABLE": self.dist_table})

    def handle_traceroute_pkt(self, port, packet):
        req_pkt = packet.content
        if packet.dstAddr != self.addr:
            if self.is_neighbour(packet.dstAddr):
                port = self.addr2port()[packet.dstAddr]
                self.send(port, packet)
            else:
                random_port = self.get_random_port(port)
                my_table = {'src': self.addr, 'dist_table': self.dist_table}
                packet.content = str(my_table)
                self.send(random_port, packet)
        else:
            print(packet)
        return req_pkt

    def handle_routing_pkt(self, port, packet):
        req_pkt = packet.content
        if packet.dstAddr != self.addr:
            random_port = self.get_random_port(port)
            packet.content = {'src': self.addr, 'dist_table': self.dist_table}
            self.send(random_port, packet)
        else:
            print(packet)
        return req_pkt

    def is_neighbour(self, addr):
        return addr in self.get_neighbours()

    def addr2port(self):
        return {v: k for k, v in self.links.items()}

    def get_neighbours(self):
        neighbours = {}
        for port, link in self.links.items():
            neighbours[link.e1] = {}
            neighbours[link.e1][link.e2] = link.l12
            neighbours[link.e2] = {}
            neighbours[link.e2][link.e1] = link.l21
        return neighbours

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
        pass

    def debugString(self):
        """TODO: generate a string for debugging in network visualizer"""
        return "{}".format(self.debug_list)
        # return "{}".format([(self.links[o].e1, self.links[o].e2) for o in self.links])
