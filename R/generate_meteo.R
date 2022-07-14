####generate meteo file fo Xpalm####



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr','viridis','tidyr','data.table','plotly')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)

# # import raw meteo----------------------------------------------------------
# 
# met_TOWE=data.table::fread(input = '../R/Data/Meteo_extract_BDD_TOWE.txt')%>%
#   mutate(date=ymd(ObservationDate))
# met_SMSE=data.table::fread(input = '../R/Data/Meteo_extract_BDD_SMSE.txt')%>%
#   mutate(date=ymd(ObservationDate))
# met_PRESCO=data.table::fread(input = '../R/Data/Meteo_extract_BDD_PR.txt')%>%
#   mutate(date=ymd(ObservationDate))
# 
# 
# period_TOWE=range(met_TOWE$date)
# period_SMSE=range(met_SMSE$date)
# period_PR=range(met_PRESCO$date)
# 
# # extract variables -------------------------------------------------------
# 
# 
# met_TOWE=met_TOWE%>%
#   mutate(day=as.numeric(str_sub(string = ObservationDate,start = 9,end = 10)),
#          month=as.numeric(str_sub(string = ObservationDate,start = 6,end = 7)),
#          year=as.numeric(str_sub(string = ObservationDate,start = 0,end = 4)))%>%
#   select(day,month,year,TMax,TMin,HRMax,HRMin,WindSpeed,Rg,Rainfall)
# summary(met_TOWE)
# 
# met_TOWE=met_TOWE%>%
#   mutate(Rainfall=ifelse(is.na(Rainfall),0,Rainfall))
# 
# met_SMSE=met_SMSE%>%
#   mutate(day=as.numeric(str_sub(string = ObservationDate,start = 9,end = 10)),
#          month=as.numeric(str_sub(string = ObservationDate,start = 6,end = 7)),
#          year=as.numeric(str_sub(string = ObservationDate,start = 0,end = 4)))%>%
#   select(day,month,year,TMax,TMin,HRMax,HRMin,WindSpeed,Rg,Rainfall)
# summary(met_SMSE)
# 
# met_PRESCO=met_PRESCO%>%
#   mutate(day=as.numeric(str_sub(string = ObservationDate,start = 9,end = 10)),
#          month=as.numeric(str_sub(string = ObservationDate,start = 6,end = 7)),
#          year=as.numeric(str_sub(string = ObservationDate,start = 0,end = 4)))%>%
#   select(day,month,year,TMax,TMin,HRMax,HRMin,WindSpeed,Rg,Rainfall)
# summary(met_PRESCO)
# 
# 
# 
# # subsetting common dates between sites -----------------------------------
# ###define dates of simul
# 
# date_ini=max(c(period_TOWE[1],period_SMSE[1],period_PR[1]))
# date_fin=min(c(period_TOWE[2],period_SMSE[2],period_PR[2]))
# 
# 
# met_PRESCO_sub=met_PRESCO%>%
#   mutate(date=ymd(paste(year,month,day)))%>%
#   filter(date>=date_ini & date<=date_fin)%>%
#   select(-date)
# 
# met_SMSE_sub=met_SMSE%>%
#   mutate(date=ymd(paste(year,month,day)))%>%
#   filter(date>=date_ini & date<=date_fin)%>%s
#   select(-date)
# 
# met_TOWE_sub=met_TOWE%>%
#   mutate(date=ymd(paste(year,month,day)))%>%
#   filter(date>=date_ini & date<=date_fin)%>%
#   select(-date)
# 
# # export ------------------------------------------------------------------
# 
# write.table(x = met_SMSE,file = '../parameters/meteo_all_SMSE.txt',row.names = F,quote=F,col.names = F,sep='\t')
# write.table(x = met_SMSE,file = '../parameters/meteo_all_PRESCO.txt',row.names = F,quote=F,col.names = F,sep='\t')
# write.table(x = met_TOWE,file = '../parameters/meteo_all_TOWE.txt',row.names = F,quote=F,col.names = F,sep='\t')
# 
# write.table(x = met_SMSE_sub,file = '../parameters/meteo_sub_SMSE.txt',row.names = F,quote=F,col.names = F,sep='\t')
# write.table(x = met_PRESCO_sub,file = '../parameters/meteo_sub_PRESCO.txt',row.names = F,quote=F,col.names = F,sep='\t')
# write.table(x = met_TOWE_sub,file = '../parameters/meteo_sub_TOWE.txt',row.names = F,quote=F,col.names = F,sep='\t')




#' Generate meteo file
#'
#' @param filename name of the raw meteo file
#' @param dir_raw directory of the raw meteo file
#' @param date_ini starting date (dd-mm-yyyy) if NULL takes the first date in the raw file)
#' @param date_end ending date (dd-mm-yyyy)if NULL takes the last date in the raw file
#'
#' @return meteofile  
#' @export meteofile 
#'
#' @examples
#' filename='meteo_SMSE'
#' dir_raw = '../parameters/'
generate_meteo=function(dir_raw,filename,date_ini=NULL,date_end=NULL){
  
  meteofile= read.table(file = paste0(dir_raw,filename,'.txt'),header = F)
  colnames(meteofile)=c('day','month','year','TMax','TMin','HRMax','HRMin','WindSpeed','Rg','Rainfall')
  
  meteofile=meteofile%>%
    mutate(date=dmy(paste(day,month,year)))
  
  min=min(meteofile$date)
  max=max(meteofile$date)
  
  if (!is.null(date_ini)){
    date_ini=dmy(date_ini)
    if(date_ini<min){
      print(paste('error: select date_ini after',min))
      return(NULL)
    }
  }else{date_ini=min}
  
  if (!is.null(date_end)){
    date_end=dmy(date_end)
    if(date_end>max){
      print(paste('error: select date_end before',max))
      return(NULL)
    }
  }else{date_end=max}

  meteofile=meteofile%>%
    filter(date>=date_ini & date<=date_end)%>%
    select(-date)
  
  write.table(x = meteofile,file = '../parameters/meteo.txt',row.names = F,quote=F,col.names = F,sep='\t')
  print(paste('meteo file generated from',date_ini,'to',date_end))
  
  
}

