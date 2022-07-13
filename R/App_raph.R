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
                          dataTableOutput('tableParam')
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
    
    updateSelectInput(session = session, inputId = 'x_var', choices = names(res))
    updateSelectInput(session = session, inputId = 'y_var', choices = names(res))
    updateSelectInput(session = session, inputId = 'second_var', choices = names(res))
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
      mutate(Date=ymd(str_sub(time,start = 0,end = 10)))
    
    return(result)
    
  })
  
  phyto <- reactive({
    
    # input$action
    
    # resultPhyto=load_phyto(file =paste0('../../bin/msvc14/x64/phytomer.csv') )
    
    resultPhyto=data.table::fread(file =paste0('../../bin/msvc14/x64/phytomers.csv'),header=F,skip = 3)
    
    head=colnames(data.table::fread(file =paste0('../../bin/msvc14/x64/phytomers.csv'),header=T))
    
    phyto_number=data.frame(path=colnames(data.table::fread(file =paste0('../../bin/msvc14/x64/phytomers.csv'),header=T,skip=1)))%>%
      mutate(n=row_number())%>%
      group_by(n)%>%
      mutate(number=str_extract(string = path,pattern =  "(?<=\\[).+?(?=\\])")[[1]])
    
    
    colnames(resultPhyto)=paste(head,phyto_number$number,sep='#')
    
    
    return(resultPhyto)
    
  })
  
  observe({
    phyto <- phyto()
    if (is.null(phyto)) return(NULL)
    
    inputPhyto=data.frame(key=colnames(phyto))%>%
      separate(key,into = c('var','number'),sep = '#')%>%
      filter(var!='time')
    
    updateSelectInput(session = session, inputId = 'phytomerNumber', choices = sort(as.numeric(unique(inputPhyto$number))))
    updateSelectInput(session = session, inputId = 'variablePhyto_x', choices = unique(inputPhyto$var))
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
             FFB=BUNCH_NONOIL_BIOMASS_HARVESTED+BUNCH_OIL_BIOMASS_HARVESTED)%>%
      group_by(month)%>%
      summarize(nb_days=n(),
                age=TREE_AGE/365,
                yield_FFB=sum(FFB)/1000,
                yield_oil=sum(BUNCH_OIL_BIOMASS_HARVESTED)/1000)%>%
      filter(nb_days>27)%>%
      ungroup()%>%
      ggplot()+
      geom_point(aes(x=my(month),y=yield_FFB,col='FFB',label=paste('age:',round(age,2))))+
      geom_line(aes(x=my(month),y=yield_FFB,col='FFB'))+
      geom_point(aes(x=my(month),y=yield_oil,col='Oil',label=paste('age:',round(age,2))))+
      geom_line(aes(x=my(month),y=yield_oil,col='Oil'))+
      labs(x='',
           y='kg.tree-1')+
      scale_color_discrete(name='')
    
    
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
     
    sub_phyto=phyto%>%
      rename(time=`time#NA`)%>%
      mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
      select(Date,contains(variablePhyto_x))%>%
      select(Date,contains(paste0("#",phytomerNumber)))%>%
      tidyr::gather('number',"value",-c(Date))%>%
      mutate(number=str_remove(string = number,pattern = paste0(variablePhyto_x,'#')))%>%
      filter(number %in% phytomerNumber)%>%
      arrange(Date,number)
    
    sub_state=phyto%>%
      rename(time=`time#NA`)%>%
      mutate(Date=ymd(str_sub(time,start = 0,end = 10)))%>%
      select(Date,contains('STATE'))%>%
      select(Date,contains(paste0("#",phytomerNumber)))%>%
      tidyr::gather('number',"INFLO_STATUS",-c(Date))%>%
      mutate(number=str_remove(string = number,pattern = paste0('STATE','#')),
             INFLO_STATUS=as.numeric(INFLO_STATUS))%>%
      filter(number %in% phytomerNumber)%>%
      arrange(Date,number)
    
    
    sub_state=merge(sub_state,table_state)%>%
      arrange(Date,number)
    
    
    sub_phyto=cbind(sub_phyto,sub_state%>%select(sex,state))
    
    gr_Phyto=sub_phyto%>%
      mutate(num=paste('phyto #',sprintf( '%03d',as.numeric(number))))%>%
      arrange(Date,number)%>%
      group_by(number)%>%
      ggplot(aes(x=Date,y=value,col=sex,pch=state))+
      geom_point()+
      geom_line(aes(group=num))+
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

