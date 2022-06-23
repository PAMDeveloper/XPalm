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

# import outputs ----------------------------------------------------------

# visu ----------------------------------------------------------


# test= data.table::fread(file =paste0('../Simu/Phyto_Indonesia_seed1.csv'),header=F)
test= data.table::fread(file =paste0('../../bin/msvc14/x64/test.csv'),header=F)
colnames(test)=c('date','construct','class','varval','V5')


# RANK,NUMBER,LEAFAREA,TT_SINCE_APPEARANCE,TT_SINCE_LEAF_EXPAND,TT_INI_HARVEST,TT_INI_FLOWERING,LEAF_STATE,INFLO_STATUS,SF_FIN

VARS=c('RANK','NUMBER','LEAFAREA','TT_SINCE_APPEARANCE','TT_SINCE_LEAF_EXPAND','TT_INI_HARVEST','TT_INI_FLOWERING','LEAF_STATE','INFLO_STATUS','SF_FIN')




don_raw=test%>%  
  select(-V5)%>%
  filter(construct %in% c('after_compute'))%>%
  mutate(item=str_extract(class, "(?<=\\[).+?(?=\\])"))%>%
  separate(varval,c('var','value'),sep='=')%>%
  mutate(value=as.numeric(value))


unique(don_raw$var)

don=don_raw%>%
  filter(var %in% VARS)%>%
  mutate(date=ymd(date))%>%
  tidyr::spread(var,value)


id_phyto=don%>%
  filter(!is.na(NUMBER))%>%
  select(date,item,NUMBER,RANK,TT_SINCE_APPEARANCE,TT_INI_FLOWERING,TT_INI_HARVEST,SF_FIN)


don_leaf=don%>%
  filter(!is.na(LEAFAREA))%>%
  select(date,item,LEAFAREA,LEAF_STATE,TT_SINCE_LEAF_EXPAND)

don_leaf=merge(don_leaf,id_phyto,all.x=T)

don_inflo=don%>%
  select(date,item,INFLO_STATUS)%>%
  na.omit()


don_phyto=merge(don_leaf,don_inflo,all.x=T,all.y=F)

summary(don_phyto)




# table state inflo -------------------------------------------------------

INFLO_STATUS=c(1,9,10,12,17,18,20,42,50,82,146,266)
sex=c('undeterminated','male','male','male','female','female','female','male','female','female','female','male')
state=c('initiated','initiated','non aborted','aborted','initiated','non aborted','aborted','spathe','spathe','oleosynthesis','harvested','senescent')

table_state=data.frame(INFLO_STATUS,sex,state)
levels(table_state$state)=list("initiated"="initiated","aborted"='aborted','non aborted'='non aborted','spathe'='spathe','oleosynthesis'='oleosynthesis','harvested'='harvested','senescent'='senescent')

table_state=table_state%>%
  mutate(state=as.character(state),
         INFLO_STATUS=as.numeric(INFLO_STATUS))

don_phyto=don_phyto%>%
  mutate(INFLO_STATUS=as.numeric(INFLO_STATUS))%>%
  as.data.frame()

don_phyto=merge(don_phyto,table_state,all.x=T)


don_phyto%>%
  group_by(NUMBER)%>%
  filter(LEAFAREA>0)%>%
  summarise(SF_FIN=unique(LEAFAREA))%>%
  ggplot(aes(x=NUMBER,y=SF_FIN))+
  geom_point()


ggplotly(don_phyto%>%
  group_by(date)%>%
  filter(LEAFAREA>0)%>%
  summarise(numberofLeaves=n(),
            totalLeafArea=sum(LEAFAREA),
            lastLeaf=max(NUMBER))%>%
  ungroup()%>%
  ggplot(aes(x=date,y=numberofLeaves,col=lastLeaf))+
  geom_point())

don_phyto%>%
  group_by(date,sex)%>%
  filter(LEAFAREA>0)%>%
  summarise(NumberofLeaves=n())%>%
  ungroup()%>%
  tidyr::spread(sex,NumberofLeaves)%>%
  mutate(NumberofLeaves=female+male)%>%
  ggplot()+
  geom_line(aes(x=date,y=male,col='male'))+
  geom_line(aes(x=date,y=female,col='female'))+
  geom_line(aes(x=date,y=NumberofLeaves))

don_phyto%>%
  # filter(NUMBER %in% c(-76,-50,-11,5,10,15,24))%>%
  filter(NUMBER %in% seq(-110,40,5))%>%
  mutate(num=paste('phyto #',sprintf( '%03d',NUMBER)))%>%
  arrange(date,NUMBER)%>%
  group_by(NUMBER)%>%
  # filter(RANK>-15 & RANK<65)%>%
  ggplot(aes(x=RANK,y=NUMBER,col=state,shape=sex))+
  # geom_hline(aes(yintercept =SF_FIN),col=2)+
  geom_vline(aes(xintercept =0),lty=2)+
  geom_vline(aes(xintercept =50),lty=2)+
  # geom_vline(aes(xintercept =TT_INI_FLOWERING,lty='flow'))+
  # facet_wrap(~NUMBER)+
  geom_point(size=4)+
  geom_line(aes(group=num))+
  scale_color_viridis_d()+
  ylab('# phytomer')+
  xlab('Leaf rank')+
  myTheme+
  theme(legend.position='right')



sub=don_phyto%>%
  filter(NUMBER %in% c(-10))%>%
  select(date,NUMBER,RANK,TT_SINCE_APPEARANCE,TT_SINCE_LEAF_EXPAND,sex,state,LEAFAREA)









