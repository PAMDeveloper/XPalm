#!/usr/bin/env python

import os
import multiprocessing
import time

from Meteo import Meteo
from Tree import Tree
from GlobalVariables import GlobalVariables

from matplotlib import *
from pylab import *

#os.chdir(GlobalVariables.DRIVE + GlobalVariables.DIRECTORYNAME)
#execfile('D:/Benoit/2011/Simulateur_python/Pasdenom-multiarbres-avec_SLA_et_retard/Simulation.py')"""
#execfile('D:/Mes donnees/copie_PC/Plantes/Elaeis/XPalm/Pasdenom-multiarbres-avec_nb_fruits_masse_fruits/Simulation.py')

#import compileall
#compileall.compile_dir(".", force = 1 )

class Simulation(object):

    def __init__(self, treeName):
        self.meteo = Meteo("meteo.txt")
        self.meteo.initMeteo()
        self.step = 0    
        self.record = zeros((1,5))
        self.index = 1
        self.tree = Tree(self, treeName)
        
    
    def getSimulation(self):
        return self.Simulation()
        
    def getStep(self) :
        return self.step
    
    def getMeteo(self) :
        return self.meteo
    
    def simulationLoop(self, queue):
        #fichier = open("fichier.txt", "w") # Ouvre le fichier.
            
        #numberOfStep = self.meteo.length()
        if GlobalVariables.NUMBER_OF_SIMULATION_STEP > self.meteo.length() :
            #print 'nombre_de_jours_simulation_trop_eleve'
            #print 'longueur du fichier meteo', self.meteo.length()
            sys.exit()
        else : 
            numberOfStep = GlobalVariables.NUMBER_OF_SIMULATION_STEP
        
        mem = 0
        for self.step in range(0, numberOfStep):    # numberOfStep
            #print '------------------------------------------------------------'
            self.meteo.doStep(self.step)
            
            self.tree.doStep(self.step, self.meteo.getTT(), self.meteo.getTEff(), self.meteo.getDay(), self.meteo.getMonth(), self.meteo.getYear())
            if ( mem != self.meteo.getYear() ) :
                print ('Tree : ', self.tree.name, ' year= ' , self.meteo.getYear())
            mem = self.meteo.getYear()
        plantation = queue.get()
        plantation[self.tree.name] = self.tree
        queue.put(plantation)
        
    def displayTree(self):
        self.tree.display()
    
    def write_results(self) :
        savetxt("simulation.txt", self.record)  
    
def sim(queue, treeName):
    simulation = Simulation(treeName)
    plantation = queue.get()
    plantation[simulation.tree.name] = simulation.tree    
    queue.put(plantation)    
    simulation.simulationLoop(queue)    
    #print plantation[simulation.tree.name].record    
    
        
def aggregateData():
    
    
        
    
    #print "P2:", plantation
    
    output_name = ""
    
    
    ### ecriture du fichier mois par arbre 
    aggregatedData = {}
    for key in sorted(plantation):
        tree = plantation[key]
        nom = "production_par_arbres" +  str(tree.name) + ".txt"
        f = open(output_name + "production_mois/" +  nom,"w")
        f.write("month" + '\t' + "year" + '\t' + "biomasse_recolte" + '\t' + "nombre_recolte" + '\t' + "Potential_biomass" + '\t' + "IC_spikelet" + '\t' + "IC_setting" + '\t' + "pot_number" + '\t' + "nombre_fruits" + '\t' + "nombre_fruits_pot" +  '\t' + "ind_fruit_weight" + '\t' + "ratio_huile" + "\n")
        for line in tree.record:
            
            month = line[1]
            year = line[2]
            biomass = line[3]
            bunchNb = line[4]
            pot_biomass = line[5]
            if bunchNb != 0 :
                IC_spikelet = line[6] / bunchNb
                IC_setting = line[7] / bunchNb
                fruit_nombre = line[9] / bunchNb
                pot_fruit_nombre = line[10] / bunchNb
                ind_fruit_weight = line[11]/ bunchNb
                ratio_huile = line[12] / bunchNb
            else :
                IC_spikelet = 0
                IC_setting = 0
                fruit_nombre = 0
                pot_fruit_nombre = 0
                ind_fruit_weight =  0
                ratio_huile = 0 
            IC_spikelet_tot = line[6]
            IC_setting_tot = line[7]
            pot_number = line[8]
            
            f.write(str(month) +'\t'+ str(year) +'\t'+ str(biomass) +'\t'+ str(bunchNb) +'\t'+ str(pot_biomass)+ '\t' + str(IC_spikelet) + '\t' + str(IC_setting)+ '\t' + str(pot_number) + '\t' + str(fruit_nombre) + '\t' + str(pot_fruit_nombre) + '\t' + str(ind_fruit_weight) + '\t' + str(ratio_huile) + "\n")
            if (year not in aggregatedData):
                #print 'nouvelle annee'
                dictMonth = {}
                dictMonth[month] = [biomass, bunchNb, pot_biomass, IC_spikelet, IC_setting, IC_spikelet_tot, IC_setting_tot, pot_number ]                
                aggregatedData[year] = dictMonth
                #print aggregatedData[year]
            elif (month not in aggregatedData[year]):
                #print 'nouveau mois'
                #dictMonth = {}
                dictMonth = aggregatedData[year]
                dictMonth[month] = [biomass, bunchNb, pot_biomass, IC_spikelet, IC_setting, IC_spikelet_tot, IC_setting_tot, pot_number]
                #print 'year', year
                aggregatedData[year] = dictMonth
                #print aggregatedData[year]
            else:
                #print 'rien de nouveau'
                aggregatedData[year][month][0] += biomass
                aggregatedData[year][month][1] += bunchNb
                aggregatedData[year][month][2] += pot_biomass
                aggregatedData[year][month][3] += IC_spikelet
                aggregatedData[year][month][4] += IC_setting
                aggregatedData[year][month][5] += IC_spikelet_tot
                aggregatedData[year][month][6] += IC_setting_tot
                aggregatedData[year][month][7] += pot_number
                
                #print aggregatedData[year]
        
        f.close()
    
    ### aggregated du fichier annee arbre/parcelle
    
    
    
    
    
    
    f=open(output_name +  "production_mois/" + "production_parcelle.txt","w")
    f.write("month" + '\t' + "year" + '\t' + "biomasse_recolte" + '\t' + "nombre_recolte" + '\t' + "Potential_biomass" +'\t' + "IC_spikelet" + '\t' +"IC_setting" +'\t'  + "pot_number" + "\n")
    for keyYear in aggregatedData:
        #print keyYear
        for keyMonth in aggregatedData[keyYear]:
            #print keyMonth
            if aggregatedData[keyYear][keyMonth][1] != 0 :
                IC_spikelet = aggregatedData[keyYear][keyMonth][5] / aggregatedData[keyYear][keyMonth][1]
                IC_setting = aggregatedData[keyYear][keyMonth][6] / aggregatedData[keyYear][keyMonth][1]
            else :
                IC_spikelet = 0
                IC_setting = 0 
            tmp = str(keyYear) + str(keyMonth) + str(aggregatedData[keyYear][keyMonth][0]) + str(aggregatedData[keyYear][keyMonth][1])
            f.write(str(keyMonth) + '\t' + str(keyYear) + '\t' + str(aggregatedData[keyYear][keyMonth][0]) + '\t' + str(aggregatedData[keyYear][keyMonth][1])+ '\t' + str(aggregatedData[keyYear][keyMonth][2]) + '\t' + str(IC_spikelet) + '\t' + str(IC_setting)+ '\t' +  str(aggregatedData[keyYear][keyMonth][7])+"\n")
            #print "Year: ", keyYear, " Month: ", keyMonth, " Biomass : ", aggregatedData[keyYear][keyMonth][0], " Bunch number: ", aggregatedData[keyYear][keyMonth][1]
    f.close()
    
    
    aggregatedData_year = {}
    for key in sorted(plantation):
        tree = plantation[key]
        aggregatedData_year_arbre = {}
        
        for line in tree.record:
            
            month = line[1]
            year = line[2]
            biomass = line[3]
            bunchNb = line[4]
            pot_biomass = line[5]
            if bunchNb != 0 :
                IC_spikelet = line[6] / bunchNb
                IC_setting = line[7] / bunchNb
            else :
                IC_spikelet = 0
                IC_setting = 0
            IC_spikelet_tot = line[6]
            IC_setting_tot = line[7]
            pot_number = line[8]
                
            if (year not in aggregatedData_year):
                aggregatedData_year[year] = [biomass, bunchNb, pot_biomass, IC_spikelet, IC_setting, IC_spikelet_tot, IC_setting_tot, pot_number ]
                
            else :
                
                aggregatedData_year[year][0] += biomass
                aggregatedData_year[year][1] += bunchNb
                aggregatedData_year[year][2] += pot_biomass
                aggregatedData_year[year][3] += IC_spikelet
                aggregatedData_year[year][4] += IC_setting
                aggregatedData_year[year][5] += IC_spikelet_tot
                aggregatedData_year[year][6] += IC_setting_tot
                aggregatedData_year[year][7] += pot_number
            
            if (year not in aggregatedData_year_arbre):
                aggregatedData_year_arbre[year] = [biomass, bunchNb, pot_biomass, IC_spikelet, IC_setting, IC_spikelet_tot, IC_setting_tot, pot_number ]
                aggregatedData_year_arbre[year] =  [biomass, bunchNb, pot_biomass, IC_spikelet, IC_setting, IC_spikelet_tot, IC_setting_tot, pot_number ]
                
            else :
                
                aggregatedData_year_arbre[year][0] += biomass
                aggregatedData_year_arbre[year][1] += bunchNb
                aggregatedData_year_arbre[year][2] += pot_biomass
                aggregatedData_year_arbre[year][3] += IC_spikelet
                aggregatedData_year_arbre[year][4] += IC_setting
                aggregatedData_year_arbre[year][5] += IC_spikelet_tot
                aggregatedData_year_arbre[year][6] += IC_setting_tot
                aggregatedData_year_arbre[year][7] += pot_number    
        #print tree.name
        #print aggregatedData_year_arbre
        #print '-------------------------'
       
       # ecriture du fichier annee par arbre
    
        
        nom = "production_par_arbres_annee" +  str(tree.name) + ".txt"
        
        f = open(output_name + "production_annee/" +  nom,"w")
        
        f.write( "year" + '\t' + "biomasse_recolte" + '\t' + "nombre_recolte" + '\t' + "Potential_biomass" + '\t' + "IC_spikelet" + '\t' + "IC_setting" + '\t' + "pot_number" + "\n")
        for keyYear in aggregatedData_year_arbre :
            if aggregatedData_year_arbre[keyYear][1] != 0 :
                IC_spikelet = aggregatedData_year_arbre[keyYear][5] / aggregatedData_year_arbre[keyYear][1]
                IC_setting = aggregatedData_year_arbre[keyYear][6] / aggregatedData_year_arbre[keyYear][1]
            else :
                IC_spikelet = 0
                IC_setting = 0
            f.write(str(keyYear) + '\t' + str(aggregatedData_year_arbre[keyYear][0]) + '\t' + str(aggregatedData_year_arbre[keyYear][1])+ '\t' + str(aggregatedData_year_arbre[keyYear][2]) + '\t' + str(IC_spikelet) + '\t' + str(IC_setting)+ '\t' +  str(aggregatedData_year_arbre[keyYear][7])+"\n")
        f.close()
        
    #print 'total'
    #print aggregatedData_year    
    
    ## ecriture du fichier a l echelle de la parcelle et annee
    
    f=open(output_name + "production_annee/" +  "production_parcelle_annee.txt","w")
    #print "ouvert parcelle"
    f.write("year" + '\t' + "biomasse_recolte" + '\t' + "nombre_recolte" + '\t' + "Potential_biomass" + '\t' + "IC_spikelet" + '\t' + "IC_setting" + '\t' +"pot_number" +  "\n")
    for keyYear in aggregatedData_year :
        if aggregatedData_year[keyYear][1] != 0 :
            IC_spikelet = aggregatedData_year[keyYear][5] / aggregatedData_year[keyYear][1]
            IC_setting = aggregatedData_year[keyYear][6] / aggregatedData_year[keyYear][1]
        else :
            IC_spikelet = 0
            IC_setting = 0 
        f.write(str(keyYear) + '\t' + str(aggregatedData_year[keyYear][0]) + '\t' + str(aggregatedData_year[keyYear][1])+ '\t' + str(aggregatedData_year[keyYear][2]) + '\t' + str(IC_spikelet) + '\t' + str(IC_setting)+ '\t' +  str(aggregatedData_year[keyYear][7])+"\n")
            #print "Year: ", keyYear, " Month: ", keyMonth, " Biomass : ", aggregatedData[keyYear][keyMonth][0], " Bunch number: ", aggregatedData[keyYear][keyMonth][1]
    f.close()
    #print "ferme parcelle"
        
    
    
    
    
    aggregatedData_flowering={}
    for key in sorted(plantation):
        nom= "type_inflo_par_arbre" + str(tree.name) + ".txt"
        tree = plantation[key]
        f=open(output_name + "inflo/" + nom ,"w")
        f.write( "month" + '\t' + "year" + '\t' + "nb_avorte" + '\t' + "nb_femelle" + '\t' + "nb_male" + "\n")
        for line in tree.record_flowering :
            month = line[1]
            year = line[2]
            nb_avorte = line[3]
            nb_femelle = line[4]
            nb_male = line[5]
            f.write(str(month) +'\t'+ str(year) +'\t'+ str(nb_avorte) +'\t'+ str(nb_femelle) + '\t' + str(nb_male) + "\n")
            if (year not in aggregatedData_flowering):
                dictMonth = {}
                dictMonth[month] = [nb_avorte,nb_femelle, nb_male ]
                aggregatedData_flowering[year] = dictMonth
            elif (month not in aggregatedData_flowering[year]):
                dictMonth = aggregatedData_flowering[year]
                dictMonth[month] = [nb_avorte,nb_femelle, nb_male ]
                aggregatedData_flowering[year] = dictMonth
            else :
                aggregatedData_flowering[year][month][0] += nb_avorte
                aggregatedData_flowering[year][month][1] += nb_femelle
                aggregatedData_flowering[year][month][2] += nb_male
        f.close()
    
    f=open(output_name + "inflo/" + "type_inflo_parcelle.txt","w")
    f.write ( "month" + '\t' + "year" + '\t' + "nb_avorte" + '\t' + "nb_femelle" + '\t' + "nb_male" + '\t' + "pourc_A" + '\t' + "pourc_F" +'\t' + "pourc_M" + "\n"   )
    for keyYear in aggregatedData_flowering :
        #print keyYear
        for keyMonth in aggregatedData_flowering[keyYear]:
            sum = aggregatedData_flowering[keyYear][keyMonth][0] + aggregatedData_flowering[keyYear][keyMonth][1] + aggregatedData_flowering[keyYear][keyMonth][2]
            if sum != 0 :
                f.write(str(keyYear) + '\t' + str(keyMonth) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][0]) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][1]) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][2]) + '\t' +  str(aggregatedData_flowering[keyYear][keyMonth][0] / sum) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][1] / sum)  + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][2] / sum) + "\n")
            else : 
                f.write(str(keyYear) + '\t' + str(keyMonth) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][0]) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][1]) + '\t' + str(aggregatedData_flowering[keyYear][keyMonth][2]) + '\t' +  str(0) + '\t' + str(0)  + '\t' + str(0) + "\n")
    f.close()
    
    #f=open("resultats_echelle_arbre.txt","w")
    for key in sorted(plantation):
        nom= "resultats_echelle_arbre" + str(tree.name) + ".txt"
        f= open(output_name + "variables_etat/" + nom,"w")
        tree=plantation[key]
        f.write("name"+ '\t'+ "day" + '\t' + "month" + '\t' + "year" + '\t' + "leafArea" + '\t' + "ei" + '\t' + "IC" + '\t' + "biomass_prod" + '\t' + "demand" +'\t' + "reserve" + '\t' + "NF" + '\t' + "hauteur" + "\n" )
        for line in tree.tabmemorise :
            day = line[1]
            month = line[2]
            year = line[3]
            leafArea = line[4]
            ei = line[5]
            IC = line[6]
            biomass_prod = line[7]
            demand = line[8]
            reserve = line[9]
            NF = line[10]
            hauteur = line[11]
            #print line
            f.write(str(tree.name)+'\t'+ str(day)+'\t'+ str(month)+'\t'+ str(year)+'\t'+ str(leafArea)+'\t'+ str(ei)+'\t'+ str(IC)+'\t'+ str(biomass_prod)+'\t'+ str(demand)+'\t'+ str(reserve)+ '\t' + str(NF) + '\t' + str(hauteur) + "\n")
        f.close()
    
        
    for key in sorted(plantation):
        nom = "resultats_echelle_arbre_mois" + str(tree.name) + ".txt"
        f=open(output_name + "variables_etat/" + nom, "w")
        f.write("name" + '\t'+  "month" + '\t' + "year" + '\t' + "prod" + '\t' + "demand" +   "\n" )   
        tree = plantation[key]
        prod_month = 0
        demand_month = 0
        month = tree.tabmemorise[1,2]
        year = tree.tabmemorise[1,3]
        for line in tree.tabmemorise :
            
            if line[2] == month :
                prod_month +=  line[7]
                demand_month += line[8]
            else :
                f.write(str(tree.name)+'\t'+ str(month) + '\t' + str(year) + '\t' + str(prod_month) + "\n" )                
                prod_month = line[7]
                demand_month = line[8]
                month = line[2]                
                if line[3] != year :     
                   year = line[3]
                
        f.close()
            
        
    memorise_moyen=zeros((GlobalVariables.NUMBER_OF_SIMULATION_STEP + 1, 12))
    
    acc = 0
    #for n in range(GlobalVariables.NUMBER_OF_TREES) :
    for key in sorted(plantation) :
        tree=plantation[key]    
        for line in tree.tabmemorise :
            acc += 1
            #print 'name' +'\t' + str(line[0])
            #print 'acc' + str(acc)
            #print 'indice' + '\t' + str(acc - line[0] * GlobalVariables.NUMBER_OF_SIMULATION_STEP - 1)
            #if acc > GlobalVariables.NUMBER_OF_SIMULATION_STEP :
            print (acc, line[0], GlobalVariables.NUMBER_OF_TREES)#, ei[acc - line[0]*GlobalVariables.NUMBER_OF_SIMULATION_STEP], acc - n*GlobalVariables.NUMBER_OF_SIMULATION_STEP    
       #     memorise_moyen[acc - line[0] * GlobalVariables.NUMBER_OF_SIMULATION_STEP - 1 ] += line / GlobalVariables.NUMBER_OF_TREES
            #print line[5], line[5] / GlobalVariables.NUMBER_OF_TREES, ei[acc - line[0]*GlobalVariables.NUMBER_OF_SIMULATION_STEP - 1  ]
            #else : 
            #   print acc, n 
            #   ei[acc] =  line[5]
                
    f=open(output_name +  "variables_etat/" + "resultat_echelle_parcelle.txt","w") 
    f.write("day" + '\t' + "month" + '\t' + "year" + '\t' + "leafArea" '\t' + "ei" + '\t' + "IC" + '\t' + "biomass_prod" + '\t' + "demand" +'\t' + "reserve" + '\t' + "NF" + "\n" )
    for n in range(GlobalVariables.NUMBER_OF_SIMULATION_STEP) :
        f.write(str(memorise_moyen[n,1])  + '\t' + str(memorise_moyen[n,2])+ '\t' + str(memorise_moyen[n,3]) + '\t' + str(memorise_moyen[n,4]) + '\t' + str(memorise_moyen[n,5]) + '\t' + str(memorise_moyen[n,6]) + '\t' + str(memorise_moyen[n,7]) + '\t' + str(memorise_moyen[n,8]) + '\t' + str(memorise_moyen[n,9]) + '\t' + str(memorise_moyen[n,10]) + "\n")
        #print "ei", memorise_moyen[n,5]
    f.close()
    
    memorise_meteo=zeros((GlobalVariables.NUMBER_OF_SIMULATION_STEP + 1, 10))
    acc = 0
    
    for key in sorted(plantation) :
        tree = plantation[key]
        
        for line in tree.tabmemorise_meteo :
            acc += 1
            
            #memorise_meteo[acc - line[0] * GlobalVariables.NUMBER_OF_SIMULATION_STEP - 1 ] += line / GlobalVariables.NUMBER_OF_TREES
            
    f=open(output_name +  "variables_etat/" + "resultat_meteo.txt","w")
    f.write("day" + '\t' + "month" + '\t' + "year" + '\t' + "FTSW" + '\t' + "pluie_efficace" + '\t' + "ETO" + '\t' + "Rg" + '\t' + "TEFF" +  "\n"   )
    for n in range(GlobalVariables.NUMBER_OF_SIMULATION_STEP) :
        f.write(str(memorise_meteo[n,1])  + '\t' + str(memorise_meteo[n,2])+ '\t' + str(memorise_meteo[n,3]) + '\t' + str(memorise_meteo[n,4]) + '\t' + str(memorise_meteo[n,5]) + '\t' + str(memorise_meteo[n,6]) +  '\t' + str(memorise_meteo[n,7]) +  '\t' + str(memorise_meteo[n,8]) + '\t' + str(memorise_meteo[n,9]) + "\n")
        #print "ei", memorise_moyen[n,5]
    f.close()
    
    
if __name__ == '__main__':
    initialTime = time.time()
    plantation = {}
    
    
    q = multiprocessing.Manager().Queue()
    q.put(plantation)
    
    processes = []
    n = GlobalVariables.NUMBER_OF_TREES
    
    
    for i in range(0, n):
        process = multiprocessing.Process(target=sim, args=(q, i))
        processes.append(process)

    for i in range(0, n):
        processes[i].start()

    #for i in range(0, n):
    #    print processes[i].is_alive()
        
    for i in range(0, n):
        processes[i].join()
        
    plantation = q.get()
        
   # print "P: ", plantation
    
    aggregateData()
        
    elapsedTime = time.time() - initialTime
    
    print ("Temps passe : ", elapsedTime )
    
    
