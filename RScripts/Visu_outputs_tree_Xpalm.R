###script to test and visualize outputs of Xpalm at tree scale###
##R PEREZ, March 2020



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr','viridis','tidyr','data.table','plotly','cowplot')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)



files=list.files(path = '../Simu/',pattern = 'Observer')


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


don_tree=NULL
for (file in files){
  id=paste0('ind_',str_extract(string = file,pattern = "(?<=seed).+?(?=.csv)"))
  site=str_extract(string = file,pattern = "(?<=Observer_).+?(?=_seed)")
  
  raw=data.table::fread(file =paste0('../Simu/',file),header=T)
  sim_tree=raw%>%
    mutate(id=id,site=site,
           Date=ymd(Date),
           FFB=BUNCH_OIL_BIOMASS_HARVESTED+BUNCH_NONOIL_BIOMASS_HARVESTED)
  
  don_tree=rbind(don_tree,sim_tree)
}


###visu outputs after x days
# delay=600
# don_tree=don_tree%>%
#   filter(Date>min(Date)+delay)


# water balance data --------------------------------------------------------------
gr_ftsw=don_tree%>%
  ggplot(aes(x=Date,y=FTSW,col=site))+
  geom_line()+
  myTheme

# IC--------------------------------------------------------------
gr_ic=don_tree%>%
  filter(id=='ind_1')%>%
  ggplot(aes(x=Date,y=IC,col=site,group=paste(id,site)))+
  geom_hline(yintercept = 1)+
  geom_line()+
  ylab('Competiton Index')+
  myTheme




# graph LAI ---------------------------------------------------------------

don_tree%>%
  ggplot(aes(x=Date,y=LAI,col=site,group=paste(id,site)))+
  geom_line()+
  myTheme

# graph ASSIM---------------------------------------------------------------

don_tree%>%
  ggplot(aes(x=Date,y=ASSIM,col=site))+
  facet_wrap(~id)+
  geom_line()



# graph FFB ----------------------------------------------------------------

don_tree%>%
  mutate(sex=ifelse(FFB>0,'female','male'))%>%
  filter(sex=='female' & id!='ind_3')%>%
  ggplot(aes(x=Date,y=FFB/1000,col=site))+
  facet_wrap(~id)+
  geom_point()+
  geom_line()+
  ylab("FFB (kg.bunch-1)")+
  myTheme

prod_cum=don_tree%>%
  mutate(bunch=ifelse(FFB>0,1,0),
         male=ifelse(MALE_BIOMASS_HARVESTED>0,1,0))%>%
  mutate(month=ymd(paste0(str_sub(string =Date,start = 0,end = 7),'-01')))%>%
  group_by(site,id,month)%>%
  summarize(total_bunches=sum(bunch),
            total_male=sum(male),
            total_FFB=sum(FFB/1000))%>%
  ungroup()%>%
  arrange(site,id,month)%>%
  group_by(site,id)%>%
  mutate(cum_bunches=cumsum(total_bunches),
         cum_male=cumsum(total_male),
         cum_FFB=cumsum(total_FFB))%>%
  ungroup()

summary(prod_cum)

gr_bunch=prod_cum%>%
  ggplot(aes(x=month,y=cum_bunches,col=site,group=paste(id,site),lty=id,pch=id))+
  # facet_wrap(~id)+
  geom_line()+
  geom_point()+
  ylab("# harveted bunches")+
  xlab('')+
  myTheme

gr_male=prod_cum%>%
  ggplot(aes(x=month,y=cum_male,col=site,group=paste(id,site),lty=id,pch=id))+
  # facet_wrap(~id)+
  geom_line()+
  geom_point()+
  ylab("# male inflorescence")+
  xlab('')+
  myTheme

gr_FFB=prod_cum%>%
  ggplot(aes(x=month,y=cum_FFB,col=site,group=paste(id,site),lty=id,pch=id))+
  # facet_wrap(~id)+
  geom_line()+
  geom_point()+
  ylab("FFB (kg.plant-1)")+
  xlab('')+
  myTheme



gr_prod=plot_grid(gr_bunch+theme(legend.position = 'none'),gr_male+theme(legend.position = 'none'),gr_FFB,ncol=1)
gr_clim=plot_grid(gr_ftsw+theme(legend.position = 'none'),gr_ic,ncol=1)


plot_grid(gr_clim,gr_prod,ncol=2,labels = c('A','B'))
  