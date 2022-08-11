#ifndef INTERNODE_H
#define INTERNODE_H
/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>

namespace model {

class Internode : public AtomicModel < Internode >
{
public:
    enum internals { LENGTH_INCREASE_POT,
                     LENGTH,
                     BIOMASS,
                     DEMAND,
                     MAX_LENGTH_POT,
                     ASSIMILATE_SUPPLY,
                     TT_INTERNODE_DEV_DURATION,
                     TT_INI_ELONGATION,
                     //                     TT_CORRIGE,
                     //                     GAIN_TEFF_JOUR
                   };

    enum externals { TEFF,
                     FR_RESTE,
                     TT_SINCE_APPEARANCE};

private:
    xpalm::ModelParameters _parameters;

    //      parameters
    double STEM_RADIUS;
    double STEM_APPARENT_DENSITY;
    double RESPIRATION_COST_INTERNODE;
    double PLASTICITY_INTERNODE_IC;
    double TEFF_INI ;
    double CONSTRUCTION_COST_INFLO;
    double INACTIVE_PHYTOMER_NUMBER;

    //     internals
    double length_increase_pot;
    double length;
    double biomass;
    double demand;
    double max_length_pot;
    double assimilate_supply;
    double TT_internode_dev_duration;
    //    double TT_corrige;
    double TT_ini_elongation;
    //    double gain_TEff_jour;

    //     externals
    double TEff;
    double fr_reste;
    double TT_since_appearance;

public:

    Internode()
    {
        //        //         internals
        Internal(LENGTH_INCREASE_POT, &Internode::length_increase_pot);
        Internal(LENGTH, &Internode::length);
        Internal(BIOMASS, &Internode::biomass);
        Internal(DEMAND, &Internode::demand);
        Internal(MAX_LENGTH_POT, &Internode::max_length_pot);
        Internal(ASSIMILATE_SUPPLY, &Internode::assimilate_supply);
        Internal(TT_INTERNODE_DEV_DURATION, &Internode::TT_internode_dev_duration);
        Internal(TT_INI_ELONGATION, &Internode::TT_ini_elongation);
        //        Internal(TT_CORRIGE, &Internode::TT_corrige);
        //        Internal(GAIN_TEFF_JOUR, &Internode::gain_TEff_jour);

        //        //          externals
        External(TEFF, &Internode::TEff);
        External(FR_RESTE, &Internode::fr_reste);
        External(TT_SINCE_APPEARANCE, &Internode::TT_since_appearance);

    }

    virtual ~Internode()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters){}
    void init(double t, const xpalm::ModelParameters& parameters, double phytomer_age,double TT_since_appearance_,  double tree_age_at_phyto_creation, double production_speed)
    {
        //        AtomicModel<Internode>::init(t, parameters);

        //default init
        last_time = t;
        _parameters = parameters;

        //        parameters
        RESPIRATION_COST_INTERNODE = parameters.get("RESPIRATION_COST_INTERNODE");
//        PLASTICITY_INTERNODE_IC = parameters.get("PLASTICITY_INTERNODE_IC");
        STEM_RADIUS = parameters.get("STEM_RADIUS"); //cm
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY"); //(g cm-3)
        TEFF_INI = parameters.get("T_EFF_INI");
        CONSTRUCTION_COST_INFLO = parameters.get("CONSTRUCTION_COST_INFLO");
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");

        //        internals
        length_increase_pot = 0;
        demand = 0;
        length = 0;
        biomass = 0;
        assimilate_supply = 0;
        //        TT_corrige = 0;
        //        gain_TEff_jour = 0;
        TT_since_appearance = TT_since_appearance_;
        TT_ini_elongation= (INACTIVE_PHYTOMER_NUMBER+30) / production_speed; //growth start at rank 30 TODO convert into param if necessary


        // ini
        double INTERNODE_START_GROWTH = parameters.get("INTERNODE_START_GROWTH") * 365;
        double INTERNODE_STOP_GROWTH = parameters.get("INTERNODE_STOP_GROWTH") * 365;
        double INTERNODE_LENGTH_ADULT = parameters.get("INTERNODE_LENGTH_ADULT") ; //cm
        double INTERNODE_LENGTH_INI = parameters.get("INTERNODE_LENGTH_INI"); //cm
        max_length_pot = age_relative_var(tree_age_at_phyto_creation + phytomer_age, INTERNODE_START_GROWTH, INTERNODE_STOP_GROWTH, INTERNODE_LENGTH_INI, INTERNODE_LENGTH_ADULT); // cm

        TT_internode_dev_duration = 20 / production_speed; //20 ranks to grow TODO convert into param if necessary

        // init structure


        if (TT_since_appearance >= TT_ini_elongation && TT_since_appearance< (TT_ini_elongation+TT_internode_dev_duration)){
            double fr_allongement = min(1.0, (TT_since_appearance - TT_ini_elongation) / TT_internode_dev_duration );

            length = fr_allongement * max_length_pot; //cm
            biomass =  length * _PI * pow( STEM_RADIUS, 2)* STEM_APPARENT_DENSITY; //gCH2O
            demand =  max_length_pot * _PI * pow( STEM_RADIUS, 2)* STEM_APPARENT_DENSITY * CONSTRUCTION_COST_INFLO * ( TEFF_INI / TT_internode_dev_duration );
        }


    }

    void compute(double /*t*/, bool /* update */)
    {
        //        age += 1;

        //        growth();

        assimilate_supply = demand * fr_reste; //compute_assimilate_supply
        length += length_increase_pot * fr_reste; //compute_length
        biomass += assimilate_supply / RESPIRATION_COST_INTERNODE; //compute_biomass



        if (TT_since_appearance >= TT_ini_elongation && TT_since_appearance< (TT_ini_elongation+TT_internode_dev_duration)){
            length_increase_pot =  max_length_pot * TEff / TT_internode_dev_duration; //cm
            biomass =  length * _PI * pow( STEM_RADIUS, 2)* STEM_APPARENT_DENSITY; //gCH2O
            demand =  max_length_pot * _PI * pow( STEM_RADIUS, 2)* STEM_APPARENT_DENSITY * RESPIRATION_COST_INTERNODE * ( TEff/ TT_internode_dev_duration );
        }
        else{
            demand=0;
            length_increase_pot=0;
        }



    }
};
} //namespace model
#endif // INTERNODE_H
