#!/usr/bin/env python

from GlobalVariables import GlobalVariables

class Organ(object):
    
    def __init__(self, phytomerInstance):
        self.phytomer = phytomerInstance
        self.thermalTimeSinceAppearance = 0
        
    def getPhytomer(self):
        return self.phytomer
    
    def getThermalTimeSinceAppearance(self):
        return self.thermalTimeSinceAppearance
        
    def increaseThermalTimeSinceAppearance(self, TEff):
        if self.phytomer.rank > 0 :
            self.thermalTimeSinceAppearance += TEff
    
    def growth_demand(self, TEff):
        self.increaseThermalTimeSinceAppearance(TEff)
        
    def computeTEffCorrection(self, TEff):
        pass
    
    