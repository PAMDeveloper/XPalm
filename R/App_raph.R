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




# meteo = data.table::fread('D:/PAMStudio/dev/git/XPalm/parameters/meteo.txt')
params = data.table::fread('D:/PAMStudio/dev/git/XPalm/parameters/XPalm_parameters.txt',header=F)%>%
  select(V1,V3)%>%
  mutate(V3=as.numeric(V3))%>%
  data.frame()

colnames(params)=c('Parameter','value')


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
                 tabPanel("Phytomer data",
                          tags$hr(),
                          # fluidRow(
                          #   column(4,
                          #          selectizeInput(inputId = 'x_var', label = 'Select X', choices =  '*', multiple = F)),
                          #   column(4,
                          #          selectizeInput(inputId = 'y_var', label = 'Select Y', choices =  '*', multiple = F)),
                          #   column(4,
                          #          selectizeInput(inputId = 'second_var', label = 'Select label variable', choices =  '*', multiple = F)),
                          #   tags$hr(),
                          #   fluidRow(
                          #     column(12,
                          #            plotlyOutput('graph')
                          #     )
                          #   )
                          # )
                 ),
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
  
  res <- reactive({
    
    input$action
    
    result=data.table::fread(file =paste0('../../bin/msvc14/x64/results.csv'),header=T)
    
    
    
    
    return(result)
    
  })
  
  
  
  
  # visualisation-----------------------------------------------------
  output$graph1<-renderPlotly({
    res<-res()
    if (is.null(res)) return(NULL)
    
    y_var<- y_var()
    if (is.null(y_var)) return(NULL)
    
    
    x_var<- x_var()
    if (is.null(x_var)) return(NULL)
    
    
    gr1=NULL
    

    if (!is.null(x_var) & !is.null(y_var)){
      
      
      gr1=res%>%
        # arrange(Date)%>%
        ggplot(aes(x=get(x_var),y=get(y_var)))+
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
    
    second_var<-  second_var()
    if (is.null(second_var)) return(NULL)
    
    
    gr2=NULL
    
    
    
    if (!is.null(x_var) & !is.null(second_var)){
      
      
      gr2=res%>%
        # arrange(Date)%>%
        ggplot(aes(x=get(x_var),y=get(second_var)))+
        geom_line()+
        geom_point()+
      labs(x=paste(x_var),
           y=paste(second_var))

    }
    
    
    if(!is.null(gr2)){
      ggplotly(gr2)
    }
    
  })
  
  output$tableParam = renderDataTable({
    params
  }, options = list(lengthMenu = c(5, 30, 50,nrow(params)), pageLength = nrow(params)))
  
}


# Run the application
shinyApp(ui = ui, server = server)

