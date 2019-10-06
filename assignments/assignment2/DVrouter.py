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
import ast
import random

# python visualize_network.py 05_pg242_net.json DV


class DVrouter(Router):
    """Distance vector routing protocol implementation."""

    def __init__(self, addr, heartbeatTime):
        """TODO: add your own class fields and initialization code here"""
        Router.__init__(self, addr)  # initialize superclass - don't remove
        self.heartbeatTime = heartbeatTime
        self.dist_table = {self.addr: {'port': -1, 'distance': 0}}

        self.debug_list = []

    def handlePacket(self, port, packet):
        """TODO: process incoming packet"""
        if packet.isTraceroute():
            self.handle_traceroute_pkt(port, packet)
        else:
            self.handle_routing_pkt(port, packet)
        self.debug_list = self.dist_table

    def handle_traceroute_pkt(self, port, packet):
        p_dist_table = ast.literal_eval(packet.content)
        p_dist_table.pop(self.addr, None)
        p_dist_table.pop(packet.srcAddr, None)

        self.dist_table[packet.srcAddr] = {'port': port, 'distance': 1}

        for row in p_dist_table:
            packet_row = p_dist_table[row]
            if row in self.dist_table:
                if packet_row['distance'] < self.dist_table[row]['distance']:
                    packet_row['distance'] += 1
                    self.dist_table[row] = packet_row
            else:
                packet_row['distance'] += 1
                self.dist_table[row] = packet_row

    def handle_routing_pkt(self, port, packet):
        if packet.dstAddr != self.addr:
            pass
        else:
            print(packet)

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
            content = str(self.dist_table)
            routing_pkt = Packet(kind, srcAddr, dstAddr, content)
            for port in range(4):
                try:
                    self.send(port, routing_pkt)
                except:
                    pass

    def debugString(self):
        """TODO: generate a string for debugging in network visualizer"""
        return "{}".format(self.debug_list)
