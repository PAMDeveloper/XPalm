#!/usr/bin/env python


from GlobalVariables import GlobalVariables
from BH import BH

class Racines(object):
    
    def __init__(self, treeInstance) :
        self.tree = treeInstance
        self.z = GlobalVariables.Z_INIT
        self.TailleC1 = 0
        self.TailleVap = 0
        self.TailleC1moinsVap = 0 
        self.TailleVap = 0
        self.TailleC2 = 0
        self.TailleC = 0
        self.compute_compartment_size(GlobalVariables.Z_INIT)
        
        
    def compute_compartment_size(self, Z) :
        if Z > GlobalVariables.Z1 : 
            self.TailleC1 = (GlobalVariables.H_CR - 0.5 * GlobalVariables.H_PF) * GlobalVariables.Z1
            self.TailleVap = 0.5 * GlobalVariables.H_PF * GlobalVariables.Z1
        else :
            self.TailleC1 = (GlobalVariables.H_CR - 0.5 * GlobalVariables.H_PF) * Z
            self.TailleVap = 0.5 * GlobalVariables.H_PF * Z
        self.TailleC1moinsVap = self.TailleC1 - self.TailleVap

        if Z > GlobalVariables.Z2 + GlobalVariables.Z1:
            self.TailleC2 =  (GlobalVariables.H_CR - GlobalVariables.H_PF) * GlobalVariables.Z2
        else:
            self.TailleC2 = max (0 , (GlobalVariables.H_CR - GlobalVariables.H_PF) * (Z - GlobalVariables.Z1))
        self.TailleC = self.TailleC2 + self.TailleC1moinsVap 
        
        
    def growth(self,TEff) :
        self.compute_reduction_vitesse(GlobalVariables.FTSW_RACINES, self.tree.BH.FTSW)
        if self.z + self.red * GlobalVariables.VITESSE_RACINES * ( TEff )  > GlobalVariables.Z2 + GlobalVariables.Z1:
            self.z = GlobalVariables.Z2 + GlobalVariables.Z1
        else:
            self.z += self.red * GlobalVariables.VITESSE_RACINES * ( TEff )
        #print "Z", self.z
        self.compute_compartment_size(self.z)
        #print "C1", self.TailleC1, "Vap",  self.TailleVap, "C1moinsVap", self.TailleC1moinsVap, "C2", self.TailleC2, "C", self.TailleC
        
    def compute_reduction_vitesse(self, seuil , FTSW) :
        
        if FTSW > seuil:
            self.red = 1
        else :   
            self.red = FTSW / seuil
    
        
        