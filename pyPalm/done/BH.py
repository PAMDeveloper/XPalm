#!/usr/bin/env python

from Meteo import Meteo
from GlobalVariables import GlobalVariables

class BH(object):
    
    def __init__(self, treeinstance):
        self.tree = treeinstance
        self.TailleC1 = ( GlobalVariables.H_CR - GlobalVariables.H_PF_Z1 ) * GlobalVariables.Z1
        self.TailleVap = GlobalVariables.H_PF_Z1 * GlobalVariables.Z1
        self.TailleC1moinsVap = self.TailleC1 - self.TailleVap
        self.TailleC2 = ( GlobalVariables.H_CR  - GlobalVariables.H_PF ) * GlobalVariables.Z2
        self.TailleC =  self.TailleC2 + self.TailleC1 - self.TailleVap
        A = min(self.TailleC1, (GlobalVariables.H_0 - GlobalVariables.H_PF_Z1 ) * GlobalVariables.Z1)
        self.qte_H2O_C1 = max(0, A )
        A = min(self.TailleVap , (GlobalVariables.H_0 - GlobalVariables.H_PF_Z1 ) * GlobalVariables.Z1 )
        self.qte_H2O_Vap = max (0 , A)
        A = min(self.TailleC2, (GlobalVariables.H_0 - GlobalVariables.H_PF ) * GlobalVariables.Z2)
        self.qte_H2O_C2 = max(0, A)
        A = self.qte_H2O_C1 + self.qte_H2O_C2 - self.qte_H2O_Vap
        self.qte_H2O_C = max(0, A)
        A = self.qte_H2O_C1 - self.qte_H2O_Vap
        self.qte_H2O_C1moinsVap = max(0, A)
        #print 'BH_TAILLE', 'tailleC1 =',  self.TailleC1, 'tailleVap', self.TailleVap, 'tailleC1moinsVap', self.TailleC1moinsVap, 'tailleC2', self.TailleC2, 'tailleC', self.TailleC
        #print 'BH_QTE', 'C1', self.qte_H2O_C1, 'Vap', self.qte_H2O_Vap, 'C1moinsVap', self.qte_H2O_C1moinsVap , 'C2', self.qte_H2O_C2, 'C', self.qte_H2O_C 

        self.qte_H2O_C1_Racines = self.qte_H2O_C1 * self.tree.racines.TailleC1 / self.TailleC1
        self.qte_H2O_Vap_Racines = self.qte_H2O_Vap * self.tree.racines.TailleVap / self.TailleVap
        self.qte_H2O_C2_Racines = self.qte_H2O_C2 * self.tree.racines.TailleC2 / self.TailleC2
        self.qte_H2O_C_Racines = self.qte_H2O_C * self.tree.racines.TailleC / self.TailleC
        self.qte_H2O_C1moinsVap_Racines = self.qte_H2O_C1moinsVap * self.tree.racines.TailleC1moinsVap / self.TailleC1moinsVap
        #print 'RACINES_TAILLE', 'tailleC1 =',  self.tree.racines.TailleC1, 'tailleVap', self.tree.racines.TailleVap, 'tailleC1moinsVap', self.tree.racines.TailleC1moinsVap, 'tailleC2', self.tree.racines.TailleC2, 'tailleC',  self.tree.racines.TailleC 
        #print 'RACINES_QTE', 'C1', self.qte_H2O_C1_Racines, 'Vap', self.qte_H2O_Vap_Racines, 'C1moinsVap', self.qte_H2O_C1moinsVap_Racines , 'C2', self.qte_H2O_C2_Racines, 'C', self.qte_H2O_C_Racines 
        self.FractionC1 = 0
        self.FractionC2 = 0
        self.FractionC = 0
        self.FractionC1Racine = 0
        self.FractionC2Racine = 0
        self.FTSW = 0   
        self.FractionC1moinsVapRacine =  0
        self.compute_fraction()
        self.EvapMax = 0
        self.Transp_Max = 0
        self.pluie_efficace = 0
        self.Evap = 0
        self.EvapC1moinsVap = 0
        self.EvapVap = 0
        self.Transpi = 0 
        self.TranspiC2 = 0
        self.TranspiC1moinsVap   = 0 

    def compute_fraction(self) :
            self.FractionC1 = self.qte_H2O_C1/ self.TailleC1
            if self.TailleC2!= 0 :
                self.FractionC2 = self.qte_H2O_C2/ self.TailleC2
            else :
                self.FractionC2 = 0 
            self.FractionC = self.qte_H2O_C/ self.TailleC 
            self.FractionC1Racine = self.qte_H2O_C1_Racines / self.tree.racines.TailleC1
            if self.tree.racines.TailleC2 != 0 : 
                self.FractionC2Racine = self.qte_H2O_C2_Racines / self.tree.racines.TailleC2
            else :
                self.FractionC2Racine = 0
            self.FTSW = self.qte_H2O_C_Racines / self.tree.racines.TailleC  
            self.FractionC1moinsVapRacine =  self.qte_H2O_C1moinsVap_Racines / self.tree.racines.TailleC1moinsVap

   
    def compute_FTSW(self, ET0, pluie):
        self.compute_Evap_Max(ET0)
        self.compute_Transp_Max(ET0)
        #print "EvapMax", self.EvapMax, "Transp_Max", self.Transp_Max
        self.compute_pluie_efficace(pluie)
        self.compute_BH_apres_pluie(self.pluie_efficace)
        self.compute_BH_Racines_pluie(self.pluie_efficace)
        self.compute_fraction()
        self.compute_BH_apres_Evap()
        self.compute_BH_Racines_apres_Evap()
        self.compute_fraction()
        self.compute_BH_Racines_apres_Transpi()
        self.compute_BH_apres_Transpi()
        self.compute_fraction()
        
    def compute_Evap_Max(self, ET0) :
        self.EvapMax = ( 1 - self.tree.ei ) * ET0 * GlobalVariables.KC   
        
    def compute_Transp_Max(self, ET0) : 
        self.Transp_Max = self.tree.ei * ET0 * GlobalVariables.KC
        
        
    def compute_pluie_efficace(self,pluie) :
        
        
        if (0.916 * pluie- 0.589) < 0 :
            Pluie_au_sol = 0
        else:
            Pluie_au_sol =(0.916 * pluie - 0.589)
  
        if (0.0713 * pluie - 0.735) < 0 :
            Ecoul_stipe = 0
        else:
            Ecoul_stipe=(0.0713* pluie - 0.735)
  
        self.pluie_efficace = Pluie_au_sol + Ecoul_stipe
        
        #print 'pluie', self.pluie_efficace
        
    
    def compute_BH_apres_pluie(self, pluie_efficace) :
        mem_qte_H2O_C1 = self.qte_H2O_C1
        mem_qte_H2O_C2 = self.qte_H2O_C2
        mem_qte_H2O_C1moinsVap = self.qte_H2O_C1moinsVap
        mem_qte_H2O_Vap = self.qte_H2O_Vap
        mem_qte_H2O_C = self.qte_H2O_C
        
        if (self.qte_H2O_Vap + pluie_efficace) >= self.TailleVap:
       
            self.qte_H2O_Vap = self.TailleVap
            if (self.qte_H2O_C1moinsVap + ( pluie_efficace - self.TailleVap + mem_qte_H2O_Vap )) >= self.TailleC1moinsVap :
       
                self.qte_H2O_C1moinsVap = self.TailleC1moinsVap
                self.qte_H2O_C1 = self.qte_H2O_C1moinsVap + self.qte_H2O_Vap
                if (self.qte_H2O_C2 + mem_qte_H2O_C1 + pluie_efficace - self.TailleC1) >= self.TailleC2:
      
                    self.qte_H2O_C2 = self.TailleC2
                else:
      
                    self.qte_H2O_C2 += mem_qte_H2O_C1 + pluie_efficace - self.TailleC1
            else:
       
                self.qte_H2O_C1moinsVap +=  pluie_efficace - self.TailleVap + mem_qte_H2O_Vap  
                self.qte_H2O_C1 = self.qte_H2O_C1moinsVap + self.qte_H2O_Vap 
                #QuantH2OC2[i] = QuantH2OC2[i-1]
        else:
        
            self.qte_H2O_Vap += pluie_efficace
            #QuantH2OC1moinsVap[i] = QuantH2OC1moinsVap[i-1]
            self.qte_H2O_C1 = self.qte_H2O_Vap + self.qte_H2O_C1moinsVap
            #QuantH2OC1[i] = QuantH2OC1moinsVap[i] + QuantH2OVap[i]
            #QuantH2OC2[i] = QuantH2OC2[i-1]
        self.qte_H2O_C = self.qte_H2O_C1moinsVap + self.qte_H2O_C2
        #print 'AVANT EVAP' , 'BH_TAILLE', 'tailleC1 =',  self.TailleC1, 'tailleVap', self.TailleVap, 'tailleC1moinsVap', self.TailleC1moinsVap, 'tailleC2', self.TailleC2, 'tailleC', self.TailleC
        #print 'AVANT EVAP', 'BH_QTE', 'C1', self.qte_H2O_C1, 'Vap', self.qte_H2O_Vap, 'C1moinsVap', self.qte_H2O_C1moinsVap , 'C2', self.qte_H2O_C2, 'C', self.qte_H2O_C 

    
    def compute_BH_Racines_pluie(self, pluie_efficace) :
        mem_qte_H2O_C1_Racines = self.qte_H2O_C1_Racines
        mem_qte_H2O_C2_Racines = self.qte_H2O_C2_Racines
        mem_qte_H2O_C1moinsVap_Racines = self.qte_H2O_C1moinsVap_Racines
        mem_qte_H2O_Vap_Racines = self.qte_H2O_Vap_Racines
        mem_qte_H2O_C_Racines = self.qte_H2O_C_Racines
        if (self.qte_H2O_Vap_Racines + pluie_efficace) >= self.tree.racines.TailleVap:
    
            self.qte_H2O_Vap_Racines = self.tree.racines.TailleVap           
            if (self.qte_H2O_C1moinsVap_Racines + ( pluie_efficace - self.tree.racines.TailleVap + mem_qte_H2O_Vap_Racines )) >= self.tree.racines.TailleC1moinsVap:
 
                self.qte_H2O_C1moinsVap_Racines = self.tree.racines.TailleC1moinsVap
                self.qte_H2O_C1_Racines = self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_Vap_Racines
                if (self.qte_H2O_C2_Racines + mem_qte_H2O_C1_Racines + pluie_efficace - self.tree.racines.TailleC1) >= self.tree.racines.TailleC2:
   
                    self.qte_H2O_C2_Racines = self.tree.racines.TailleC2
                else:
 
  
                    self.qte_H2O_C2_Racines += mem_qte_H2O_C1_Racines  + pluie_efficace - self.tree.racines.TailleC1
            else:
   
                self.qte_H2O_C1moinsVap_Racines +=  pluie_efficace - self.tree.racines.TailleVap + mem_qte_H2O_Vap_Racines  
                self.qte_H2O_C1_Racines = self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_Vap_Racines
                
        else:
   
            self.qte_H2O_Vap_Racines +=  pluie_efficace
            self.qte_H2O_C1_Racines = self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_Vap_Racines
        self.qte_H2O_C_Racines =  self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_C2_Racines
        #print 'AVANT EVAP' , 'RACINES_TAILLE', 'tailleC1 =',  self.tree.racines.TailleC1, 'tailleVap', self.tree.racines.TailleVap, 'tailleC1moinsVap', self.tree.racines.TailleC1moinsVap, 'tailleC2', self.tree.racines.TailleC2, 'tailleC',  self.tree.racines.TailleC 
        #print 'AVANT EVAP' , 'RACINES_QTE', 'C1', self.qte_H2O_C1_Racines, 'Vap', self.qte_H2O_Vap_Racines, 'C1moinsVap', self.qte_H2O_C1moinsVap_Racines , 'C2', self.qte_H2O_C2_Racines, 'C', self.qte_H2O_C_Racines 
   
    def compute_BH_apres_Evap(self) :
        
        self.Evap = self.EvapMax * self.compute_ks(GlobalVariables.SEUIL_EVAP, self.FractionC1)    
        #print 'evap_max', self.EvapMax, 'fraction_C1', self.FractionC1 ,'evap', self.Evap
        if self.qte_H2O_C1moinsVap - self.Evap >=0:
            self.qte_H2O_C1moinsVap +=  - self.Evap
            self.EvapC1moinsVap = self.Evap
            self.EvapVap = 0
        else:
            self.EvapC1moinsVap = self.qte_H2O_C1moinsVap
            self.qte_H2O_C1moinsVap = 0
            self.EvapVap = self.Evap - self.EvapC1moinsVap
            self.qte_H2O_Vap +=  - self.EvapVap
        #print 'EvapVap', self.EvapVap, 'EvapC1moinsVap', self.EvapC1moinsVap
        self.qte_H2O_C1 = self.qte_H2O_C1moinsVap + self.qte_H2O_Vap 
        self.qte_H2O_C = self.qte_H2O_C1 + self.qte_H2O_C2 - self.qte_H2O_Vap 
        #print 'APRES EVAP' , 'BH_TAILLE', 'tailleC1 =',  self.TailleC1, 'tailleVap', self.TailleVap, 'tailleC1moinsVap', self.TailleC1moinsVap, 'tailleC2', self.TailleC2, 'tailleC', self.TailleC
        #print 'APRES EVAP', 'BH_QTE', 'C1', self.qte_H2O_C1, 'Vap', self.qte_H2O_Vap, 'C1moinsVap', self.qte_H2O_C1moinsVap , 'C2', self.qte_H2O_C2, 'C', self.qte_H2O_C 


    def compute_BH_Racines_apres_Evap(self) :
       
        
        A = self.qte_H2O_C1moinsVap_Racines - self.EvapC1moinsVap * self.tree.racines.TailleC1moinsVap / self.TailleC1moinsVap
        self.qte_H2O_C1moinsVap_Racines = max ( 0 , A)
        A = self.qte_H2O_Vap_Racines  - self.EvapVap * self.tree.racines.TailleVap / self.TailleVap
        self.qte_H2O_Vap_Racines = max (0 ,A)
        self.qte_H2O_C1_Racines = self.qte_H2O_Vap_Racines + self.qte_H2O_C1moinsVap_Racines
        self.qte_H2O_C_Racines = self.qte_H2O_C2_Racines + self.qte_H2O_C1moinsVap_Racines
        #print 'APRES EVAP' , 'RACINES_TAILLE', 'tailleC1 =',  self.tree.racines.TailleC1, 'tailleVap', self.tree.racines.TailleVap, 'tailleC1moinsVap', self.tree.racines.TailleC1moinsVap, 'tailleC2', self.tree.racines.TailleC2, 'tailleC',  self.tree.racines.TailleC 
        #print 'APRES EVAP' , 'RACINES_QTE', 'C1', self.qte_H2O_C1_Racines, 'Vap', self.qte_H2O_Vap_Racines, 'C1moinsVap', self.qte_H2O_C1moinsVap_Racines , 'C2', self.qte_H2O_C2_Racines, 'C', self.qte_H2O_C_Racines 
    
    def compute_BH_Racines_apres_Transpi(self) :
        
        self.Transpi = self.Transp_Max * self.compute_ks(GlobalVariables.SEUIL_TRANSPI, self.FTSW)
        #print 'Transp_Max', self.Transp_Max, 'FTSW', self.FTSW, 'Transp', self.Transpi
        if self.qte_H2O_C2_Racines > 0:
            self.TranspiC2 = min( self.Transpi * ( self.qte_H2O_C2_Racines / ( self.qte_H2O_C2_Racines + self.qte_H2O_C1moinsVap_Racines ) ), self.qte_H2O_C2_Racines )
        else:
            self.TranspiC2 = 0
        
  #TranspC2[j] = min(Transp[j] * (QuantH2OC2Racine[j] / (QuantH2OC2Racine[j] + QuantH2OC1moinsVapRacine[j])),QuantH2OC2Racine[j])
        if self.qte_H2O_C1moinsVap_Racines > 0:
            self.TranspiC1moinsVap= min(self.Transpi * (self.qte_H2O_C1moinsVap_Racines / (self.qte_H2O_C2_Racines + self.qte_H2O_C1moinsVap_Racines)), self.qte_H2O_C1moinsVap_Racines)
        else:
            self.TranspiC1moinsVap = 0
        #print 'tranpi_C2', self.TranspiC2, 'transpi C1moinsVap', self.TranspiC1moinsVap
        
        self.qte_H2O_C1moinsVap_Racines +=  - self.TranspiC1moinsVap  
        self.qte_H2O_C2_Racines +=  - self.TranspiC2
        self.qte_H2O_C_Racines = self.qte_H2O_C2_Racines + self.qte_H2O_C1moinsVap_Racines
        self.qte_H2O_C1_Racines = self.qte_H2O_Vap_Racines + self.qte_H2O_C1moinsVap_Racines
        #print 'APRES TRANSPI' , 'RACINES_TAILLE', 'tailleC1 =',  self.tree.racines.TailleC1, 'tailleVap', self.tree.racines.TailleVap, 'tailleC1moinsVap', self.tree.racines.TailleC1moinsVap, 'tailleC2', self.tree.racines.TailleC2, 'tailleC',  self.tree.racines.TailleC 
        #print 'APRES TRANSPI' , 'RACINES_QTE', 'C1', self.qte_H2O_C1_Racines, 'Vap', self.qte_H2O_Vap_Racines, 'C1moinsVap', self.qte_H2O_C1moinsVap_Racines , 'C2', self.qte_H2O_C2_Racines, 'C', self.qte_H2O_C_Racines 
  
    
    def compute_BH_apres_Transpi(self) :
        self.qte_H2O_C1moinsVap += - self.TranspiC1moinsVap  
        self.qte_H2O_C2 +=  - self.TranspiC2 
        self.qte_H2O_C = self.qte_H2O_C2 + self.qte_H2O_C1moinsVap
        self.qte_H2O_C1 = self.qte_H2O_Vap + self.qte_H2O_C1moinsVap
        #print 'APRES TRANSPI' , 'BH_TAILLE', 'tailleC1 =',  self.TailleC1, 'tailleVap', self.TailleVap, 'tailleC1moinsVap', self.TailleC1moinsVap, 'tailleC2', self.TailleC2, 'tailleC', self.TailleC
        #print 'APRES TRANSPI', 'BH_QTE', 'C1', self.qte_H2O_C1, 'Vap', self.qte_H2O_Vap, 'C1moinsVap', self.qte_H2O_C1moinsVap , 'C2', self.qte_H2O_C2, 'C', self.qte_H2O_C 
        #print '-------------------------------------------------------------------------------------------------------------------------'
    
    def compute_ks(self, seuil , FTSW) :
        
        if FTSW > seuil:
            y = 1
        else :   
            y = FTSW / seuil
        return(y)