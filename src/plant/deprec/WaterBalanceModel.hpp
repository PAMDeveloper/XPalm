/**
 * @file xpalm/plant/thermal-time/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef WATER_BALANCE_MODEL_HPP
#define WATER_BALANCE_MODEL_HPP

#include <defines.hpp>

namespace model {

class WaterBalanceModel : public AtomicModel < WaterBalanceModel >
{
public:
    enum internals { /*CSTR, FCSTR, FCSTRA, FCSTRI, FCSTRL, FCSTRLLEN, */FTSW, DECREASED_FTSW/*, TRANSPIRATION, SWC, PSIB*/ };
//    enum externals { INTERC };


    WaterBalanceModel() {
        //    computed variables
//        Internal(CSTR, &WaterBalanceModel::_cstr);
//        Internal(FCSTR, &WaterBalanceModel::_fcstr);
//        Internal(FCSTRA, &WaterBalanceModel::_fcstrA);
//        Internal(FCSTRI, &WaterBalanceModel::_fcstrI);
//        Internal(FCSTRL, &WaterBalanceModel::_fcstrL);
//        Internal(FCSTRLLEN, &WaterBalanceModel::_fcstrLlen);
        Internal(FTSW, &WaterBalanceModel::_ftsw);
        Internal(DECREASED_FTSW, &WaterBalanceModel::_decreased_ftsw);
//        Internal(TRANSPIRATION, &WaterBalanceModel::_transpiration);
//        Internal(SWC, &WaterBalanceModel::_swc);
//        Internal(PSIB, &WaterBalanceModel::_psib);

//        External(INTERC, &WaterBalanceModel::_interc);
    }

    virtual ~WaterBalanceModel()
    {}


    void compute(double t, bool /* update */) {
        /** METEO **/
        double water = _parameters.get(t).Irrigation + _parameters.get(t).Rain;
//        double et0 = _parameters.get(t).Etp; // get ETO in TTModel



        //compute_decrease_FTSW
        double SEUIL_ORGANO = _parameters.get("SEUIL_ORGANO");
        _decreased_ftsw = _ftsw > SEUIL_ORGANO ? 1 : _ftsw / SEUIL_ORGANO;



//        //evap max
//        EvapMax = ( 1 - tree.ei ) * ET0 * GlobalVariables.KC;

//        //transp max
//        self.Transp_Max = self.tree.ei * ET0 * GlobalVariables.KC;

//        //pluie efficace
//        if (0.916 * pluie- 0.589) < 0 :
//            Pluie_au_sol = 0
//        else:
//            Pluie_au_sol =(0.916 * pluie - 0.589)

//        if (0.0713 * pluie - 0.735) < 0 :
//            Ecoul_stipe = 0
//        else:
//            Ecoul_stipe=(0.0713* pluie - 0.735)

//        self.pluie_efficace = Pluie_au_sol + Ecoul_stipe;

//        //BH aprs pluie
//        mem_qte_H2O_C1 = self.qte_H2O_C1;
//        if (self.qte_H2O_Vap + pluie_efficace) >= self.TailleVap:
//           self.qte_H2O_Vap = self.TailleVap
//           if (self.qte_H2O_C1moinsVap + ( pluie_efficace - self.TailleVap + mem_qte_H2O_Vap )) >= self.TailleC1moinsVap :

//               self.qte_H2O_C1moinsVap = self.TailleC1moinsVap
//               self.qte_H2O_C1 = self.qte_H2O_C1moinsVap + self.qte_H2O_Vap
//               if (self.qte_H2O_C2 + mem_qte_H2O_C1 + pluie_efficace - self.TailleC1) >= self.TailleC2:

//                   self.qte_H2O_C2 = self.TailleC2
//               else:

//                   self.qte_H2O_C2 += mem_qte_H2O_C1 + pluie_efficace - self.TailleC1
//           else:

//               self.qte_H2O_C1moinsVap +=  pluie_efficace - self.TailleVap + mem_qte_H2O_Vap
//               self.qte_H2O_C1 = self.qte_H2O_C1moinsVap + self.qte_H2O_Vap
//               #QuantH2OC2[i] = QuantH2OC2[i-1]
//       else:

//           self.qte_H2O_Vap += pluie_efficace
//           #QuantH2OC1moinsVap[i] = QuantH2OC1moinsVap[i-1]
//           self.qte_H2O_C1 = self.qte_H2O_Vap + self.qte_H2O_C1moinsVap
//           #QuantH2OC1[i] = QuantH2OC1moinsVap[i] + QuantH2OVap[i]
//           #QuantH2OC2[i] = QuantH2OC2[i-1]
//       self.qte_H2O_C = self.qte_H2O_C1moinsVap + self.qte_H2O_C2

//       //compute_BH_Racines_pluie
//         mem_qte_H2O_C1_Racines = self.qte_H2O_C1_Racines
//        mem_qte_H2O_Vap_Racines = self.qte_H2O_Vap_Racines
//        if (self.qte_H2O_Vap_Racines + pluie_efficace) >= self.tree.racines.TailleVap:

//            self.qte_H2O_Vap_Racines = self.tree.racines.TailleVap
//            if (self.qte_H2O_C1moinsVap_Racines + ( pluie_efficace - self.tree.racines.TailleVap + mem_qte_H2O_Vap_Racines )) >= self.tree.racines.TailleC1moinsVap:

//                self.qte_H2O_C1moinsVap_Racines = self.tree.racines.TailleC1moinsVap
//                self.qte_H2O_C1_Racines = self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_Vap_Racines
//                if (self.qte_H2O_C2_Racines + mem_qte_H2O_C1_Racines + pluie_efficace - self.tree.racines.TailleC1) >= self.tree.racines.TailleC2:

//                    self.qte_H2O_C2_Racines = self.tree.racines.TailleC2
//                else:


//                    self.qte_H2O_C2_Racines += mem_qte_H2O_C1_Racines  + pluie_efficace - self.tree.racines.TailleC1
//            else:

//                self.qte_H2O_C1moinsVap_Racines +=  pluie_efficace - self.tree.racines.TailleVap + mem_qte_H2O_Vap_Racines
//                self.qte_H2O_C1_Racines = self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_Vap_Racines

//        else:

//            self.qte_H2O_Vap_Racines +=  pluie_efficace
//            self.qte_H2O_C1_Racines = self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_Vap_Racines
//        self.qte_H2O_C_Racines =  self.qte_H2O_C1moinsVap_Racines + self.qte_H2O_C2_Racines




//        //parameters
//        _etp = _parameters.get(t).Etp;
//        _water_supply = 0;
//        if(_wbmodel == 1) {
//            //Waterbalance model
//            //FTSW
//            _ftsw = _swc / RU1;

//            //cstr
//            _cstr = (_ftsw < ThresTransp) ? std::max(1e-4, _ftsw * 1. / ThresTransp) : 1;

//            //fcstr
//            _fcstr = std::sqrt(_cstr);

//            //transpiration
//            _transpiration = std::min(_swc, (Kcpot * std::min(_etp, ETPmax) * _interc * _cstr) / Density);

//            //SWC
//            _swc = _swc - _transpiration + _water_supply;
//        } else {
//            _water_supply = _parameters.get(t).Irrigation;

//            //Field waterbalance model
//            _cstr = 1;
//            _transpiration = _swc;
//            _ftsw = 1;
//            if(_water_supply == 1) {
//                _stressdays = std::min(10.,_stressdays + 1);
//                _psib = (pot/10)*_stressdays;
//                _fcstrA = std::min(1.,std::max(0.,(((thresAssim+thresLER)+(stressBP2+stressBP))-_psib)/(((thresAssim+thresLER)+(stressBP2+stressBP))-(stressBP2+stressBP))));
//                _fcstrL = std::min(1.,std::max(0.,((thresLER+stressBP)-_psib)/((thresLER+stressBP)-stressBP)));
//                _fcstrI = std::min(1.,std::max(0.,((thresINER+stressBP)-_psib)/((thresINER+stressBP)-stressBP)));
//                _fcstrLlen = std::min(1.,std::max(0.,((thresLEN+stressBP)-_psib)/((thresLEN+stressBP)-stressBP)));
//            } else {
//                _fcstr = 1;
//                _fcstrA = 1;
//                _fcstrI = 1;
//                _fcstrL = 1;
//                _stressdays = 0;
//            }
//        }
    }


    void init(double /*t*/, const xpalm::ModelParameters& parameters) {
        _parameters = parameters;

//        //    paramaters variables
//        ThresTransp = parameters.get("thresTransp");
//        RU1 = parameters.get("RU1");
//        ETPmax = parameters.get("ETPmax");
//        Kcpot = parameters.get("Kcpot");
//        Density = parameters.get("density");
//        thresLER = parameters.get("thresLER");
//        thresINER = parameters.get("thresINER");
//        thresAssim = parameters.get("thresAssim");
//        thresLEN = parameters.get("thresLEN");
//        stressBP = parameters.get("stressBP");
//        stressBP2 = parameters.get("stressBP2");
//        pot = parameters.get("psib");

//        _wbmodel = parameters.get("wbmodel");
//        //    computed variables
//        _cstr = 1;
//        _fcstr = 1;
        _ftsw = 1;
        _decreased_ftsw = 1;
//        _swc = RU1;
//        _transpiration = 0;
//        _psib = 0;
//        _stressdays = 0;
//        _fcstrA = 1;
//        _fcstrL = 1;
//        _fcstrI = 1;
//        _fcstrLlen = 1;
    }

private:
    xpalm::ModelParameters _parameters;
//    // parameters
//    double ETPmax;
//    double Kcpot;
//    double Density;
//    double RU1;
//    double ThresTransp;
//    //Field WB
//    double thresLER;
//    double thresAssim;
//    double thresINER;
//    double thresLEN;
//    double pot;
//    double _wbmodel;
//    double stressBP;
//    double stressBP2;
//    //meteo
//    double _etp;
//    double _water_supply;

//    //    internals (computed)
//    double _transpiration;
    double _ftsw;
    double _decreased_ftsw;
//    double _swc;
//    double _cstr;
//    double _fcstr;
//    double _fcstrA;
//    double _fcstrL;
//    double _fcstrI;
//    double _fcstrLlen;
//    //Field WB
//    double _psib;
//    double _stressdays;

//    //  externals
//    double _interc;
};

} // namespace model
#endif
