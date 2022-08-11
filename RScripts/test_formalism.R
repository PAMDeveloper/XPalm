###script to test and visualize formalism of Xpalm###
##R PEREZ, January 2020



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)




source('Xpalm_functions.R')



# test leaf area expansion -----------------------------------------------
T_EFF_INI=9
TT_sim=T_EFF_INI*seq(0,15*80,1)

PRODUCTIOn_SPEED_INITIAL=0.0141
PRODUCTIOn_SPEED_ADULT=0.0063
AGE_INI=3*365
AGE_FIN=8*365



leaf1=data.frame(leaf='leaf1',age=3*365,TT_sim=TT_sim)%>%
  mutate(prodspeed=age_relative_var(age = age,age_ini =AGE_INI ,age_fin =AGE_FIN ,val_ini =PRODUCTIOn_SPEED_INITIAL ,val_fin = PRODUCTIOn_SPEED_ADULT),
         SF_sim=Leafarea_expand(TTleaf =TT_sim,SFMax = 2.5,INFLEXION = 15*50*T_EFF_INI, COURBURE = 100))

leaf2=data.frame(leaf='leaf2',age=4*365,TT_sim=TT_sim)%>%
  mutate(prodspeed=age_relative_var(age = age,age_ini =AGE_INI ,age_fin =AGE_FIN ,val_ini =PRODUCTIOn_SPEED_INITIAL ,val_fin = PRODUCTIOn_SPEED_ADULT),
         SF_sim=Leafarea_expand(TTleaf =TT_sim,SFMax = 3.5,INFLEXION = 15*50*T_EFF_INI/1.5, COURBURE = 100),
         
  )

don=rbind(leaf1,leaf2)

ggplot(data=don,aes(x =TT_sim,y=SF_sim,col=leaf))+
  geom_line(lwd=2)+
  geom_vline(aes(xintercept =50*15*T_EFF_INI,col='rang -10'))+
  geom_vline(aes(xintercept =60*15*T_EFF_INI,col='rang 0'))+
  geom_vline(aes(xintercept =70*15*T_EFF_INI,col='rang 10'))



Leafarea_expand(TTleaf =10089,SFMax = 6.36,INFLEXION = 15*55*T_EFF_INI, COURBURE = 200)-Leafarea_expand(TTleaf =10089-9.4,SFMax = 6.36,INFLEXION = 15*55*T_EFF_INI, COURBURE = 200)


derive_Leafarea_expand(TTleaf =10089,SFMax = 6.36,INFLEXION = 15*55*T_EFF_INI, COURBURE = 200)*(10089+9.4)


# test ontogeny -----------------------------------------------------------
PRODUCTIOn_SPEED_INITIAL=0.0141
PRODUCTIOn_SPEED_ADULT=0.0063
age=5*365

age_relative_var(age = age,age_ini = 5*365,age_fin = 8*365,val_ini =PRODUCTIOn_SPEED_INITIAL ,val_fin = PRODUCTIOn_SPEED_ADULT)


# INITIAL_SFIND=0
  MAXIMAl_SFIND=12

AGE_INI=0*365
AGE_ADULT=8*365
first_LA=0.5

age_relative_var(age = 5*365,age_ini = 0*365,age_fin = 8*365,val_ini = 0,val_fin = 12)



ggplotly(data.frame(x=seq(5,10,1))%>%
  group_by(x)%>%
  mutate(y=age_relative_var(age =x*365 ,age_ini = AGE_INI,age_fin = AGE_ADULT,val_ini = first_LA,val_fin = MAXIMAl_SFIND))%>%
           ggplot()+
           geom_line(aes(x=x,y=y,col='V0')))
         
         
         
        


# TEST estimation initial number of phytomer ------------------------------

T_EFF_INI=9
PRODUCTION_SPEED_INITIAL = 0.01111111
PRODUCTION_SPEED_ADULT = 0.007407407

LEAF_PRUNING_RANK = 50
INACTIVE_PHYTOMER_NUMBER = 60

AGE_INI=4* 365;
AGE_ADULT = 5.0 * 365; 
AGE_PLANTING = 1.0 * 365;
AGE_START_PROD = 3.0 * 365;



nb_phyto=INACTIVE_PHYTOMER_NUMBER

i=0 ##days

while(i<=AGE_INI){
  i=i+1
  prod_speed=age_relative_var(age = i,age_ini =0 ,age_fin =AGE_ADULT ,val_ini =PRODUCTION_SPEED_INITIAL ,val_fin = PRODUCTION_SPEED_ADULT)
  nb_phyto=nb_phyto+prod_speed*T_EFF_INI
  
}





for (i in 0:round(nb_phyto)){
  n=-i
  print(paste('number ',n))

  rank = -INACTIVE_PHYTOMER_NUMBER-n
  print(paste('rank',rank))
  
}

newLeaf=2
total_phyto=floor(nb_phyto)+newLeaf

for (i in -round(nb_phyto):1){
  rank = newLeaf-INACTIVE_PHYTOMER_NUMBER-i
  print(paste('number ',i))
  print(paste('rank',rank))
  
}

