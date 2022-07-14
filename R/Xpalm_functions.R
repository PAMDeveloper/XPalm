###list of functions used in Xpalm###
##R PEREZ, January 2020

#' Calculation of leaf area over organ growth
#'
#' @param TTleaf themal time since phytomer appearance
#' @param SFMax maximal leaf area
#' @param INFLEXION inflexion point
#' @param COURBURE slope at inflexion
#'
#' @return
#' @export
#'
#' @examples
Leafarea_expand=function(TTleaf,SFMax,INFLEXION,COURBURE){
  y=SFMax/(1+exp(-(TTleaf-INFLEXION)/COURBURE))
  return(y)
}


derive_Leafarea_expand=function(TTleaf,SFMax,INFLEXION,COURBURE){
  y=SFMax/COURBURE*exp(-(TTleaf-INFLEXION)/COURBURE)/((1+exp(-(TTleaf-INFLEXION)/COURBURE))**2)
  return(y)
}



#' variable value at a given age (ontogenetic gradient)
#'
#' @param ageplant age
#' @param age_ini age at planting
#' @param age_fin adult age
#' @param val_ini value of the variable at planting
#' @param val_fin  value of the variable at adult age
#'
#' @return 
#' @export
#'
#' @examples
age_relative_var=function(age, age_ini, age_fin, val_ini,val_fin) {
  
  if ( age > age_fin ) {
    y= val_fin
  } else if (age < age_ini ) {
    y=val_ini
  } else {
    age_relative = age - age_ini;
    inc = (val_fin - val_ini) / (age_fin - age_ini);
    y=val_ini + age_relative * inc
  }
  
  return (y)
}