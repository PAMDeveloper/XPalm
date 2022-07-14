###script to test and visualize outputs of Xpalm at tree scale###
##R PEREZ, March 2020



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr','viridis','tidyr','data.table','plotly','cowplot')
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
# import data -------------------------------------------------------------


don_tree=data.table::fread(file =paste0('../../bin/msvc14/x64/results.csv'),header=T)


don_tree%>%
  ggplot()+
  geom_point(aes(x=Date,y=GROWTH_DEMAND))


don_tree%>%
  ggplot()+
  geom_point(aes(x=Date,y=RESPI_MAINTENANCE,col='respi'))+
  geom_point(aes(x=Date,y=ASSIM,col='ASSIM'))+
  geom_point(aes(x=Date,y=ASSIM_TO_GROWTH,col='ASSIM_TO_MIN_RES'))+
  geom_point(aes(x=Date,y=ASSIM_TO_GROWTH,col='ASSIM_TO_GROWTH'))+
  geom_point(aes(x=Date,y=ASSIM_TO_RES,col='ASSIM_TO_RES'))+
  geom_point(aes(x=Date,y=ASSIM_TO_RESPI,col='ASSIM_TO_RESPI'))

don_tree%>%
  filter(Date==ymd('2014-01-01'))%>%
    mutate(Cbal=ASSIM - ASSIM_TO_MIN_RES - ASSIM_TO_RESPI - ASSIM_TO_GROWTH - ASSIM_TO_RES - ASSIM_EXCESS)
