#ifndef RESULTPARSER_H
#define RESULTPARSER_H

#include <defines.hpp>

#include <observer/PlantView.hpp>
#include <utils/ParametersReader.hpp>


static const string _TIME_ = "_time_";

class ResultParser
{
 public:
    ResultParser() {

    }

    pair<map<string,vector<double>>,map<string,vector<string>>> resultToMaps(const XPalmSimulator &simulator) const
    {
        map<string, vector<double>> results;
        map<string, vector<string>> headers;

        vector<string> v;
        v.push_back(_TIME_);
        v.push_back(_TIME_);
        v.push_back(_TIME_);
        headers[_TIME_] = v;

        const Observer& observer = simulator.observer();
        const Observer::Views& views = observer.views();


        View::Values global_map;

        double begin = numeric_limits<double>::max();
        double end = numeric_limits<double>::lowest();

        Observer::Views::const_iterator it = views.begin();
        for (it; it != views.end(); ++it) {

            if(it->second->begin() < begin)
                begin = it->second->begin();

            if(it->second->end() > end)
                end = it->second->end();

            View::Values values = it->second->values();
            for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
                vector<string> v;
                v.push_back(it->second->name(itv->first));
                v.push_back(it->second->path(itv->first));
                v.push_back(itv->first);
                headers[itv->first] = v;
            }

            global_map.insert(values.begin(), values.end());

            results[_TIME_] = vector<double>();
            for (typename View::Values::const_iterator itv = global_map.begin(); itv != global_map.end(); ++itv) {
                results[itv->first] = vector<double>();
            }

            for (double t = begin; t <= end; ++t)
                results[_TIME_].push_back(t);

            // write values
            for (View::Values::const_iterator itv = global_map.begin(); itv != global_map.end(); ++itv) {
                View::Value::const_iterator itp = itv->second.begin();
                string s = itv->first;

                for (double t = begin; t <= end; ++t) {
                    while (itp != itv->second.end() and itp->first < t) {
                        ++itp;
                    }

                    if (itp != itv->second.end()) {
                        string c = itp->second;
                        char* p;
                        double converted = strtod(c.c_str(), &p);
                        if (*p) {
                            results[s].push_back(nan(""));
                        } else {
                            results[s].push_back(converted);
                        }
                    } else {
                        results[s].push_back(nan(""));
                    }
                }
            }
        }

        return make_pair(results, headers);

    }


    //1 seule view
    map<string, vector<double>>  resultsToMap(XPalmSimulator * simulator) {
        map<string, vector<double>> result;
        const Observer& observer = simulator->observer();
        const Observer::Views& views = observer.views();
        Observer::Views::const_iterator it = views.begin();


        View::Values values = it->second->values();
        double begin = it->second->begin();
        double end = it->second->end();

        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
            string s = itv->first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            result.insert(std::pair<string,vector<double> >(s, vector<double>()) );
        }

        // write values
        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
            View::Value::const_iterator itp = itv->second.begin();
            string s = itv->first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);

            for (double t = begin; t <= end; ++t) {
                while (itp != itv->second.end() and itp->first < t) {
                    ++itp;
                }

                if (itp != itv->second.end()) {
#ifdef UNSAFE_RUN
                    double converted = itp->second;
                    result[s].push_back(converted);
#else
                    string c = itp->second;
                    char* p;
                    double converted = strtod(c.c_str(), &p);
                    if (*p) {
                        result[s].push_back(nan(""));
                    } else {
                        result[s].push_back(converted);
                    }
#endif
                } else {
                    result[s].push_back(nan(""));
                }
            }
        }
        return result;
    }

    map<string, vector<double>> filterVObs( map<string, vector<double>> vObs,
                                            map<string, vector<double> > results,
                                            bool keepDay = true,
                                            map<string, double> constraints = map<string,double>(),
                                            string dayId = "day")
    {
        double dayMax = results.begin()->second.size();
        map<string, vector<double>> filteredVObs;
        for(auto const &token : vObs) {
            string * s = new string(token.first);
            transform(s->begin(), s->end(), s->begin(), ::tolower);
            if(results.find(*s) != results.end() || (keepDay && *s == dayId)) {
                filteredVObs.insert( pair<string,vector<double> >(*s, vector<double>()) );
            }
            delete s;
        }

        for (int i = 0; i < vObs[dayId].size(); ++i) {
            bool valid = true;

            valid &= vObs[dayId][i] <= dayMax;
            if(valid){
                for(auto const &token : filteredVObs)  {
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    if(constraints.find(*s) != constraints.end()) {
                        valid &= vObs[*s][i] == constraints[*s];
                    }
                    delete s;
                }
            }

            if(valid){
                for(auto token : filteredVObs) {
                    string * h = new string(token.first);
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    filteredVObs[*s].push_back(vObs[*h][i]);
                    delete s;
                    delete h;
                }
            }
        }

        return filteredVObs;
    }


    map<string, vector<double>> reduceResults(map<string, vector<double> > results,
                                              map<string, vector<double> > vObs,
                                              map<string, double> constraints = map<string,double>(),
                                              string dayId = "day") {

        map<string, vector<double>> filteredVObs = filterVObs(vObs, results, true, constraints, dayId);

        map<string, vector<double>> reducedResults;
        for(auto const &token : filteredVObs) {
            string s = token.first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            if(results.find(s) != results.end()) {
                reducedResults.insert( pair<string,vector<double> >(s, vector<double>()) );
            }
        }


        for(auto const &r : reducedResults) {
            for (int i = 0; i < filteredVObs[dayId].size(); ++i) {
                int day = filteredVObs[dayId][i];
                if(day <= results[r.first].size())
                    reducedResults[r.first].push_back(results[r.first][day-1]);
            }
        }

        return reducedResults;
    }
};

#endif // RESULTPARSER_H
