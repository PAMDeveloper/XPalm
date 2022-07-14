#include <Rcpp.h>
#include <Rinternals.h>

#include "xpalm_types.hpp"

using namespace Rcpp;
using namespace std;

map<string,vector<string>> sim_headers;
map<string,vector<double>> sim_values;

DataFrame mapOfDVectorToDF(std::map<std::string, std::vector<double>> map) {
  Rcpp::CharacterVector names;
  List values(map.size());
  int idx = 0;
  for(auto const& it: map){
    names.push_back(it.first);
    NumericVector vec( it.second.begin(), it.second.end() );
    values[idx] = vec;
    idx++;
  }
  DataFrame df(values);
  df.attr("names") = names;
  return df;
}


DataFrame mapOfSVectorToDF(std::map<std::string, std::vector<string>> map) {
  Rcpp::CharacterVector names;
  List values(map.size());
  int idx = 0;
  for(auto const& it: map){
    names.push_back(it.first);
    CharacterVector vec( it.second.begin(), it.second.end() );
    values[idx] = vec;
    idx++;
  }
  DataFrame df(values);
  df.attr("names") = names;
  return df;
}


// map <string, vector <double > > mapFromDF(DataFrame list) {
//   map <string, vector <double > > map;
//   CharacterVector names = list.attr("names");
//   for (int i = 0; i < names.size(); ++i) {
//     std::vector<double> vec = as<std::vector<double> >(list[i]);
//     std::pair <std::string, std::vector<double> > token( Rcpp::as<string>(names(i)), vec );
//     map.insert( token );
//   }
//   return map;
// }



// [[Rcpp::export]]
Rcpp::List runFolder(Rcpp::String folder)
{
  GlobalParameters globalParameters;
  xpalm::ModelParameters parameters;

  utils::ParametersReader reader;

  reader.loadParametersFromFiles(folder, parameters);

  /** RUN SIMU **/
  XPalmContext context(parameters.get("BeginDate"), parameters.get("EndDate"));
  Tree * m = new Tree();
  XPalmSimulator simulator(m, globalParameters);
  simulator.attachView("plant", new observer::PlantView());
  simulator.attachView("phytomers", new observer::PhytomerView());
  simulator.init(parameters.get("BeginDate"), parameters);
  simulator.run(context);

  ResultParser parser;
  pair<map<string,vector<double>>,map<string,vector<string>>> results;
  results = parser.resultToMaps(simulator);
  sim_values = results.first;
  sim_headers = results.second;
  return mapOfDVectorToDF(sim_values);
}



// [[Rcpp::export]]
Rcpp::List runDF(Rcpp::List dfParameters, Rcpp::List dfMeteo)
{
  /** INIT PARAMS **/
  GlobalParameters globalParameters;
  xpalm::ModelParameters parameters;

  CharacterVector names = dfParameters[0];
  NumericVector values = dfParameters[1];
  for (int i = 0; i < names.size(); ++i) {
    std::cout << Rcpp::as<std::string>(names(i)) << std::endl;
    std::cout << values(i) << std::endl;
    std::string name = Rcpp::as<std::string>(names(i));
    name = name.substr(0, name.size()-1);
    parameters.getRawParameters()->insert(
        std::pair<std::string,double>(
            name,
            values(i)
        )
    );
  }

  map<string, double>::iterator it;
  for (it = parameters.getRawParameters()->begin(); it != parameters.getRawParameters()->end(); it++)
  {
    std::cout << it->first  // string (key)
              << ':'
              << it->second // string's value
              << std::endl;
  }

  IntegerVector Day = dfMeteo[0];
  IntegerVector Month = dfMeteo[1];
  IntegerVector Year = dfMeteo[2];
  NumericVector TMax = dfMeteo[3];
  NumericVector TMin = dfMeteo[4];
  NumericVector HMax = dfMeteo[5];
  NumericVector HMin = dfMeteo[6];
  NumericVector Vt = dfMeteo[7];
  NumericVector Rg = dfMeteo[8];
  NumericVector Rain = dfMeteo[9];


  for (int i = 0; i < TMax.size(); ++i) {
    xpalm::Climate c(TMax(i), TMin(i), HMax(i), HMin(i), Vt(i), Rg(i), Rain(i));
    parameters.meteoValues.push_back(c);
  }

  double beginDate = JulianCalculator::getJulianDay(JulianCalculator::dateInts(Year(0), Month(0), Day(0)));
  parameters.set("BeginDate", beginDate);
  parameters.beginDate = beginDate;

  int end_idx = Day.size()-1;
  double endDate = JulianCalculator::getJulianDay(JulianCalculator::dateInts(Year(end_idx), Month(end_idx), Day(end_idx)));
  parameters.set("EndDate", endDate);

    /** RUN SIMU **/
  XPalmContext context(parameters.get("BeginDate"), parameters.get("EndDate"));
  Tree * m = new Tree();
  XPalmSimulator simulator(m, globalParameters);
  simulator.attachView("plant", new observer::PlantView());
  simulator.attachView("phytomers", new observer::PhytomerView());
  simulator.init(parameters.get("BeginDate"), parameters);
  simulator.run(context);

  ResultParser parser;
  pair<map<string,vector<double>>,map<string,vector<string>>> results;
  results = parser.resultToMaps(simulator);
  sim_values = results.first;
  sim_headers = results.second;
  return mapOfDVectorToDF(sim_values);
}

// [[Rcpp::export]]
Rcpp::List getValues()
{
  return mapOfDVectorToDF(sim_values);
}


// [[Rcpp::export]]
Rcpp::List getHeaders()
{
  return mapOfSVectorToDF(sim_headers);
}
