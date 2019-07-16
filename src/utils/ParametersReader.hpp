#ifndef UTILS_PARAMETERS_READER_HPP
#define UTILS_PARAMETERS_READER_HPP

#include <ModelParameters.hpp>
#include <utils/juliancalculator.h>
#include <defines.hpp>

using namespace xpalm;
using namespace std;

#include <iostream>
#include <fstream>

template<typename Out>
void split(const std::string& s, char delim, Out result) {
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

//vector<string> manualSplit(const std::string &line, char delim) {
//    vector<string> strings;
//    istringstream f(line);
//    string s;
//    while (getline(f, s, delim)) {
//        cout << s << "\n";
//        strings.push_back(s);
//    }
//    return strings;
//}


inline std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


inline double round(double val, int decimal) {
    double factor = std::pow(10, decimal);
    val *= factor;
    val = std::round(val);
    return val / factor;
}

namespace utils {

class ParametersReader {
public:
    ParametersReader()
    { }

    virtual ~ParametersReader()
    { }

    void loadParametersFromFiles(const std::string& folder, ModelParameters& parameters) {
        std::ifstream varietyParams(folder + "/XPALM_parameters.txt");
        std::string line;

        while (std::getline(varietyParams, line)) {
            auto escape_pos = line.find("#");
            if(escape_pos != string::npos) {
                line = line.substr(0, escape_pos);
            }
//            std::cout << line << std::endl;

            auto pos = line.find("=");
            if(pos == string::npos)
//                std::cout << "=" << line << std::endl;
                continue;

            std::string s = line.substr(pos + 1, line.size());
            char* p;
            double converted = strtod(s.c_str(), &p);
//            if (*p) {
//                converted = JulianCalculator::toJulianDay(s, JulianCalculator::DMY, '/');
//            }
            parameters.set(line.substr(0, line.find("=") - 1), converted);
            std::cout << line.substr(0, line.find("=")) << converted << std::endl;
        }

        std::cout << "params done" << std::endl;

        std::ifstream meteo(folder + "/meteo.txt");

        std::string TMax, TMin, HMax, HMin, Vt, Rg, Rain;
        std::string day, month, year;
        bool first = true;
        while (meteo >> day) {
            meteo >> month >> year;
            if (first) {
                double julianBegin = JulianCalculator::getJulianDay(year, month, day);
                parameters.set("BeginDate", julianBegin);
                parameters.beginDate = julianBegin;
                first = false;
            }
            meteo >> TMax >> TMin >> HMax >> HMin >> Vt >> Rg >> Rain;
            parameters.meteoValues.push_back(
                    Climate(
                            round(std::stod(TMax), 9),
                            round(std::stod(TMin), 9),
                            round(std::stod(HMax), 9),
                            round(std::stod(HMin), 9),
                            round(std::stod(Vt), 9),
                            round(std::stod(Rg), 9),
                            round(std::stod(Rain), 9)
                    )
            );
        }
        parameters.set("EndDate", JulianCalculator::getJulianDay(year, month, day));
        meteo.close();
        varietyParams.close();
    }


#ifdef UNSAFE_RUN
    std::istream& safeGetline(std::istream& is, std::string& t) {
        t.clear();

        // The characters in the stream are read one-by-one using a std::streambuf.
        // That is faster than reading them one-by-one using the std::istream.
        // Code that uses streambuf this way must be guarded by a sentry object.
        // The sentry object performs various tasks,
        // such as thread synchronization and updating the stream state.

        std::istream::sentry se(is, true);
        std::streambuf* sb = is.rdbuf();

        for (;;) {
            int c = sb->sbumpc();
            switch (c) {
            case '\n':
                return is;
            case '\r':
                if (sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case std::streambuf::traits_type::eof():
                // Also handle the case when the last line has no line ending
                if (t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
            }
        }
    }

    map<string, vector<double>> loadCleanObsFromFile(const std::string& file_path, const SimpleView& view) {
        std::ifstream vObsFile(file_path);
        std::string line;
        std::getline(vObsFile, line); //headers
        vector<string> headers;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(headers));

        map<string, vector<double>> obs;
        for (string h : headers) {
            string* s = new string(h);
            transform(s->begin(), s->end(), s->begin(), ::tolower);
            if (view._selectors.find(*s) != view._selectors.end() || *s == "day")
                obs.insert(std::pair<string, vector<double>>(*s, vector<double>()));
            delete s;
        }

        while (getline(vObsFile, line)) {
            //std::cout << line << "\n";
            //line.erase (line.begin(), line.end()-2);

            vector<string> data = split(line, '\t');
            for (int i = 0; i < data.size(); ++i) {
                string s = data[i];
                string* h = new string(headers[i]);
                //std::cout << s << " " << s.size() << " " << data[i] << " " << data[i].size() << " " << *h << " " << headers[i] << "\n";

                transform(h->begin(), h->end(), h->begin(), ::tolower);
                if (view._selectors.find(*h) != view._selectors.end() || *h == "day") {
                    char* p;
                    double converted = strtod(s.c_str(), &p);
                    if (*p) {
                        //std::cout << "ADD NAN \n";
                        obs[*h].push_back(nan(""));
                    } else {
                        //std::cout << converted << "\n";
                        obs[*h].push_back(converted);
                    }
                }
                delete h;
            }
        }
        vObsFile.close();

        return obs;
    }
#endif

    map<string, vector<double>> loadVObsFromFile(const std::string& file_path) {
        std::ifstream vObsFile(file_path);
        std::string line;
        std::getline(vObsFile, line); //headers
        vector<string> headers;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(headers));

        map<string, vector<double>> obs;
        for (string h : headers) {
            string* s = new string(h);
            transform(s->begin(), s->end(), s->begin(), ::tolower);
            obs.insert(std::pair<string, vector<double>>(*s, vector<double>()));
            delete s;
        }

        while (std::getline(vObsFile, line)) {
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            vector<string> data = split(line, '\t');
            for (int i = 0; i < data.size(); ++i) {
                string s = data[i];
                string* h = new string(headers[i]);
                transform(h->begin(), h->end(), h->begin(), ::tolower);
                char* p;
                double converted = strtod(s.c_str(), &p);
                if (*p) {
                    obs[*h].push_back(nan(""));
                } else {
                    obs[*h].push_back(converted);
                }
                delete h;
            }
        }
        vObsFile.close();

        return obs;
    }


    //      void loadParametersFromProstgresql(const std::string &id,
    //            ModelParameters &parameters);

private:
    //        void load_meteo(PGconn* connection, ModelParameters &parameters);
    //      void load_data(PGconn* connection,
    //          const std::string &table,
    //          const std::string &id,
    //          const std::vector < std::string > &names,
    //            ModelParameters &parameters);

    //      void load_simulation(const std::string &id,
    //          PGconn* connection,
    //            ModelParameters &parameters);
};
} // namespace utils

#endif

