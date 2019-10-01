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


class DVrouter(Router):
    """Distance vector routing protocol implementation."""

    def __init__(self, addr, heartbeatTime):
        """TODO: add your own class fields and initialization code here"""
        Router.__init__(self, addr)  # initialize superclass - don't remove
        self.heartbeatTime = heartbeatTime
        self.routing_table = {}

    def handlePacket(self, port, packet):
        """TODO: process incoming packet"""
        if packet.dstAddr != self.addr:
            self.send(port, packet)
        else:
            if packet.kind == 1:
                print("This is a TRACEROUTE packet")

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
        return "{}".format([(self.links[o].e1, self.links[o].e2) for o in self.links])
