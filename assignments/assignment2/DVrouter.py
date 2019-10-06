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
        if packet.isRouting():
            self.handle_routing(port, packet)
        else:
            self.handle_traceroute(port, packet)
        self.debug_list = self.dist_table

    def handle_routing(self, port, packet):
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

    def handle_traceroute(self, port, packet):
        if packet.dstAddr != self.addr:
            if packet.dstAddr in self.dist_table:
                send_port = self.dist_table[packet.dstAddr]['port']
                self.send(send_port, packet)
            else:
                self.dist_table[packet.srcAddr] = {'port': port, 'distance': 1}
        else:
            self.try_send(packet)

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
            self.try_send(routing_pkt)

    def try_send(self, pkt):
        for port in range(4):
            try:
                self.send(port, pkt)
            except:
                pass

    def debugString(self):
        """TODO: generate a string for debugging in network visualizer"""
        return "{}".format(self.debug_list)
