#
# This is a Shiny web application. You can run the application by clicking
# the 'Run App' button above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/


### Shiny app to visulaize XPalm outputs at plant and phytomer scale
### RPA PEREZ, G. BEURIER, July 2022


# Load packages -----------------------------------------------------------

packs <- c('data.table','shiny',"lubridate", "stringr", "ggplot2",'dplyr','viridis','plotly','tidyr','DT','scales','shinythemes','cowplot','XPalm','Rcpp')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)



# inputs ------------------------------------------------------------------


### inflo status

INFLO_STATUS=c(1,9,10,12,17,18,20,42,50,82,146,266)
sex=c('undeterminated','male','male','male','female','female','female','male','female','female','female','male')
state=c('initiated','initiated','non aborted','aborted','initiated','non aborted','aborted','spathe','spathe','oleosynthesis','harvested','senescent')

table_state=data.frame(INFLO_STATUS,sex,state)
levels(table_state$state)=list("initiated"="initiated","aborted"='aborted','non aborted'='non aborted','spathe'='spathe','oleosynthesis'='oleosynthesis','harvested'='harvested','senescent'='senescent')

table_state=table_state%>%
  mutate(state=as.character(state),
         INFLO_STATUS=as.numeric(INFLO_STATUS))


parameters = read.csv("./parameters/XPalm_parameters.txt",header=F,sep='=')

colnames(parameters)=c('parameter','value')

paramDef=data.table::fread("./parameters/parameters_def.csv")%>%
  mutate(parameter=paste0(parameter,' '))


parameters=merge(parameters,paramDef,all.x=T)


varDef=data.table::fread("./parameters/variables_def.csv")

# -------------------------------------------------------------------------


ui<-navbarPage(theme = shinytheme("united"),
               title = " Visu Xpalm outputs",

               sidebarPanel(
                 fileInput("meteoFile", "Choose meteo File", accept = ".txt"),
                 tags$hr(),
                 actionButton("run", "Run XPalm")),

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
                                     selectizeInput(inputId = 'second_y_var', label = 'Select second Y', choices =  '*', multiple = F)),
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

                            tags$hr(),
                            fluidRow(
                              column(12,
                                     plotlyOutput('graphPhyto')
                              )

                            )

                   )
                   ,
                   tabPanel("edit parameters",
                            tags$hr(),
                            dataTableOutput('table1'))
                   ,
                   tabPanel("meteo",
                            tags$hr(),
                            dataTableOutput('meteoTable')

                   )
                 )
               )
)

server = function(input,output,session){



  # load meteo file ---------------------------------------------------------


  meteo<- reactive({
    file <- input$meteoFile
    ext <- tools::file_ext(file$datapath)

    req(file)

    validate(need(ext == "txt", "Please upload a txt file"))

    meteo <-read.table(file$datapath, header=F, sep="\t")
    colnames(meteo)=c('day','month','year','TMax','TMin','HRMax','HRMin','WindSpeed','Rg','Rainfall')
    return(meteo)
  })


  # view meteo file ---------------------------------------------------------

  output$meteoTable= renderDT({
    meteo <- meteo()
    if (is.null(meteo)) return(NULL)

    datatable(meteo,editable = F)
  }, options = list(lengthMenu = 100, pageLength = 100))


  # table edit parameters ----------------------------------------------------------


  param<- reactiveValues(data=parameters)

  output$table1= renderDT({
    datatable(param$data,editable = TRUE)
  }, options = list(lengthMenu =20, pageLength = 100))


  observeEvent(input$table1_cell_edit, {
    row  <- input$table1_cell_edit$row
    clmn <- input$table1_cell_edit$col
    param$data[row, clmn] <- input$table1_cell_edit$value
  })


  observeEvent(input$replacevalues, {
    param$data <- fillvalues(param$data, input$textbox, input$selectcolumn)
  })




  # run Xpalm ---------------------------------------------------------------


  res<-eventReactive(input$run, {

    meteo <- meteo()
    if (is.null(meteo)) return(NULL)

    ### debug

    # varDef=data.table::fread("./parameters/variables_def.csv")
    # meteo = read.table('./parameters/meteo.txt', header=F, sep="\t")
    # colnames(meteo)=c('day','month','year','TMax','TMin','HRMax','HRMin','WindSpeed','Rg','Rainfall')
    # parameters = read.csv("./parameters/XPalm_parameters.txt",header=F,sep='=')
    # colnames(parameters)=c('parameter','value')
    # paramDef=data.table::fread("./parameters/parameters_def.csv")%>%
    # mutate(parameter=paste0(parameter,' '))
    # parameters=merge(parameters,paramDef)
    # XPalm::runDF(parameters, meteo)
    # Hdrs = XPalm::getHeaders()
    # Vals = XPalm::getValues()


    XPalm::runDF(param$data, meteo)
    Hdrs = XPalm::getHeaders()
    Vals = XPalm::getValues()


    fullHdrs=data.frame(t(Hdrs[1:3,]),row.names = NULL)
    colnames(fullHdrs)=c('var','class','code')

    res=list(fullHdrs=fullHdrs,Vals=Vals)


    return(res)
  })

  # view outputs ---------------------------------------------------------

  phyto <- reactive({


    res <- res()
    if (is.null(res)) return(NULL)


    meteo <- meteo()
    if (is.null(meteo)) return(NULL)


    meteo=meteo%>%
      mutate(Date=ymd(paste(year,month,day,sep='-')))

    phytoHdrs=res$fullHdrs%>%
      filter(class!='Tree')%>%
      mutate(n=row_number())%>%
      group_by(n)%>%
      mutate(number=str_extract(string = class,pattern =  "(?<=\\[).+?(?=\\])")[[1]],
             number=sprintf("%03d",as.numeric(number)))


    phyto=res$Vals%>%
      select(phytoHdrs$code)

    colnames(phyto)=paste(phytoHdrs$var,phytoHdrs$number,sep='#')

    phyto=phyto%>%
      mutate(Date=meteo$Date)

    return(phyto)

  })

  plant <- reactive({

    res <- res()
    if (is.null(res)) return(NULL)

    meteo <- meteo()
    if (is.null(meteo)) return(NULL)



    meteo=meteo%>%
      mutate(Date=ymd(paste(year,month,day,sep='-')))


    plantHdrs=res$fullHdrs%>%
      filter(class %in% c('Tree','Tree/model::WaterBalance','Tree/model::Meteo'))

    ValsPlant=res$Vals%>%
      select(plantHdrs$code)

    colnames(ValsPlant)=plantHdrs$var
    plant=ValsPlant%>%
      mutate(Date=meteo$Date)

    return(plant)

  })


  observe({
    plant <- plant()
    if (is.null(plant)) return(NULL)


    updateSelectInput(session = session, inputId = 'x_var', choices = unique(c('Date',names(plant))))
    updateSelectInput(session = session, inputId = 'y_var', choices = unique(c('PLANTLEAFAREA',names(plant))))
    updateSelectInput(session = session, inputId = 'second_y_var', choices = unique(c('COMPETITION_INDEX',names(plant))))
  })



  x_var<- reactive({
    input$x_var
  })

  y_var<- reactive({
    input$y_var
  })


  second_y_var<- reactive({
    input$second_y_var
  })

  variablePhyto_x<- reactive({
    input$variablePhyto_x
  })

  phytomerNumber<-reactive ({
    input$phytomerNumber
  })




  observe({
    phyto <- phyto()
    if (is.null(phyto)) return(NULL)

    inputPhyto=phyto%>%
      select(contains('LEAF_NUMBER'))%>%
      filter(row_number()==1)%>%
      tidyr::gather('number',"LeafId")%>%
      arrange(LeafId)


    inputPhytoVar=data.frame(key=colnames(phyto))%>%
      separate(key,into = c('var','number'),sep = '#')%>%
      filter(number!=' NA')%>%
      mutate(number=sprintf("%03d",as.numeric(number)))


    updateSelectInput(session = session, inputId = 'phytomerNumber', choices = sort(unique(inputPhyto$LeafId)))
    updateSelectInput(session = session, inputId = 'variablePhyto_x', choices = unique(inputPhytoVar$var)[unique(inputPhytoVar$var)!='LEAF_NUMBER'])
  })


  # graphics ----------------------------------------------------------------

  output$graph1<-renderPlotly({
    plant<-plant()
    if (is.null(plant)) return(NULL)

    y_var<- y_var()
    if (is.null(y_var)) return(NULL)


    x_var<- x_var()
    if (is.null(x_var)) return(NULL)

    second_y_var<-  second_y_var()
    if (is.null(second_y_var)) return(NULL)



    gr1=NULL


    if (!is.null(x_var) & !is.null(y_var)){

      # x_var='Date'
      # y_var='ASSIM'
      # second_y_var='LAI'

      xlab=ifelse(varDef[varDef$variable==x_var,unit]=='',varDef[varDef$variable==x_var,variable],paste0(varDef[varDef$variable==x_var,variable], " (",varDef[varDef$variable==x_var,unit],")"))
      ylab=ifelse(varDef[varDef$variable==y_var,unit]=='',varDef[varDef$variable==y_var,variable],paste0(varDef[varDef$variable==y_var,variable], " (",varDef[varDef$variable==y_var,unit],")"))

      gr1=plant%>%
        # arrange(Date)%>%
        ggplot(aes(x=get(x_var),y=get(y_var),label=get(second_y_var)))+
        geom_line()+
        geom_point()+
        labs(x=xlab,
             y=ylab)


    }


    if(!is.null(gr1)){
      ggplotly(gr1)
    }

  })

  output$graph2<-renderPlotly({
    plant<-plant()
    if (is.null(res)) return(NULL)


    x_var<- x_var()
    if (is.null(x_var)) return(NULL)

    y_var<- y_var()
    if (is.null(y_var)) return(NULL)


    second_y_var<-  second_y_var()
    if (is.null(second_y_var)) return(NULL)


    gr2=NULL



    if (!is.null(x_var) & !is.null(second_y_var)){

      xlab=ifelse(varDef[varDef$variable==x_var,unit]=='',varDef[varDef$variable==x_var,variable],paste0(varDef[varDef$variable==x_var,variable], " (",varDef[varDef$variable==x_var,unit],")"))
      secondlab=ifelse(varDef[varDef$variable==second_y_var,unit]=='',varDef[varDef$variable==second_y_var,variable],paste0(varDef[varDef$variable==second_y_var,variable], " (",varDef[varDef$variable==second_y_var,unit],")"))

      gr2=plant%>%
        ggplot(aes(x=get(x_var),y=get(second_y_var),label=get(y_var)))+
        geom_line()+
        geom_point()+
        labs(x=xlab,
             y=secondlab)

    }


    if(!is.null(gr2)){
      ggplotly(gr2)
    }

  })


  # graph yield -------------------------------------------------------------



  output$graphYield<-renderPlotly({
    plant<-plant()
    if (is.null(plant)) return(NULL)

    grY=plant%>%
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
      geom_point(aes(x=my(month),y=yield_FFB,col='bunch weight',label=paste('age:',round(age,2))))+
      geom_line(aes(x=my(month),y=yield_FFB,col='bunch weight'))+
      geom_point(aes(x=my(month),y=yield_oil,col='Oil',label=paste('age:',round(age,2))))+
      geom_line(aes(x=my(month),y=yield_oil,col='Oil'))+
      labs(x='',
           y='kg.tree-1.month-1')+
      scale_color_discrete(name='')+
      xlim(range(plant$Date,na.rm=T))


    if(!is.null(grY)){
      ggplotly(grY)
    }

  })



  # graph phyto -------------------------------------------------------------


  output$graphPhyto<-renderPlotly({


    ### debug
    # phytomerNumber=-20
    # variablePhyto_x='FRUIT_NUMBER'
    phyto<-phyto()
    if (is.null(phyto)) return(NULL)

    phytomerNumber<- phytomerNumber()
    if (is.null(phytomerNumber)) return(NULL)


    variablePhyto_x<- variablePhyto_x()
    if (is.null(variablePhyto_x)) return(NULL)

    inputPhyto=phyto%>%
      select(contains('LEAF_NUMBER'))%>%
      filter(row_number()==1)%>%
      tidyr::gather('number',"LeafId")%>%
      arrange(LeafId)

    inputPhytoNumber=inputPhyto%>%
      filter(LeafId %in% phytomerNumber)%>%
      mutate(number=str_remove(string = number,'LEAF_NUMBER'))

    sub_phyto=phyto%>%
      select(Date,contains(variablePhyto_x))%>%
      select(Date,contains(inputPhytoNumber$number))%>%
      tidyr::gather('number',"value",-c(Date))%>%
      mutate(number=str_remove(string = number,pattern = variablePhyto_x))%>%
      filter(number %in% inputPhytoNumber$number)%>%
      arrange(Date,number)

    sub_state=phyto%>%
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


    lab=ifelse(varDef[varDef$variable==variablePhyto_x,unit]=='',varDef[varDef$variable==variablePhyto_x,variable],paste0(varDef[varDef$variable==variablePhyto_x,variable], " (",varDef[varDef$variable==variablePhyto_x,unit],")"))


    gr_Phyto=sub_phyto%>%
      mutate(num=paste('phyto #',sprintf( '%03d',as.numeric(LeafId))))%>%
      arrange(Date,number)%>%
      group_by(number)%>%
      ggplot(aes(x=Date,y=value,col=sex,pch=state,label=num))+
      geom_point(alpha=alpha)+
      geom_line(aes(group=num),alpha=alpha)+
      scale_color_viridis_d()+
      ylab(lab)+
      theme(legend.position='right')

    if(!is.null(gr_Phyto)){
      ggplotly(gr_Phyto)
    }

  })




}

shinyApp(ui,server)

