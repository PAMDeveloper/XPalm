#Script estimating parameters for EcoMeristem model
#Authors : Florian Larue, Gregory Beurier, Lauriane Rouan, Delphine Luquet
#-- (PAM, AGAP, BIOS, CIRAD)

###SET INFORMATION FOR ESTIMATION###
path <- "D:/Workspace/estimworkspace/2016_tallage/G26_LD"
vName <- "vobs_moy.txt"
vETName <- "vobs_et.txt"
paramOfInterest <- c("Epsib", "Ict","MGR_init","plasto_init","phyllo_init","ligulo_init",
                     "coef_MGR_PI","slope_length_IN","density_IN2","coef_plasto_PI",
                     "coef_phyllo_PI","coef_ligulo_PI","slope_LL_BL_at_PI", "coeff_in_diam")
minValue <- c(3, 0.5, 6, 25, 25, 25, -0.5, 0.5, 0.08, 1, 1, 1, 0.0, 0.8)
maxValue <- c(8, 2.5, 14, 45, 45, 45, 0.5, 1, 0.3, 3.0, 3.0, 3.0, 0.4, 1.0)

#paramOfInterest <- c("Epsib","Ict","plasto_init","phyllo_init","ligulo_init",
#                     "density_IN2","coef_plasto_PI",
#                     "coef_phyllo_PI","coef_ligulo_PI","coeff_in_diam")
#minValue <- c(3, 0.5, 20, 20, 20, 0.08, 1, 1, 1, 0.8)
#maxValue <- c(8, 2.5, 45, 45, 45, 0.3, 3.0, 3.0, 3.0, 1.0)

coefIncrease <- 10
maxIter <- 20000
penalty <- 10 #Penalty for simulation outside of SD (RMSE * Penalty)
solTol <- 0.01 #will be multiplied by the number of observed variables
relTol <- 0.001 #estimation stops if unable to reduce RMSE by (reltol * rmse) after steptol steps
stepTol <- 10000 #see above
clusterA <- TRUE  #parallel for machines with at least 4 cores

###START INSTALL AND PACKAGES LOAD###
list.of.packages <- c("Rcpp","recomeristem","parallel","DEoptim")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
invisible(lapply(list.of.packages, library, character.only=TRUE))

###INIT SIMULATIONS###
setwd(path)
meteo <- recomeristem::getMeteo_from_files(path)
param <- recomeristem::getParameters_from_files(path)
obs <- recomeristem::get_clean_obs(paste(path,"/",vName,sep=""))
obsET <- recomeristem::get_clean_obs(paste(path,"/",vETName,sep=""))

###INIT PARAMFILE FOR SAVING###
paramInit <- recomeristem::getParameters_from_files(path)
ParamList <- as.character(paramInit$Name)
paramInitTrans <- as.data.frame(matrix(ncol=length(ParamList)))
colnames(paramInitTrans) <- ParamList
paramInitTrans[1,] <- paramInit$Values

###INIT ESTIMATION###
isInit <- FALSE
bounds <- matrix(c(minValue,maxValue),ncol=2)
obsCoef <- rep(1,ncol(obs))
nbParam <- length(paramOfInterest)
result <- list()
VarList <- names(obs)

###FUNCTIONS###
optimEcomeristem <- function(p) {
  if(isInit == FALSE) {
    recomeristem::init_simu(param, meteo, obs, "env1")
    isInit <<- TRUE
  }
  if("phyllo_init" %in% paramOfInterest && "plasto_init" %in% paramOfInterest && "ligulo_init" %in% paramOfInterest) {
    if(p[match("phyllo_init",paramOfInterest)] < p[match("plasto_init",paramOfInterest)]) {
      return(99999)
    } else if(p[match("ligulo_init",paramOfInterest)] < p[match("phyllo_init",paramOfInterest)]) {
      return(99999)
    } else if(p[match("phyllo_init",paramOfInterest)]*p[match("coef_phyllo_PI",paramOfInterest)] < p[match("plasto_init",paramOfInterest)]*p[match("coef_plasto_PI",paramOfInterest)]) {
      return(99999)
    } else if(p[match("ligulo_init",paramOfInterest)]*p[match("coef_ligulo_PI",paramOfInterest)] < p[match("phyllo_init",paramOfInterest)]*p[match("coef_phyllo_PI",paramOfInterest)]) {
      return(99999)
    }
  }
  res <- recomeristem::launch_simu("env1", paramOfInterest, p)

  diff1 <- abs(1-(data.table::between(res,obs-obsET,obs+obsET)))
  diff2 <- diff1 * penalty
  diff3 <-  replace(diff2, diff2 == 0,1)
  diff4 <- ((((obs - res)/obs)^2)*diff3)*coeff
  diff <- sum(sqrt((colSums(diff4, na.rm=T))/(colSums(!is.na(diff4)))),na.rm=T)

  return(diff)
}
optimisation <- function(Optimizer, maxIter, solTol, bounds) {
  if(clusterA && detectCores() >= 4) {
    resOptim <- DEoptim(optimEcomeristem, lower=bounds[,1], upper=bounds[,2], DEoptim.control(reltol=relTol,steptol=stepTol,VTR=solTol,itermax=maxIter,strategy=2,cluster=cl,packages=c("recomeristem"),parVar=c("meteo","obs", "paramOfInterest", "obsET","penalty","coeff","isInit","param")))
  } else {
    resOptim <- DEoptim(optimEcomeristem, lower=bounds[,1], upper=bounds[,2], DEoptim.control(reltol=relTol,steptol=stepTol,VTR=solTol,itermax=maxIter,strategy=2))
  }
  result$optimizer <- "Diffential Evolution Optimization"
  result$par <- resOptim$optim$bestmem
  result$value <- resOptim$optim$bestval
  result$iter <- resOptim$optim$iter
  return(list(result,resOptim))
}
coefCompute <- function(length,obsCoef) {
  tmp <- c()
  for(i in 0:(length-1)) {
    tmp <- c(tmp, (obsCoef+(i*(obsCoef/(100/coefIncrease)))))
  }
  return(data.frame(matrix(unlist(tmp),nrow=length,byrow=T)))
}

resPlot <- function() {
  p <- result$par
  recomeristem::init_simu(param, meteo, obs, "resP")
  res <<- recomeristem::launch_simu("resP", paramOfInterest, p)
  obsT <- data.frame(day = seq(1,max(obs$day,na.rm=T)))
  for(v in VarList) {
    if(v != "day") {
      obsT[,v] <- 1
    }
  }
  recomeristem::init_simu(param, meteo, obsT, "total_res")
  resT <<- recomeristem::launch_simu("total_res", paramOfInterest, p)

  plotF <- function(x) {
    plot(resT[[x]], type="l", pch=3, xlab="DAS", ylab=x,ylim=c(0,(max(max(resT[[x]], na.rm=T),max(obs[[x]]+obsET[[x]], na.rm=T)))*1.1), xlim=c(0,max(obs$day,na.rm=T)+5),xaxs="i", yaxs="i")
    points(obs$day, obs[[x]], type="p", col="red")
    if(!is.null(obsET[[x]])) {
      arrows(obs$day,obs[[x]]-obsET[[x]],obs$day,obs[[x]]+obsET[[x]], code=3, length=0.02, angle = 90)
    }
    diff <- ((obs[[x]] - res[[x]])/obs[[x]])^2
    return(sqrt((sum(diff, na.rm=T))/(sum(!is.na(diff)))))
  }
  sapply(VarList, plotF)
}

savePar <- function(name = Sys.Date()) {
  resPar <- matrix(as.vector(c(result$value, result$par)), ncol=length(paramOfInterest)+1)
  write.table(resPar, file=paste("par_",name,".csv"), sep=",", append=F, dec=".",col.names=c("RMSE",paramOfInterest),row.names = F)
}

savePlot <- function(name = Sys.Date()) {
  pdf(paste(name,"_T.pdf",sep=""))
  resPlot()
  dev.off()
}

saveParF <- function() {
  bestp <- as.vector(result$par)
  paramInitTrans[paramOfInterest] <- bestp
  parameters <- data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]), row.names=NULL)
  write.table(parameters, "ECOMERISTEM_parameters.txt", sep="=", dec=".", quote=F, row.names=F, col.names=F)
}

###OPTIMISATION RUN###
set.seed(1337)
coeff <<- coefCompute(nrow(obs),obsCoef)
if(clusterA && detectCores() >= 4) {
  nbCores <- detectCores()
  cl <- makeCluster(nbCores, outfile="clusterlog.txt")
  clusterEvalQ(cl, library(recomeristem,rgenoud))
  clusterExport(cl, varlist = c("meteo","obs","param","paramOfInterest","obsET","penalty","coeff","isInit"))
} #parallel
time <- system.time(resOptim <- optimisation(Optimizer, maxIter, solTol, bounds))
result <- resOptim[[1]]
result$time <- time
resOptim <- resOptim[[2]]
print(paste("End of Estimation. Elapsed time :", time[[3]],"s"))
stopCluster(cl)
