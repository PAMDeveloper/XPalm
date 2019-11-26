#ifndef METEO_H
#define METEO_H
#include <defines.hpp>

namespace model {

class Meteo : public AtomicModel < Meteo >
{
public:
    enum internals { TT,
                     LAT_RAD,
                     TMAX,
                     TMIN,
                     RHMAX,
                     RHMIN,
                     WINDSPEED,
                     RG,
                     RAIN,
                     TEFF,
                     ET0 };

private:
    xpalm::ModelParameters _parameters;
    //      parameters
    double LATITUDE;
    double ALTITUDE;
//    double IRRIGATION_DAY;
//    double IRRIGATION_QUANTITY;
    double TBase;
    double TOpt1;
    double TOpt2;
    double TLim;


    //     internals
    double tt;
    double lat_rad;
    double Tmax;
    double Tmin;
    double RHmax;
    double RHmin;
    double windspeed;
    double Rg;
    double Rain;
    double TEff;
    double Et0;

public:

    Meteo()
    {
        //         internals
        Internal(TT, &Meteo::tt);
        Internal(LAT_RAD, &Meteo::lat_rad);
        Internal(TMAX, &Meteo::Tmax);
        Internal(TMIN, &Meteo::Tmin);
        Internal(RHMAX, &Meteo::RHmax);
        Internal(RHMIN, &Meteo::RHmin);
        Internal(WINDSPEED, &Meteo::windspeed);
        Internal(RG, &Meteo::Rg);
        Internal(RAIN, &Meteo::Rain);
        Internal(TEFF, &Meteo::TEff);
        Internal(ET0, &Meteo::Et0);
    }

    virtual ~Meteo()
    {
    }


    void computeTEff() {
        double V, W, S2, m1, m2;
        if (Tmin >= Tmax) {
            if (Tmin > TOpt1)
                TEff = TOpt1 - TBase;
            else
                TEff = Tmin - TBase;
        } else {
            if (Tmin < TOpt1)
                V = ((min(TOpt1, Tmax) + Tmin) / 2 - TBase) / (TOpt1 - TBase);
            else
                V = 0;
            if (Tmax > TOpt2)
                W = (TLim - (Tmax + max(TOpt2, Tmin)) / 2) / (TLim - TOpt2);
            else
                W = 0;
            if (Tmax < TOpt1)
                S2 = 0;
            else {
                if (Tmax < TOpt2)
                    S2 = Tmax - max(TOpt1, Tmin);
                else {
                    if (Tmin > TOpt2)
                        S2 = 0;
                    else
                        S2= TOpt2 - max(TOpt1, Tmin);
                }
            }
            m1 = V * (min(TOpt1, Tmax) - Tmin);
            m2 = W * (Tmax - max(Tmin, TOpt2));
            if (Tmax <= TBase)
                TEff = 0;
            else {
                if (Tmin >= TLim)
                    TEff = 0;
                else
                    TEff = ((m1 + m2 + S2) / (Tmax - Tmin)) * (TOpt1 - TBase);
            }
            if (TEff < 0)
                TEff = 0;
        }
    }

    void computeTT() {
        tt += TEff;
    }

    void computeET0(double t) {
        double tDay = JulianCalculator::dayNumber(t);
        double TMoy = (Tmax + Tmin) / 2;
        double HMoy = (RHmin + RHmax) / 2;

        if (Tmin > Tmax)
            Tmax = Tmin;
        if (RHmin > RHmax)
            RHmax = RHmin;

        double Decli = 0.409*sin(0.0172 * tDay-1.39);
        double SunPos = acos( - tan( lat_rad ) * tan(Decli));
        double Sundist = 1 + 0.033*cos( 2 * (_PI/365) * tDay );
        double Ray_extra = 24 * 60 * 0.0820 / _PI * Sundist * (SunPos * sin(Decli) * sin(lat_rad) + cos(Decli)* cos(lat_rad) * sin(SunPos) );
        double RGMax = ( 0.75 + 0.00002 * ALTITUDE ) *   Ray_extra;
        double day_length = 7.64 * SunPos;
        double PAR = 0.48 * Rg;
        double esat = 0.3054 * (exp(17.24 * Tmax/ ( Tmax + 237.3)) + exp(17.27 * Tmin/(Tmin + 237.3)));
        double ea = 0.3054 * (exp(17.27 * Tmax/(Tmax + 237.3))* RHmin / 100 + exp(17.27 * Tmin / (Tmin + 237.3))* RHmax/100);
        double VPD = esat - ea;
        double ratioRg = (Rg > RGMax) ? 1 : Rg /  RGMax;
        double Rn = 0.77 * Rg - (1.35 * ratioRg - 0.35) *
                (0.34 - 0.14 * std::pow(ea, 0.5)) *
                (pow(Tmax + 273.16, 4) + std::pow(Tmin + 273.16, 4)) * 2.45015 * std::pow(10, -9);
        double Tlat =  2.501 - 2.361 * std::pow(10, -3) * TMoy;
        double pent_vap_sat = 4098 * (0.6108*exp(17.27 * TMoy /(TMoy +237.3)))/pow((TMoy +237.3),2);
        double Kpsy = 0.00163 * 101.3 * pow((1-(0.0065*ALTITUDE/293)),5.26);     //TODO BUG
        //should be
        //            KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude * 1.0 / 293), 5.26) / TLat;

        double erad = 0.408 * Rn*pent_vap_sat/(pent_vap_sat+Kpsy*(1+0.34*windspeed));
        double eaero = ((900/(TMoy +273.16))*(( esat - ea)* windspeed)*Kpsy)/(pent_vap_sat+Kpsy*(1+0.34*windspeed));
        Et0 = erad + eaero;
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;
        _parameters = parameters;

        //        parameters
        LATITUDE = parameters.get("LATITUDE");
        ALTITUDE = parameters.get("ALTITUDE");
//        IRRIGATION_DAY = parameters.get("IRRIGATION_DAY");
//        IRRIGATION_QUANTITY = parameters.get("IRRIGATION_QUANTITY");
        TBase = parameters.get("T_BASE");
        TOpt1 = parameters.get("T_OPT1");
        TOpt2 = parameters.get("T_OPT2");
        TLim = parameters.get("T_LIM");

        //       internals
        tt = Tmax = Tmin = RHmax = RHmin = windspeed = Rg = Rain = TEff = Et0 = 0;
        lat_rad = LATITUDE  * _PI/180;
    }

    void compute(double t, bool /* update */)
    {
        Tmax = _parameters.get(t).TMax;
        Tmin = _parameters.get(t).TMin;
        RHmax = _parameters.get(t).HMax;
        RHmin = _parameters.get(t).HMin;
        windspeed = _parameters.get(t).Vt;
        Rg = _parameters.get(t).Rg;
        Rain = _parameters.get(t).Rain;

        computeTEff();
        computeTT();
        computeET0(t);
    }

};
} //namespace model


//void Samara::eval_Par(double t) {
//    try {
//        double tDay = JulianCalculator::dayNumber(t);
//        Decli = 0.409 * sin(0.0172 * tDay - 1.39);
//        SunPosi = acos(-tan(LatRad) * tan(Decli));
//        DayLength = 7.64 * SunPosi;
//        SunDistance = 1 + 0.033 * cos(2 * M_PI * 1.0 / 365 * tDay);
//        RayExtra = 24 * 60 * 0.0820 * 1.0 / M_PI * SunDistance *
//                (SunPosi * sin(Decli) * sin(LatRad) +
//                 cos(Decli) * cos(LatRad) * sin(SunPosi));
//        RgMax = (0.75 + 0.00002 * Altitude) * RayExtra;

//        if ((Rg == NilValue)) {
//            RgCalc = (0.25 + 0.50 * std::min(Ins * 1.0 / DayLength, 1.)) * RayExtra;
//        } else {
//            RgCalc = Rg;
//        }

//        Par = KPar * RgCalc;
//    } catch (...) {
//        error_message("EvalRg", UMeteo);
//    }
//}


//void Samara::EToFao_2_3(double const &/*ETP*/, double const &/*Alt*/, double const &RgMax, double const &RgCalc,
//                        double const &TMin, double const &TMax,
//                        double const &HMin, double const &HMax, double const &HMoyCalc,
//                        double const &TMoyCalc, double const &Vt, double &ETo,
//                        double &TMoyPrec, double &VPDCalc) {
//    try {
//        if(Vt == 0) {
//            VPDCalc = VPDCalc;
//        }
//        if ((ETP == NilValue)) {
//            double eActual; double eSat; double RgRgMax; double TLat; double delta; double KPsy; double Eaero; double Erad; double Rn; double G;
//            eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) +
//                             exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
//            if ((HMax == NilValue))
//                eActual = eSat * HMoyCalc * 1.0 / 100;
//            else
//                eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) *
//                                    HMin * 1.0 / 100 + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) *
//                                    HMax * 1.0 / 100);

//            VPDCalc = eSat - eActual;
//            RgRgMax = RgCalc * 1.0 / RgMax;
//            if ((RgRgMax > 1))
//                RgRgMax = 1;
//            Rn = 0.77 * RgCalc - (1.35 * RgRgMax - 0.35) *
//                    (0.34 - 0.14 * std::pow(eActual, 0.5)) *
//                    (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10, -9);

//            // chaleur latente de vaporisation de l'eau
//            TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoyCalc;
//            //  pente de la courbe de pression de vapeur saturante en kPa/°C
//            delta = 4098 * (0.6108 * exp(17.27 * TMoyCalc * 1.0 / (TMoyCalc + 237.3))) * 1.0 / std::pow(TMoyCalc
//                                                                                                        + 237.3, 2);
//            // constante psychrométrique en kPa/°C
//            KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude * 1.0 / 293), 5.26) * 1.0 / TLat;
//            // Radiative
//            G = 0.38 * (TMoyCalc - TMoyPrec);
//            Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vt));
//            // Partie évaporative de ET0 = Eaéro
//            Eaero = (900 * 1.0 / (TMoyCalc + 273.16)) * ((eSat - eActual) * Vt) * KPsy * 1.0 /
//                    (delta + KPsy * (1 + 0.34 * Vt));
//            ETo = Erad + Eaero;
//        } else {
//            ETo = ETP;
//        }
//        TMoyPrec = TMoyCalc;

//    } catch (...) {
//        error_message("EToFAO", UMeteo);
//    }
//}


//void Samara::EToFao(double const &/*ETP*/, double const &/*Alt*/, double const &RgMax, double const &RgCalc,
//                    double const &TMin, double const &TMax,
//                    double const &HMin, double const &HMax, double const &HMoyCalc,
//                    double const &TMoyCalc, double const &Vt, double &ETo,
//                    double &TMoyPrec, double &VPDCalc) {
//    try {
//        if(Vt == 0) {
//            VPDCalc = VPDCalc;
//        }
//        if ((ETP == NilValue)) {
//            double eActual; double eSat; double RgRgMax; double TLat; double delta; double KPsy; double Eaero; double Erad; double Rn; double G;
//            eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) +
//                             exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
//            if ((HMax == NilValue))
//                eActual = eSat * HMoyCalc * 1.0 / 100;
//            else
//                eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) *
//                                    HMin * 1.0 / 100 + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) *
//                                    HMax * 1.0 / 100);

//            VPDCalc = eSat - eActual;
//            RgRgMax = RgCalc * 1.0 / RgMax;
//            if ((RgRgMax > 1))
//                RgRgMax = 1;
//            Rn = 0.77 * RgCalc - (1.35 * RgRgMax - 0.35) *
//                    (0.34 - 0.14 * std::pow(eActual, 0.5)) *
//                    (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10, -9);

//            // chaleur latente de vaporisation de l'eau
//            TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoyCalc;
//            //  pente de la courbe de pression de vapeur saturante en kPa/°C
//            delta = 4098 * (0.6108 * exp(17.27 * TMoyCalc * 1.0 / (TMoyCalc + 237.3))) * 1.0 / std::pow(TMoyCalc
//                                                                                                        + 237.3, 2);
//            // constante psychrométrique en kPa/°C
//            KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude * 1.0 / 293), 5.26) * 1.0 / TLat;
//            // Radiative
//            G = 0.38 * (TMoyCalc - TMoyPrec);
//            Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vt));
//            // Partie évaporative de ET0 = Eaéro
//            Eaero = (900 * 1.0 / (TMoyCalc + 273.16)) * ((eSat - eActual) * Vt) * KPsy * 1.0 /
//                    (delta + KPsy * (1 + 0.34 * Vt));
//            ETo = Erad + Eaero;
//        } else {
//            ETo = ETP;
//        }
//        TMoyPrec = TMoyCalc;

//    } catch (...) {
//        error_message("EToFAO", UMeteo);
//    }
//}
#endif // METEO_H
