from random import *
from math import *

p=float(0.5)
t=random()
if (t<p):
    print("Succès")
else:
    print("Echec")
    

def sexe_decision(p) :
    t=random()
    if (t<p) :
        self.sex = "FEMELLE"
    else:
        self.sex = "MALE"

def abortion_decision(p) :
    t=random()
    if (t<p) :
        self.abortion = "NON_AVORTE"
    else:
        self.sex = "AVORTE"
    
def generer_gaussienne(mean,sd) :
    u1 = random()
    u2 = random()
    return((-2.0 *log(u1))**(1/2) * cos( 2 * 3.14 * u2) * sd + mean)
 
