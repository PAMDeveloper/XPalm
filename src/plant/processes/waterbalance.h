#ifndef WATERBALANCE_H
#define WATERBALANCE_H

/*
 * Copyright (C) 2019-2020 Cirad http://www.cirad.fr
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

/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>

namespace model {

class WaterBalance : public AtomicModel < WaterBalance >
{
public:
    enum internals { A_C1,
                     A_C2,
                     A_Vap,
                     A_C1moinsV,
                     A_C,
                     EVAP,
                     EVAPC1MOINSVAP,
                     EVAPMAX,
                     EVAPVAP,
                     FTSW,
                     FRACTIONC,
                     FRACTIONC1,
                     FRACTIONC1RACINE,
                     FRACTIONC1MOINSVAPRACINE,
                     FRACTIONC2,
                     FRACTIONC2RACINE,
                     TAILLEC,
                     TAILLEC1,
                     TAILLEC1MOINSVAP,
                     TAILLEC2,
                     TAILLEVAP,
                     TRANSP_MAX,
                     TRANSPI,
                     TRANSPIC1MOINSVAP,
                     TRANSPIC2,
                     PLUIE_EFFICACE,
                     QTE_H2O_C,
                     QTE_H2O_C1,
                     QTE_H2O_C1_RACINES,
                     QTE_H2O_C1MOINSVAP,
                     QTE_H2O_C1MOINSVAP_RACINES,
                     QTE_H2O_C2,
                     QTE_H2O_C2_RACINES,
                     QTE_H2O_C_RACINES,
                     QTE_H2O_VAP,
                     QTE_H2O_VAP_RACINES };

    enum externals { RACINES_TAILLEC,
                     RACINES_TAILLEC1,
                     RACINES_TAILLEC2,
                     RACINES_TAILLEC1MOINSVAP,
                     RACINES_TAILLEVAP,
                     ET0,
                     TREE_EI };

private:
    xpalm::ModelParameters _parameters;
//      parameters
    double H_CR;
    double H_PF_Z1;
    double Z1;
    double H_PF;
    double Z2;
    double H_0;
    double KC;
    double SEUIL_EVAP;
    double SEUIL_TRANSPI;

//     internals
    double a_C1;
    double a_C2;
    double a_C;
    double a_vap;
    double a_C1moinsV;
    double a_C1moinsVap_Racines;
    double a_Vap_Racines;
    double Evap;
    double EvapC1moinsVap;
    double EvapMax;
    double EvapVap;
    double ftsw;
    double FractionC;
    double FractionC1;
    double FractionC1Racine;
    double FractionC1moinsVapRacine;
    double FractionC2;
    double FractionC2Racine;
    double TailleC;
    double TailleC1;
    double TailleC1moinsVap;
    double TailleC2;
    double TailleVap;
    double Transp_Max;
    double Transpi;
    double TranspiC1moinsVap;
    double TranspiC2;
    double pluie_efficace;
    double qte_H2O_C;
    double qte_H2O_C1;
    double qte_H2O_C1_Racines;
    double qte_H2O_C1moinsVap;
    double qte_H2O_C1moinsVap_Racines;
    double qte_H2O_C2;
    double qte_H2O_C2_Racines;
    double qte_H2O_C_Racines;
    double qte_H2O_Vap;
    double qte_H2O_Vap_Racines;

//     externals
    double racines_TailleC;
    double racines_TailleC1;
    double racines_TailleC2;
    double racines_TailleC1moinsVap;
    double racines_TailleVap;
    double Et0;
    double tree_ei;

public:

    WaterBalance()
    {
//         internals
        Internal(A_C1, &WaterBalance::a_C1);
        Internal(A_C2, &WaterBalance::a_C2);
        Internal(A_Vap, &WaterBalance::a_vap);
        Internal(A_C, &WaterBalance::a_C);
        Internal(A_C1moinsV, &WaterBalance::a_C1moinsV);
       Internal(EVAP, &WaterBalance::Evap);
        Internal(EVAPC1MOINSVAP, &WaterBalance::EvapC1moinsVap);
        Internal(EVAPMAX, &WaterBalance::EvapMax);
        Internal(EVAPVAP, &WaterBalance::EvapVap);
        Internal(FTSW, &WaterBalance::ftsw);
        Internal(FRACTIONC, &WaterBalance::FractionC);
        Internal(FRACTIONC1, &WaterBalance::FractionC1);
        Internal(FRACTIONC1RACINE, &WaterBalance::FractionC1Racine);
        Internal(FRACTIONC1MOINSVAPRACINE, &WaterBalance::FractionC1moinsVapRacine);
        Internal(FRACTIONC2, &WaterBalance::FractionC2);
        Internal(FRACTIONC2RACINE, &WaterBalance::FractionC2Racine);
        Internal(TAILLEC, &WaterBalance::TailleC);
        Internal(TAILLEC1, &WaterBalance::TailleC1);
        Internal(TAILLEC1MOINSVAP, &WaterBalance::TailleC1moinsVap);
        Internal(TAILLEC2, &WaterBalance::TailleC2);
        Internal(TAILLEVAP, &WaterBalance::TailleVap);
        Internal(TRANSP_MAX, &WaterBalance::Transp_Max);
        Internal(TRANSPI, &WaterBalance::Transpi);
        Internal(TRANSPIC1MOINSVAP, &WaterBalance::TranspiC1moinsVap);
        Internal(TRANSPIC2, &WaterBalance::TranspiC2);
        Internal(PLUIE_EFFICACE, &WaterBalance::pluie_efficace);
        Internal(QTE_H2O_C, &WaterBalance::qte_H2O_C);
        Internal(QTE_H2O_C1, &WaterBalance::qte_H2O_C1);
        Internal(QTE_H2O_C1_RACINES, &WaterBalance::qte_H2O_C1_Racines);
        Internal(QTE_H2O_C1MOINSVAP, &WaterBalance::qte_H2O_C1moinsVap);
        Internal(QTE_H2O_C1MOINSVAP_RACINES, &WaterBalance::qte_H2O_C1moinsVap_Racines);
        Internal(QTE_H2O_C2, &WaterBalance::qte_H2O_C2);
        Internal(QTE_H2O_C2_RACINES, &WaterBalance::qte_H2O_C2_Racines);
        Internal(QTE_H2O_C_RACINES, &WaterBalance::qte_H2O_C_Racines);
        Internal(QTE_H2O_VAP, &WaterBalance::qte_H2O_Vap);
        Internal(QTE_H2O_VAP_RACINES, &WaterBalance::qte_H2O_Vap_Racines);

//          externals
        External(RACINES_TAILLEC, &WaterBalance::racines_TailleC);
        External(RACINES_TAILLEC1, &WaterBalance::racines_TailleC1);
        External(RACINES_TAILLEC2, &WaterBalance::racines_TailleC2);
        External(RACINES_TAILLEC1MOINSVAP, &WaterBalance::racines_TailleC1moinsVap);
        External(RACINES_TAILLEVAP, &WaterBalance::racines_TailleVap);
        External(ET0, &WaterBalance::Et0);
        External(TREE_EI, &WaterBalance::tree_ei);

    }

    virtual ~WaterBalance()
    {
    }


    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;
        _parameters = parameters;
//        parameters
        H_CR = parameters.get("H_CR");
        H_PF_Z1 = parameters.get("H_PF_Z1");
        Z1 = parameters.get("Z1");
        H_PF = parameters.get("H_PF");
        Z2 = parameters.get("Z2");
        H_0 = parameters.get("H_0");
        KC = parameters.get("KC");
        SEUIL_EVAP = parameters.get("SEUIL_EVAP");
        SEUIL_TRANSPI = parameters.get("SEUIL_TRANSPI");


//        internals
        TailleC1 = ( H_CR - H_PF_Z1 ) * Z1;
        TailleVap = H_PF_Z1 * Z1;
        TailleC1moinsVap = TailleC1 - TailleVap;
        TailleC2 = ( H_CR  - H_PF ) * Z2;
        TailleC =  TailleC2 + TailleC1 - TailleVap;
        a_C1 = min(TailleC1, (H_0 - H_PF_Z1 ) * Z1);
        qte_H2O_C1 = max(0., a_C1 );
        a_vap = min(TailleVap , (H_0 - H_PF_Z1 ) * Z1 );
        qte_H2O_Vap = max (0., a_vap);
        a_C2 = min(TailleC2, (H_0 - H_PF ) * Z2);
        qte_H2O_C2 = max(0., a_C2);
        a_C = qte_H2O_C1 + qte_H2O_C2 - qte_H2O_Vap;
        qte_H2O_C = max(0., a_C);
        a_C1moinsV = qte_H2O_C1 - qte_H2O_Vap;
        qte_H2O_C1moinsVap = max(0., a_C1moinsV);
        qte_H2O_C1_Racines = max(0.,qte_H2O_C1 * racines_TailleC1 / TailleC1); //TO CHECK
        qte_H2O_Vap_Racines = max(0.,qte_H2O_Vap * racines_TailleVap / TailleVap); //TO CHECK
        qte_H2O_C2_Racines = max(0.,qte_H2O_C2 * racines_TailleC2 / TailleC2);
        qte_H2O_C_Racines = max(0.,qte_H2O_C * racines_TailleC / TailleC);
        qte_H2O_C1moinsVap_Racines = max(0.,qte_H2O_C1moinsVap * racines_TailleC1moinsVap / TailleC1moinsVap);
        FractionC1 = 0;
        FractionC2 = 0;
        FractionC = 0;
        FractionC1Racine = 0;
        FractionC2Racine = 0;
        ftsw = 0;
        FractionC1moinsVapRacine =  0;
//        compute_fraction();
        EvapMax = 0;
        Transp_Max = 0;
        pluie_efficace = 0;
        Evap = 0;
        EvapC1moinsVap = 0;
        EvapVap = 0;
        Transpi = 0 ;
        TranspiC2 = 0;
        TranspiC1moinsVap = 0;
    }

    void compute_fraction() {
        FractionC1 = qte_H2O_C1/ TailleC1;
        if (TailleC2 > 0)
            FractionC2 = qte_H2O_C2/ TailleC2;
        else
            FractionC2 = 0 ;
        FractionC = qte_H2O_C/ TailleC ;
        FractionC1Racine = qte_H2O_C1_Racines / racines_TailleC1;
        if (racines_TailleC2 > 0)
            FractionC2Racine = qte_H2O_C2_Racines / racines_TailleC2;
        else
            FractionC2Racine = 0;
        ftsw = qte_H2O_C_Racines / racines_TailleC;
        FractionC1moinsVapRacine =  qte_H2O_C1moinsVap_Racines / racines_TailleC1moinsVap;
    }


    void compute(double t, bool /* update */)
    {
        double pluie = _parameters.get(t).Rain;

        EvapMax = ( 1 - tree_ei ) * Et0 * KC;
        Transp_Max = tree_ei * Et0 * KC;

        //        compute_pluie_efficace
        double Pluie_au_sol, Ecoul_stipe;
        if ((0.916 * pluie- 0.589) < 0)
            Pluie_au_sol = 0;
        else
            Pluie_au_sol =(0.916 * pluie - 0.589);

        if ((0.0713 * pluie - 0.735) < 0)
            Ecoul_stipe = 0;
        else
            Ecoul_stipe=(0.0713* pluie - 0.735);
        pluie_efficace = Pluie_au_sol + Ecoul_stipe;

//            compute_BH_apres_pluie
        double mem_qte_H2O_C1 = qte_H2O_C1;
        double mem_qte_H2O_Vap = qte_H2O_Vap;
        if ((qte_H2O_Vap + pluie_efficace) >= TailleVap) {
            qte_H2O_Vap = TailleVap;
            if ((qte_H2O_C1moinsVap + ( pluie_efficace - TailleVap + mem_qte_H2O_Vap )) >= TailleC1moinsVap) {
                qte_H2O_C1moinsVap = TailleC1moinsVap;
                qte_H2O_C1 = qte_H2O_C1moinsVap + qte_H2O_Vap;
                if ( (qte_H2O_C2 + mem_qte_H2O_C1 + pluie_efficace - TailleC1) >= TailleC2 )
                    qte_H2O_C2 = TailleC2;
                else
                    qte_H2O_C2 += mem_qte_H2O_C1 + pluie_efficace - TailleC1;
            } else {
                qte_H2O_C1moinsVap +=  pluie_efficace - TailleVap + mem_qte_H2O_Vap;
                qte_H2O_C1 = qte_H2O_C1moinsVap + qte_H2O_Vap;
            }
        } else {
            qte_H2O_Vap += pluie_efficace;
            qte_H2O_C1 = qte_H2O_Vap + qte_H2O_C1moinsVap;
        }
        qte_H2O_C = qte_H2O_C1moinsVap + qte_H2O_C2;

//        compute_BH_Racines_pluie
        double mem_qte_H2O_C1_Racines = qte_H2O_C1_Racines;
        double mem_qte_H2O_Vap_Racines = qte_H2O_Vap_Racines;
        if ((qte_H2O_Vap_Racines + pluie_efficace) >= racines_TailleVap) {
            qte_H2O_Vap_Racines = racines_TailleVap;
            if ((qte_H2O_C1moinsVap_Racines + ( pluie_efficace - racines_TailleVap + mem_qte_H2O_Vap_Racines )) >= racines_TailleC1moinsVap) {
                qte_H2O_C1moinsVap_Racines = racines_TailleC1moinsVap;
                qte_H2O_C1_Racines = qte_H2O_C1moinsVap_Racines + qte_H2O_Vap_Racines;
                if ((qte_H2O_C2_Racines + mem_qte_H2O_C1_Racines + pluie_efficace - racines_TailleC1) >= racines_TailleC2)
                    qte_H2O_C2_Racines = racines_TailleC2;
                else
                    qte_H2O_C2_Racines += mem_qte_H2O_C1_Racines  + pluie_efficace - racines_TailleC1;
            } else {
                qte_H2O_C1moinsVap_Racines +=  pluie_efficace - racines_TailleVap + mem_qte_H2O_Vap_Racines;
                qte_H2O_C1_Racines = qte_H2O_C1moinsVap_Racines + qte_H2O_Vap_Racines;
            }
        } else {
            qte_H2O_Vap_Racines +=  pluie_efficace;
            qte_H2O_C1_Racines = qte_H2O_C1moinsVap_Racines + qte_H2O_Vap_Racines;
        }
        qte_H2O_C_Racines =  qte_H2O_C1moinsVap_Racines + qte_H2O_C2_Racines;

        compute_fraction();

//       compute_BH_apres_Evap
        Evap = EvapMax * (FractionC1 > SEUIL_EVAP ? 1 : FractionC1 / SEUIL_EVAP ) ;
        if ( qte_H2O_C1moinsVap - Evap >=0 ){
            qte_H2O_C1moinsVap +=  - Evap;
            EvapC1moinsVap = Evap;
            EvapVap = 0;
        } else {
            EvapC1moinsVap = qte_H2O_C1moinsVap;
            qte_H2O_C1moinsVap = 0;
            EvapVap = Evap - EvapC1moinsVap;
            qte_H2O_Vap +=  - EvapVap;
        }
        qte_H2O_C1 = qte_H2O_C1moinsVap + qte_H2O_Vap;
        qte_H2O_C = qte_H2O_C1 + qte_H2O_C2 - qte_H2O_Vap;

//      compute_BH_Racines_apres_Evap
        a_C1moinsVap_Racines = qte_H2O_C1moinsVap_Racines - EvapC1moinsVap * racines_TailleC1moinsVap / TailleC1moinsVap;
        qte_H2O_C1moinsVap_Racines = max (0. , a_C1moinsVap_Racines);
        a_Vap_Racines = qte_H2O_Vap_Racines  - EvapVap * racines_TailleVap / TailleVap;
        qte_H2O_Vap_Racines = max(0., a_Vap_Racines);
        qte_H2O_C1_Racines = qte_H2O_Vap_Racines + qte_H2O_C1moinsVap_Racines;
        qte_H2O_C_Racines = qte_H2O_C2_Racines + qte_H2O_C1moinsVap_Racines;

        compute_fraction();

//      compute_BH_Racines_apres_Transpi
        Transpi = Transp_Max * (ftsw > SEUIL_TRANSPI ? 1 : ftsw / SEUIL_TRANSPI) ;
        if (qte_H2O_C2_Racines > 0)
            TranspiC2 = min( Transpi * ( qte_H2O_C2_Racines / ( qte_H2O_C2_Racines + qte_H2O_C1moinsVap_Racines ) ), qte_H2O_C2_Racines );
        else
            TranspiC2 = 0;

        if (qte_H2O_C1moinsVap_Racines > 0)
            TranspiC1moinsVap= min(Transpi * (qte_H2O_C1moinsVap_Racines / (qte_H2O_C2_Racines + qte_H2O_C1moinsVap_Racines)), qte_H2O_C1moinsVap_Racines);
        else
            TranspiC1moinsVap = 0;

        qte_H2O_C1moinsVap_Racines +=  - TranspiC1moinsVap  ;
        qte_H2O_C2_Racines +=  - TranspiC2;
        qte_H2O_C_Racines = qte_H2O_C2_Racines + qte_H2O_C1moinsVap_Racines;
        qte_H2O_C1_Racines = qte_H2O_Vap_Racines + qte_H2O_C1moinsVap_Racines;

//      compute_BH_apres_Transpi()
        qte_H2O_C1moinsVap += - TranspiC1moinsVap ;
        qte_H2O_C2 +=  - TranspiC2;
        qte_H2O_C = qte_H2O_C2 + qte_H2O_C1moinsVap;
        qte_H2O_C1 = qte_H2O_Vap + qte_H2O_C1moinsVap;

        compute_fraction();
    }

};
} //namespace model
#endif // WATERBALANCE_H
