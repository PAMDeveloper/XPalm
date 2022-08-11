library(XPalm)


# Run from folder
ResFromFolder = XPalm::runFolder("parameters")
HdrsFolder = XPalm::getHeaders()

# Run from dataframes
parameters = read.table("parameters/XPalm_parameters.txt", header=F, sep="=")
meteo = read.table("parameters/meteo.txt", header=F, sep="\t")

ResFromDF = XPalm::runDF(parameters, meteo)
Hdrs = XPalm::getHeaders()
Vals = XPalm::getValues()
