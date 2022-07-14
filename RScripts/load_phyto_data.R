###function to load phytomers outputs of Xpalm###
##R PEREZ, july 2022



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr','viridis','tidyr','data.table','plotly')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)





# import outputs ----------------------------------------------------------

# visu ----------------------------------------------------------


#' Title
#'
#' @param file : file with phytomers outputs
#' @param VARS : variables at phyto scale 
#' @param VARS_TREE : variables at tree scale
#'
#' @return 
#' @export
#'
#' @examples
load_phyto=function(file=file, VARS=c('RANK','NUMBER','LEAFAREA','TT_SINCE_APPEARANCE','TT_SINCE_LEAF_EXPAND','TT_INI_HARVEST','TT_INI_FLOWERING','LEAF_STATE','INFLO_STATUS','SF_FIN'),VARS_TREE=c('IC','PLANTLEAFAREA','NEWPHYTOMER')){
  
  ##inputs
  # file =paste0('../../bin/msvc14/x64/test.csv')
  # VARS=c('RANK','NUMBER','LEAFAREA','TT_SINCE_APPEARANCE','TT_SINCE_LEAF_EXPAND','TT_INI_HARVEST','TT_INI_FLOWERING','LEAF_STATE','INFLO_STATUS','SF_FIN')
  # VARS_TREE=c('IC','PLANTLEAFAREA','NEWPHYTOMER')
  
  
  ##load data
  dat= data.table::fread(file =file,header=F)
  colnames(dat)=c('date','construct','class','varval','V5')
  

  
  
  don_raw=dat%>%  
    select(-V5)%>%
    filter(construct %in% c('after_compute'))%>%
    mutate(item=str_extract(class, "(?<=\\[).+?(?=\\])"))%>%
    separate(varval,c('var','value'),sep='=')%>%
    mutate(value=as.numeric(value))
  
  
  id_phyto=don_raw%>%
    filter(var %in% VARS)%>%
    mutate(date=ymd(date))%>%
    tidyr::spread(var,value)%>%
    filter(!is.na(NUMBER))%>%
    select(date,item,NUMBER,RANK,TT_SINCE_APPEARANCE,TT_INI_FLOWERING,TT_INI_HARVEST,SF_FIN)
  
  info_tree=don_raw%>%
    filter(var %in% VARS_TREE)%>%
    mutate(date=ymd(date))%>%
    tidyr::spread(var,value)%>%
    filter(!is.na(IC))%>%
    select(date,VARS_TREE)
  
  
  don_leaf=don_raw%>%
    filter(var %in% VARS)%>%
    mutate(date=ymd(date))%>%
    tidyr::spread(var,value)%>%
    filter(!is.na(LEAFAREA))%>%
    select(date,item,LEAFAREA,LEAF_STATE,TT_SINCE_LEAF_EXPAND)
  
  don_leaf=merge(don_leaf,id_phyto,all.x=T)
  
  don_inflo=don_raw%>%
    filter(var %in% VARS)%>%
    mutate(date=ymd(date))%>%
    tidyr::spread(var,value)%>%
    select(date,item,INFLO_STATUS)%>%
    na.omit()
  
  
  don_phyto=merge(don_leaf%>%data.frame(),don_inflo,all.x=T,all.y=F)
  
  don_phyto=merge(don_phyto,info_tree,all.x=T)
  # summary(don_phyto)
  
  
  
  
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
  
  return(don_phyto)
}




# 
# don_phyto%>%
#   # filter(NUMBER %in% c(-76,-50,-11,5,10,15,24))%>%
#   # filter(NUMBER %in% seq(-110,40,5))%>%
#   mutate(num=paste('phyto #',sprintf( '%03d',NUMBER)))%>%
#   arrange(date,NUMBER)%>%
#   group_by(NUMBER)%>%
#   # filter(RANK>-15 & RANK<65)%>%
#   ggplot(aes(x=RANK,y=NUMBER,col=state,shape=sex))+
#   # geom_hline(aes(yintercept =SF_FIN),col=2)+
#   geom_vline(aes(xintercept =0),lty=2)+
#   geom_vline(aes(xintercept =50),lty=2)+
#   # geom_vline(aes(xintercept =TT_INI_FLOWERING,lty='flow'))+
#   # facet_wrap(~NUMBER)+
#   geom_point(size=1)+
#   geom_line(aes(group=num))+
#   scale_color_viridis_d()+
#   ylab('# phytomer')+
#   xlab('Leaf rank')+
#   theme(legend.position='right')
# 
# 
# v='LEAFAREA'
# nb=-50
# 
# don_phyto%>%
#   filter(NUMBER==nb)%>%
#   arrange(date)%>%
#   # filter(RANK>0)%>%
#   ggplot(aes(x=date,y=get(v),col=paste(state,sex),shape=sex))+
#   geom_line()+
#   geom_point()+
#   scale_color_viridis_d()+
#   labs(y=v)




