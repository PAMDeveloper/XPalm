/**
 * @file xpalm/root/Model.hpp
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

#ifndef ROOT_MODEL_HPP
#define ROOT_MODEL_HPP

#include "defines.hpp"

namespace model {

class Racines : public AtomicModel < Racines >
{
private:
    //    parameters
    double FTSW_RACINES;
    double VITESSE_RACINES;
    double Z2;
    double Z1;
    double H_CR;
    double H_PF;


    //    internals
    double TailleC1;
    double TailleVap;
    double TailleC1moinsVap;
    double TailleC2;
    double TailleC;
    double z;

    //    externals
    double ftsw;
    double TEff;

public:
    enum internals { TAILLEC1,
                     TAILLEVAP,
                     TAILLEC1MOINSVAP,
                     TAILLEC2,
                     TAILLEC,
                     Z };

    enum externals { FTSW, TEFF };


    Racines() {
        //    internals
        Internal(TAILLEC1, &Racines::TailleC1);
        Internal(TAILLEVAP, &Racines::TailleVap);
        Internal(TAILLEC1MOINSVAP, &Racines::TailleC1moinsVap);
        Internal(TAILLEVAP, &Racines::TailleVap);
        Internal(TAILLEC2, &Racines::TailleC2);
        Internal(TAILLEC, &Racines::TailleC);
        Internal(Z, &Racines::z);

        //    external variables
        External(FTSW, &Racines::ftsw);
        External(TEFF, &Racines::TEff);
    }

    virtual ~Racines()
    {}


    void init(double t, const xpalm::ModelParameters& parameters) {
        last_time = t-1;

        //    parameters variables
        FTSW_RACINES = parameters.get("FTSW_RACINES");
        VITESSE_RACINES = parameters.get("VITESSE_RACINES");
        Z2 = parameters.get("Z2");
        Z1 = parameters.get("Z1");
        H_CR = parameters.get("H_CR");
        H_PF = parameters.get("H_PF");

        //    computed variables (internal)
        z = parameters.get("Z_INIT");
        TailleC1 = 0;
        TailleVap = 0;
        TailleC1moinsVap = 0;
        TailleVap = 0;
        TailleC2 = 0;
        TailleC = 0;
        compute_compartment_size();
    }


    void compute_compartment_size() {
        if (z > Z1) {
            TailleC1 = (H_CR - 0.5 * H_PF) * Z1;
            TailleVap = 0.5 * H_PF * Z1;
        } else {
            TailleC1 = (H_CR - 0.5 * H_PF) * z;
            TailleVap = 0.5 * H_PF * z;
        }
        TailleC1moinsVap = TailleC1 - TailleVap;

        if (z > Z2 + Z1)
            TailleC2 =  (H_CR - H_PF) * Z2;
        else
            TailleC2 = max(0. , (H_CR - H_PF) * (z - Z1));
        TailleC = TailleC2 + TailleC1moinsVap;
    }

    void compute(double /*t*/, bool /* update */) {
        //growth
        double red;
        if (ftsw > FTSW_RACINES)
            red = 1;
        else
            red = ftsw / FTSW_RACINES;

        if (z + red * VITESSE_RACINES * TEff > Z2 + Z1)
            z = Z2 + Z1;
        else
            z += red * VITESSE_RACINES * TEff;

        compute_compartment_size();
    }

};

} // namespace model

#endif //ROOT_MODEL_HPP
