#
# This is a Shiny web application. You can run the application by clicking
# the 'Run App' button above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

# Load packages -----------------------------------------------------------

packs <- c('data.table','shiny',"lubridate", "stringr", "ggplot2",'dplyr','viridis','plotly','tidyr','DT','scales','shinythemes','cowplot')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)



# source('load_phyto_data.R')


# meteo = data.table::fread('D:/PAMStudio/dev/git/XPalm/parameters/meteo.txt')
params = data.table::fread('D:/PAMStudio/dev/git/XPalm/parameters/XPalm_parameters.txt',header=F)%>%
  select(V1,V3)%>%
  mutate(V3=as.numeric(V3))%>%
  data.frame()

colnames(params)=c('Parameter','value')


### inflo status

INFLO_STATUS=c(1,9,10,12,17,18,20,42,50,82,146,266)
sex=c('undeterminated','male','male','male','female','female','female','male','female','female','female','male')
state=c('initiated','initiated','non aborted','aborted','initiated','non aborted','aborted','spathe','spathe','oleosynthesis','harvested','senescent')

table_state=data.frame(INFLO_STATUS,sex,state)
levels(table_state$state)=list("initiated"="initiated","aborted"='aborted','non aborted'='non aborted','spathe'='spathe','oleosynthesis'='oleosynthesis','harvested'='harvested','senescent'='senescent')

table_state=table_state%>%
  mutate(state=as.character(state),
         INFLO_STATUS=as.numeric(INFLO_STATUS))

# Define UI for application that draws a histogram

# ui ----------------------------------------------------------------------
ui<-
  navbarPage(theme = shinytheme("united"),
             title = " Visu Xpalm outputs",
             
             
             mainPanel(
               tabsetPanel(
                 tabPanel("Tree data",
                          tags$hr(),
                          fluidRow(
                            column(4,
                                   selectizeInput(inputId = 'x_var', label = 'Select X', choices =  '*', multiple = F)),
                            column(4,
                                   selectizeInput(inputId = 'y_var', label = 'Select Y', choices =  '*', multiple = F)),
                            column(4,
                                   selectizeInput(inputId = 'second_var', label = 'Select second Y', choices =  '*', multiple = F)),
                            tags$hr(),
                            fluidRow(
                              column(12,
                                     plotlyOutput('graph1')
                              ),
                              column(12,
                                     plotlyOutput('graph2')
                              )
                            )
                            
                          )
                          
                 ),
                 tabPanel("Yield",
                          tags$hr(),
                          fluidRow(
                            column(12,
                                   plotlyOutput('graphYield')
                            )
                          )
                 ),
                 
                 tabPanel("Phytomer data",
                          tags$hr(),
                          fluidRow(
                            column(4,
                                   selectizeInput(inputId = 'phytomerNumber', label = 'phytomer number', choices =  '*', multiple = T)),
                            column(6,
                                   selectizeInput(inputId = 'variablePhyto_x', label = 'Select phytomer variable X', choices =  '*', multiple = F))),
                          # column(6,
                          #        selectizeInput(inputId = 'variablePhyto_y', label = 'Select phytomer variable  Y', choices =  '*', multiple = F)),
                          
                          tags$hr(),
                          fluidRow(
                            # column(12,
                            #        plotlyOutput('graphPhyto')
                            # ),
                            column(12,
                                   plotlyOutput('graphPhyto')
                            )
                            
                          )
                          # )
                 )
                 ,
                 tabPanel("parameters",
                          tags$hr(),
                          dataTableOutput('tableParam'),
                          # column(12,
                          #        wellPanel(numericInput("LATITUDE", "LATITUDE:",value = 0.97),
                          #                  numericInput("ALTITUDE", "ALTITUDE:",value = 50),
                          #                  numericInput("DENS", "DENS:",value = 143),
                          #                  numericInput("T_BASE", "T_BASE:",value = 15),
                          #                  numericInput("T_OPT1", "T_OPT1:",value = 25),
                          #                  numericInput("T_OPT2", "T_OPT2",value = 30),
                          #                  numericInput("T_LIM", "T_LIM:",value = 40),
                          #                  numericInput("T_EFF_INI", "T_EFF_INI:",value = 9),
                          #                  numericInput("Z1", "Z1:",value = 200),
                          #                  numericInput("Z2", "Z2:",value = 2000),
                          #                  numericInput("H_CR", "H_CR:",value = 0.23),
                          #                  numericInput("H_PF", "H_PF:",value = 0.1),
                          #                  numericInput("H_0", "H_0:",value = 0.15),
                          #                  numericInput("H_PF_Z1", "H_PF_Z1:",value = 0.05),
                          #                  numericInput("VITESSE_RACINES", "VITESSE_RACINES:",value = 0.3),
                          #                  numericInput("FTSW_RACINES", "FTSW_RACINES:",value = 0.2),
                          #                  numericInput("SEUIL_EVAP", "SEUIL_EVAP:",value = 0.5),
                          #                  numericInput("SEUIL_TRANSPI", "SEUIL_TRANSPI:",value = 0.5),
                          #                  numericInput("Z_INIT", "Z_INIT:",value = 2000),
                          #                  numericInput("SEED", "SEED:",value = 1),
                          #                  numericInput("AGE_INI", "AGE_INI:",value = 2),
                          #                  numericInput("AGE_ADULT", "AGE_ADULT:",value = 8),
                          #                  numericInput("AGE_PLANTING", "AGE_PLANTING:",value = 2)))
                          )
                 )
               )
               
             )
             
             # Define server logic required to draw a histogram
             
             server<-function(input, output,session){
               
               
               observe({
                 files <- list.files('./0-data/')
                 
                 updateSelectizeInput(session = session, inputId = 'select_input', choices = files)
               })
               
               
               observe({
                 res <- res()
                 if (is.null(res)) return(NULL)
                 
                 updateSelectInput(session = session, inputId = 'x_var', choices = unique(c('Date',names(res))))
                 updateSelectInput(session = session, inputId = 'y_var', choices = unique(c('PLANTLEAFAREA',names(res))))
                 updateSelectInput(session = session, inputId = 'second_var', choices = unique(c('COMPETITION_INDEX',names(res))))
               })
               
               
               
               x_var<- reactive({
                 input$x_var
               })
               
               y_var<- reactive({
                 input$y_var
               })
               
               
               second_var<- reactive({
                 input$second_var
               })
               
               variablePhyto_x<- reactive({
                 input$variablePhyto_x
               })
               
               # variablePhyto_y<- reactive({
               #   input$variablePhyto_y
               # })
               
               phytomerNumber<-reactive ({
                 input$phytomerNumber
               })
               
               res <- reactive({
                 
                 input$action
                 
                 result=data.table::fread(file =paste0('../../bin/msvc14/x64/plant.csv'),header=T,skip = 3)
                 head=colnames(data.table::fread(file =paste0('../../bin/msvc14/x64/plant.csv'),header=T))
                 colnames(result)=head
                 result=result%>%
                   mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
                   select(-time)
                 
                 return(result)
                 
               })
               
               phyto <- reactive({
                 
                 # input$action
                 
                 # phyto=load_phyto(file =paste0('../../bin/msvc14/x64/phytomer.csv') )
                 
                 phyto=data.table::fread(file =paste0('../../bin/msvc14/x64/phytomers.csv'),header=F,skip = 3)
                 
                 head=colnames(data.table::fread(file =paste0('../../bin/msvc14/x64/phytomers.csv'),header=T))
                 
                 phyto_number=data.frame(path=colnames(data.table::fread(file =paste0('../../bin/msvc14/x64/phytomers.csv'),header=T,skip=1)))%>%
                   mutate(n=row_number())%>%
                   group_by(n)%>%
                   mutate(number=str_extract(string = path,pattern =  "(?<=\\[).+?(?=\\])")[[1]],
                          number=sprintf("%03d",as.numeric(number)))
                 
                 
                 colnames(phyto)=paste(head,phyto_number$number,sep='#')
                 
                 
                 return(phyto)
                 
               })
               
               observe({
                 phyto <- phyto()
                 if (is.null(phyto)) return(NULL)
                 
                 inputPhyto=phyto%>%
                   rename(time=`time# NA`)%>%
                   mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
                   select(contains('LEAF NUMBER'))%>%
                   filter(row_number()==1)%>%
                   tidyr::gather('number',"LeafId")%>%
                   arrange(LeafId)
                   
                 
                 inputPhytoVar=data.frame(key=colnames(phyto))%>%
                   separate(key,into = c('var','number'),sep = '#')%>%
                   filter(var!='time')%>%
                   mutate(number=sprintf("%03d",as.numeric(number)))

                 updateSelectInput(session = session, inputId = 'phytomerNumber', choices = sort(unique(inputPhyto$LeafId)))
                 updateSelectInput(session = session, inputId = 'variablePhyto_x', choices = unique(inputPhytoVar$var)[unique(inputPhytoVar$var)!='LEAF NUMBER'])
                 # updateSelectInput(session = session, inputId = 'variablePhyto_y', choices = unique(inputPhyto$var))
               })
               
               
               
               # visualisation-----------------------------------------------------
               output$graph1<-renderPlotly({
                 res<-res()
                 if (is.null(res)) return(NULL)
                 
                 y_var<- y_var()
                 if (is.null(y_var)) return(NULL)
                 
                 
                 x_var<- x_var()
                 if (is.null(x_var)) return(NULL)
                 
                 second_var<-  second_var()
                 if (is.null(second_var)) return(NULL)
                 
                 
                 
                 gr1=NULL
                 
                 
                 if (!is.null(x_var) & !is.null(y_var)){
                   
                   
                   gr1=res%>%
                     # arrange(Date)%>%
                     ggplot(aes(x=get(x_var),y=get(y_var),label=get(second_var)))+
                     geom_line()+
                     geom_point()+
                     labs(x=paste(x_var),
                          y=paste(y_var))
                   
                   
                 }
                 
                 
                 if(!is.null(gr1)){
                   ggplotly(gr1)
                 }
                 
               })
               
               output$graph2<-renderPlotly({
                 res<-res()
                 if (is.null(res)) return(NULL)
                 
                 
                 x_var<- x_var()
                 if (is.null(x_var)) return(NULL)
                 
                 y_var<- y_var()
                 if (is.null(y_var)) return(NULL)
                 
                 
                 second_var<-  second_var()
                 if (is.null(second_var)) return(NULL)
                 
                 
                 gr2=NULL
                 
                 
                 
                 if (!is.null(x_var) & !is.null(second_var)){
                   
                   
                   gr2=res%>%
                     # arrange(Date)%>%
                     ggplot(aes(x=get(x_var),y=get(second_var),label=get(y_var)))+
                     geom_line()+
                     geom_point()+
                     labs(x=paste(x_var),
                          y=paste(second_var))
                   
                 }
                 
                 
                 if(!is.null(gr2)){
                   ggplotly(gr2)
                 }
                 
               })
               
               ### visu yield
               
               output$graphYield<-renderPlotly({
                 res<-res()
                 if (is.null(res)) return(NULL)
                 
                 grY=res%>%
                   mutate(month=paste0(month(Date),'/',year(Date)),
                          FFB=BIOMASS_BUNCH_NONOIL_HARVESTED+BIOMASS_BUNCH_OIL_HARVESTED)%>%
                   group_by(month)%>%
                   summarize(nb_days=n(),
                             age=TREE_AGE/365,
                             yield_FFB=sum(FFB)/1000,
                             yield_oil=sum(BIOMASS_BUNCH_OIL_HARVESTED)/1000)%>%
                   filter(nb_days>27 & yield_oil>0)%>%
                   ungroup()%>%
                   ggplot()+
                   geom_point(aes(x=my(month),y=yield_FFB,col='FFB',label=paste('age:',round(age,2))))+
                   geom_line(aes(x=my(month),y=yield_FFB,col='FFB'))+
                   geom_point(aes(x=my(month),y=yield_oil,col='Oil',label=paste('age:',round(age,2))))+
                   geom_line(aes(x=my(month),y=yield_oil,col='Oil'))+
                   labs(x='',
                        y='kg.tree-1')+
                   scale_color_discrete(name='')+
                   xlim(range(res$Date,na.rm=T))
                 
                 
                 if(!is.null(grY)){
                   ggplotly(grY)
                 }
                 
               })
               
               
               #### visu phyto
               output$graphPhyto<-renderPlotly({
                 
                 phyto<-phyto()
                 if (is.null(phyto)) return(NULL)
                 
                 phytomerNumber<- phytomerNumber()
                 if (is.null(phytomerNumber)) return(NULL)
                 
                 
                 variablePhyto_x<- variablePhyto_x()
                 if (is.null(variablePhyto_x)) return(NULL)
                 
                 # variablePhyto_y<- variablePhyto_y()
                 # if (is.null(variablePhyto_y)) return(NULL)
                 
                 
                 inputPhytoNumber=inputPhyto%>%
                   filter(LeafId %in% phytomerNumber)%>%
                   mutate(number=str_remove(string = number,'LEAF NUMBER'))
                 
                 sub_phyto=phyto%>%
                   rename(time=`time# NA`)%>%
                   mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
                   select(Date,contains(variablePhyto_x))%>%
                   select(Date,contains(inputPhytoNumber$number))%>%
                   tidyr::gather('number',"value",-c(Date))%>%
                   mutate(number=str_remove(string = number,pattern = variablePhyto_x))%>%
                   filter(number %in% inputPhytoNumber$number)%>%
                   arrange(Date,number)
                 
                 sub_state=phyto%>%
                   rename(time=`time# NA`)%>%
                   mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
                   select(Date,contains('STATE'))%>%
                   select(Date,contains(inputPhytoNumber$number))%>%
                   tidyr::gather('number',"INFLO_STATUS",-c(Date))%>%
                   mutate(number=str_remove(string = number,pattern = 'STATE'),
                          INFLO_STATUS=as.numeric(INFLO_STATUS))%>%
                   filter(number %in% inputPhytoNumber$number)%>%
                   arrange(Date,number)
                 
                 
                 sub_state=merge(sub_state,table_state)%>%
                   arrange(Date,number)
                 
                 alpha=min(0.1,1/length(phytomerNumber))
                 
                 sub_phyto=merge(cbind(sub_phyto,sub_state%>%select(sex,state)),
                                       inputPhytoNumber)
                 
                 gr_Phyto=sub_phyto%>%
                   mutate(num=paste('phyto #',sprintf( '%03d',as.numeric(LeafId))))%>%
                   arrange(Date,number)%>%
                   group_by(number)%>%
                   ggplot(aes(x=Date,y=value,col=sex,pch=state,label=num))+
                   geom_point(alpha=alpha)+
                   geom_line(aes(group=num),alpha=alpha)+
                   scale_color_viridis_d()+
                   ylab(variablePhyto_x)+
                   # scale_shape_manual(values = c(undeterminated=1,male=2,female=15))+
                   # xlab(variablePhyto_y)
                   theme(legend.position='right')
                 
                 if(!is.null(gr_Phyto)){
                   ggplotly(gr_Phyto)
                 }
                 
               })
               
               
               # output$graphPhyto2<-renderPlotly({
               #   
               #   phyto<-phyto()
               #   if (is.null(phyto)) return(NULL)
               #   
               #   variablePhyto<- variablePhyto()
               #   if (is.null(variablePhyto)) return(NULL)
               #   
               #   
               #   nb<- phytomerNumber()
               #   if (is.null(nb)) return(NULL)
               #   
               #   gr_Phyto=phyto%>%
               #     filter(NUMBER %in% nb)%>%
               #     arrange(date)%>%
               #     # filter(RANK>0)%>%
               #     ggplot(aes(x=date,y=get(variablePhyto),col=paste(state,sex),shape=sex))+
               #     geom_line()+
               #     geom_point()+
               #     scale_color_viridis_d()+
               #     labs(y=variablePhyto)+
               #     facet_wrap(~paste('phytomer',NUMBER))
               #   
               #   if(!is.null(gr_Phyto)){
               #     ggplotly(gr_Phyto)
               #   }
               #   
               # })
               
               output$tableParam = renderDataTable({
                 params
               }, options = list(lengthMenu = c(5, 30, 50,nrow(params)), pageLength = nrow(params)))
               
             }
             
             
             # Run the application
             shinyApp(ui = ui, server = server)
             
             