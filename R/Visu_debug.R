###script to test and visualize outputs of Xpalm###
##R PEREZ, March 2020



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr','viridis','tidyr','data.table','plotly')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)


# theme -------------------------------------------------------------------

myTheme <- theme(
  panel.background=element_rect(fill="transparent", color=NA),  
  plot.background = element_rect(fill = "transparent",colour = NA),
  axis.line=element_line(colour="black"), 
  axis.title=element_text(size=16),
  axis.text.y=element_text(size=14, colour="black"), 
  axis.text.x=element_text(size=14, colour="black", angle=0, hjust=0.5),
  panel.grid.minor = element_blank(), 
  panel.grid.major = element_line(colour="grey90", size=0.2), 
  legend.position="bottom", 
  legend.text=element_text(size=14),
  legend.title=element_text(size=14)
)



# load functions ----------------------------------------------------------

source('generate_params.R')
source('generate_meteo.R')


# generate param and meteo files ------------------------------------------

age_ini=3
seed=1

generate_param(dir='../parameters/',filename = 'XPalm_parameters',AGE_INI = age_ini,SEED = seed)

meteo_file="meteo_SMSE"
generate_meteo(dir_raw = '../parameters/',filename = meteo_file,date_ini = NULL,date_end = NULL)


# lunch simu --------------------------------------------------------------
###externally

sim_name=paste0('Simu_',meteo_file,'_age',age_ini,'_seed',seed)


# save outputs ------------------------------------------------------------

don=data.table::fread(file =paste0('../../bin/msvc14/x64/results.csv'),header=T)%>%
  mutate(Date=ymd(Date),
         Age=(Date-first(Date))/365)

save(x=don,file =paste0('../Simu/',sim_name,'.RData'))

load(paste0('../Simu/',sim_name,'.RData'))


vars=don%>%
  select(-c(Date,Age))%>%
  colnames()


tableVar=data.frame(var=vars,min=0,max=NA,type='sink & source')
levels(tableVar$type)=c('sink & source','meteo','prod')
tableVar[tableVar$var %in% c('EI','FRACTION_NON_STR_BIOMASS_TOTAL','FR_FRUITS','FR_RESTE','FTSW'),]$max=1
tableVar[tableVar$var %in% c('C_BALANCE'),]$max=0
tableVar[tableVar$var %in% c('FRACTION_NON_STR_BIOMASS_TOTAL'),]$min=2

tableVar[tableVar$var %in% c("FTSW","RG","RAIN","TEFF"),]$type='meteo'
tableVar[tableVar$var %in% c("PLANTLEAFAREA",'LAI',"PHYTOMERNUMBER","NEWPHYTOMEREMERGENCE","LEAVES_NON_STRUCTURAL_BIOMASS_HARVESTED","LEAVES_STRUCTURAL_BIOMASS_HARVESTED","MALE_BIOMASS_HARVESTED","PEDUNCLE_BIOMASS_HARVESTED","BUNCH_NONOIL_BIOMASS_HARVESTED","BUNCH_OIL_BIOMASS_HARVESTED","TRUNK_BIOMASS","TRUNK_HEIGHT"),]$type='prod'



# Visu outputs ------------------------------------------------------------

don2=don%>%
  tidyr::gather(key = 'var',value='value',-c(Date,Age,TREE_AGE))

don2=merge(don2,tableVar)%>%
  mutate(outliers=ifelse((!is.na(value) & value<min) |(!is.na(value) & !is.na(max) & value>max),'yes','no'))


out=don2%>%
  filter(outliers=='yes')

pdf(paste0('../Outputs/',sim_name,'.pdf'),onefile = T)

# print(out)

for (t in unique(don2$type)){
  sub=don2%>%
    filter(type==t)
  
  gr=ggplot()+
    geom_hline(data = sub,aes(yintercept = min),lty=2)+
    geom_hline(data = sub,aes(yintercept = max),lty=2)+
    geom_path(data=sub,aes(x=Date,y=value))+
    geom_point(data=sub[sub$outliers=='yes',],aes(y=value,x=Date,col='outliers'))+
    facet_wrap(~var,scales = 'free_y')+
    myTheme
  
  print(gr)
}

dev.off()

