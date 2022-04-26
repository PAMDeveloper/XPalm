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
age=2*365

age_relative_var(age = age,age_ini = 3*365,age_fin = 8*365,val_ini =PRODUCTIOn_SPEED_INITIAL ,val_fin = PRODUCTIOn_SPEED_ADULT)


INITIAL_SFIND=2.5
MAXIMAl_SFIND=12
age_relative_var(age = age,age_ini = 3*365,age_fin = 8*365,val_ini = INITIAL_SFIND,val_fin = MAXIMAl_SFIND)



data.frame(x=seq(0,16,1))%>%
  group_by(x)%>%
  mutate(y=age_relative_var(age =x ,age_ini = 2,age_fin = 8,val_ini = 0,val_fin = 1))%>%
  ggplot()+
  geom_line(aes(x=x,y=y))


# leaf expansion ---------------------------------------------------------