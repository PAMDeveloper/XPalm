
# Compare Xpalm vs CIGE data ----------------------------------------------



# Load packages -----------------------------------------------------------

packs <- c('data.table','shiny',"lubridate", "stringr", "ggplot2",'dplyr','viridis','plotly','tidyr','DT','scales','shinythemes','cowplot')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)



# function ----------------------------------------------------------------

monnb <- function(d) { lt <- as.POSIXlt(as.Date(d, origin="1900-01-01"));
lt$year*12 + lt$mon } 

# compute a month difference as a difference between two monnb's
mondf <- function(d1, d2) { monnb(d2) - monnb(d1) }  


#  Xplam params -----------------------------------------------------------


params = data.table::fread('D:/PAMStudio/dev/git/XPalm/parameters/XPalm_parameters_ref.txt',header=F)%>%
  select(V1,V3)%>%
  mutate(V3=as.numeric(V3))%>%
  data.frame()

colnames(params)=c('Parameter','value')




# meteo -------------------------------------------------------------------

meteo_raw = data.table::fread('D:/PAMStudio/dev/git/XPalm/parameters/meteo.txt')
colnames(meteo_raw)=c('day','month','year','TMax','TMin','HRMax','HRMin','WindSpeed','Rg','Rainfall')

meteo=meteo_raw%>%
  mutate(Date=ymd(paste(year,month,day,sep='-')))

range(meteo$Date)

# load Xpalm output -------------------------------------------------------

result=data.table::fread(file =paste0('../../bin/msvc14/x64/plant.csv'),header=T,skip = 3)

head=colnames(data.table::fread(file =paste0('../../bin/msvc14/x64/plant.csv'),header=T))

colnames(result)=head
result=result%>%
  mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
  select(-time)

PlantingDate=ymd('2011-01-01')

# load CIGE data ----------------------------------------------------------
obs=data.table::fread(file = './Data/Full_Data_Plot_Pheno_SMSE_B22.txt')%>%
  data.frame()%>%
  mutate(PlantingDate=PlantingDate,
         HarvestDate=ymd(HarvestDate),
         MAP=mondf(d2 = HarvestDate,d1 = PlantingDate),
         TreeId=as.factor(paste(Site,Plot,BlockNumber,IdGenotype,LineNumber,TreeNumber,sep='_')),
         Density=136)

range(obs$HarvestDate,na.rm=T)
range(obs$MAP,na.rm=T)/12


AGE_INI_d=min(meteo$Date)-PlantingDate

# grph --------------------------------------------------------------------
 

obs%>%
  ggplot(aes(x=MAP))

result%>%
  mutate(month=paste0(month(Date),'/',year(Date)),
         FFB=BUNCH_NONOIL_BIOMASS_HARVESTED+BUNCH_OIL_BIOMASS_HARVESTED)%>%
  group_by(month)%>%
  summarize(nb_days=n(),
            age=TREE_AGE/365,
            yield_FFB=sum(FFB)/1000,
            yield_oil=sum(BUNCH_OIL_BIOMASS_HARVESTED)/1000)%>%
  filter(nb_days>27)%>%
  ungroup()%>%
  ggplot()+
  geom_point(aes(x=my(month),y=yield_FFB,col='FFB Xpalm',label=paste('age:',round(age,2))))+
  geom_line(aes(x=my(month),y=yield_FFB,col='FFB Xpalm'))+
  # geom_point(aes(x=my(month),y=yield_oil,col='Oil',label=paste('age:',round(age,2))))+
  # geom_line(aes(x=my(month),y=yield_oil,col='Oil'))+
  labs(x='',
       y='kg.tree-1')+
  scale_color_discrete(name='')
