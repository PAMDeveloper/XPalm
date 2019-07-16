#!C:/Python26/python.exe -u

from os import chdir
from numpy import *
from math import *
from GlobalVariables import GlobalVariables
import time

TBase = 15
TOpt1 = 25
TOpt2 = 30
TLim = 40

class Meteo(object):
    
    def __init__(self, drive, directoryName, fileName):
        self.drive = drive
        self.directoryName = directoryName
        self.fileName = fileName
        self.TT = 0
        self.lat_rad = GlobalVariables.LATITUDE  * pi/180
        self.jour_julien = 0
        self.irrigation = {}
        for i in range(len(GlobalVariables.IRRIGATION_DAY)) :
            self.irrigation[GlobalVariables.IRRIGATION_DAY[i]] = GlobalVariables.IRRIGATION_QUANTITY[i]
        self.irrigation_day = "PAS_D_IRRIGATION"
        
        
    def getDrive(self):
        return self.drive
    
    def getDirectoryName(self):
        return self.directoryName
    
    def getFileName(self):
        return self.fileName
    
    def getMeteoData(self):
        return self.meteoData
    
    def getCurrentLine(self):
        return self.currentLine
    
    def getTEff(self):
        return self.TEff
    
    def getTT(self):
        return self.TT
    
    def initMeteo(self):
        meteoFile = open(self.drive + self.directoryName + self.fileName)
        self.meteoData = meteoFile.readlines()
        meteoFile.close()
        
    def length(self):
        return len(self.meteoData)
        
    def readline(self, index):
        self.currentLine = array([self.meteoData[index].split('\t')], dtype=float)
        self.day = self.currentLine[0,0].astype('int')
        self.month = self.currentLine[0,1].astype('int')
        self.year = self.currentLine[0,2].astype('int')
        self.Tmax = self.currentLine[0,3].astype('float')
        self.Tmin = self.currentLine[0,4].astype('float')
        self.RHmax = self.currentLine[0,5]
        self.RHmin = self.currentLine[0,6]
        self.windspeed = self.currentLine[0,7]
        self.Rg = self.currentLine[0,8]
        self.Rain = self.currentLine[0,9]
        
    def getDay(self):
        return self.currentLine[0,0].astype('int')
    
    def getMonth(self):
        return self.currentLine[0,1].astype('int')
        
    def getYear(self):
        return self.currentLine[0,2].astype('int')
        
    def getTMax(self):
        return self.currentLine[0,3].astype('float')
        
    def getTMin(self):     
        return self.currentLine[0,4].astype('float')
        
    def getRHMax(self):
        return self.currentLine[0,5]
        
    def getRHMin(self):
        return self.currentLine[0,6]
    
    def getWindSpeed(self):
        return self.currentLine[0,7]
        
    def getRG(self):
        return self.currentLine[0,8]
        
    def getRain(self):
        
        return self.currentLine[0,9]
        
    
    
    def computeTEff(self):
        TMin = self.getTMin()
        TMax = self.getTMax()
        if (TMin >= TMax):
            if (TMin > TOpt1):
                y = TOpt1 - TBase
            else:
                y = TMin - TBase
        else:
            if (TMin < TOpt1):
                V = ((min(TOpt1, TMax) + TMin) / 2 - TBase) / (TOpt1 - TBase)
            else:
                V = 0    
            if (TMax > TOpt2):
                W = (TLim - (TMax + max(TOpt2, TMin)) / 2) / (TLim - TOpt2)
            else:
                W = 0    
            if (TMax < TOpt1):
                S2=0
            else : 
                if (TMax < TOpt2):
                    S2 = TMax - max(TOpt1, TMin) 
                else:
                    if (TMin > TOpt2) :
                        S2 = 0
                    else :
                        S2= TOpt2 - max(TOpt1, TMin)
            m1 = V * (min(TOpt1, TMax) - TMin)
            m2 = W * (TMax - max(TMin, TOpt2))
            if (TMax <= TBase):           #### ATTENTION NE PAS UTILISER CETTE FONCTION PAS CORRECTE
                y = 0
            else:
                if (TMin >= TLim):
                    y = 0
                else:
                    y = ((m1 + m2 + S2) / (TMax - TMin)) * (TOpt1 - TBase)
            if (y < 0):
                y = 0
        self.TEff = y
        
    def computeTT(self):
        self.TT += self.TEff
        
    def doStep(self, step):
           
        self.readline(step)
        self.compute_jour_julien()
        if self.jour_julien in self.irrigation :
            self.irrigation_day = "IRRIGATION"
        else :
            self.irrigation_day = "PAS_D_IRRIGATION"
        
        self.computeTEff()
        self.computeTT()
        self.computeET0()
    
    def computeET0(self) :
        self.TMoy = (self.getTMax() + self.getTMin()) / 2
        self.HMoy = (self.getRHMin() + self.getRHMax() ) / 2
        
        if self.getTMin() > self.getTMax() :
            self.TMax = self.TMin
        if self.getRHMin() > self.getRHMax() : 
            self.RHMax = self.RHMin
        self.Decli = 0.409*sin(0.0172 * self.jour_julien-1.39)
        self.SunPos = acos( - tan( self.lat_rad ) * tan(self.Decli))  
        self.Sundist = 1 + 0.033*cos( 2 * (pi/365) * self.jour_julien )
        self.Ray_extra = 24 * 60 * 0.0820 / pi * self.Sundist * (self.SunPos * sin(self.Decli) * sin(self.lat_rad) + cos(self.Decli)* cos(self.lat_rad) * sin(self.SunPos) )
        self.RGMax = ( 0.75 + 0.00002 * GlobalVariables.ALTITUDE ) *   self.Ray_extra
        self.day_lenght = 7.64 * self.SunPos
        self.PAR = 0.48 * self.getRG()
        self.esat = 0.3054 * (exp(17.24 * self.getTMax()/ ( self.getTMax() + 237.3)) + exp(17.27 * self.getTMin()/(self.getTMin() + 237.3)))
        self.ea = 0.3054 * (exp(17.27 * self.getTMax()/(self.getTMax() + 237.3))* self.getRHMin() / 100 + exp(17.27 * self.getTMin() / (self.getTMin() + 237.3))* self.getRHMax()/100)
        self.VPD = self.esat - self.ea
        if self.getRG() > self.RGMax :
            self.ratioRg = 1
        else :
            self.ratioRg = self.getRG() /  self.RGMax
        
        self.Rn = 0.77*self.getRG() - (1.35* self.ratioRg -0.35)*(0.34-0.14*(self.ea)**(0.5))*((self.getTMax()+273.16)**4+(self.getTMin()+273.16)**4)*2.45015*10**(-9)
        self.Tlat = 2.501-2.361*10**(-3)*self.getTMoy()
        self.pent_vap_sat = 4098 * (0.6108*exp(17.27 * self.getTMoy()/(self.getTMoy()+237.3)))/((self.getTMoy()+237.3)**2)
        self.Kpsy = 0.00163 * 101.3 * (1-(0.0065*GlobalVariables.ALTITUDE/293))**5.26
        self.erad = 0.408 * self.Rn*self.pent_vap_sat/(self.pent_vap_sat+self.Kpsy*(1+0.34*self.getWindSpeed()))
        self.eaero = ((900/(self.getTMoy()+273.16))*(( self.esat - self.ea)* self.getWindSpeed())*self.Kpsy)/(self.pent_vap_sat+self.Kpsy*(1+0.34*self.getWindSpeed()))
        self.ET0 = self.erad + self.eaero
        #print self.ET0
    
    
    def compute_jour_julien(self) :
        t= time.mktime((self.getYear(), self.getMonth(), self.getDay(), 1, 0, 0, 0, 0, 0))
        self.jour_julien = time.gmtime(t)[7]
        
    def getjour_julien(self) :
        return self.jour_julien
    
    def getTMoy(self) :
        return self.TMoy
    
    def getHMoy(self) :
        return self.HMoy
    
    def getET0(self) :
        return self.ET0
    

