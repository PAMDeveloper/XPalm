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
    enum internals { AGE,
                     LENGTH_MAX_INCREASE,
                     DEMAND,
                     MAX_LENGTH,
                     LENGTH,
                     BIOMASS,
                     ASSIMILATE_SUPPLY,
                     DUREE_ALLONGEMENT,
                     TT_CORRIGE,
                     GAIN_TEFF_JOUR };

    enum externals { TEFF,
                     PRODUCTIONSPEED,
                     FR_RESTE };

private:

    //      parameters
    double STEM_RAYON;
    double STEM_APPARENT_DENSITY;
    double COUT_RESPI_INTERNODE;
    double AGE_PARAM;
    double DEBUT_CROISSANCE_EN;
    double EN_LENGHT_INI;
    double FIN_CROISSANCE_EN;
    double LENGHT_ADULTE;
    double PLASTICITY_INTERNODE_IC;

    //     internals
    double age;
    double length_max_increase;
    double demand;
    double max_length;
    double length;
    double biomass;
    double assimilate_supply;
    double duree_allongement;
    double TT_corrige;
    double gain_TEff_jour;

    //     externals
    double TEff;
    double productionSpeed;
    double fr_reste;

public:

    Internode()
    {
        //         internals
        Internal(AGE, &Internode::age);
        Internal(LENGTH_MAX_INCREASE, &Internode::length_max_increase);
        Internal(DEMAND, &Internode::demand);
        Internal(MAX_LENGTH, &Internode::max_length);
        Internal(LENGTH, &Internode::length);
        Internal(BIOMASS, &Internode::biomass);
        Internal(ASSIMILATE_SUPPLY, &Internode::assimilate_supply);
        Internal(DUREE_ALLONGEMENT, &Internode::duree_allongement);
        Internal(TT_CORRIGE, &Internode::TT_corrige);
        Internal(GAIN_TEFF_JOUR, &Internode::gain_TEff_jour);

        //          externals
        External(TEFF, &Internode::TEff);
        External(PRODUCTIONSPEED, &Internode::productionSpeed);
        External(FR_RESTE, &Internode::fr_reste);

    }

    virtual ~Internode()
    {
    }


    void growth_demand(double t) {
        gain_TEff_jour = TEff;
        age += 1;
        duree_allongement = 1 / productionSpeed;

        if (TT_corrige < duree_allongement) {
            //compute final length
            double age_actuel = AGE_PARAM + t/365;
            if  (age_actuel < DEBUT_CROISSANCE_EN)
                max_length = EN_LENGHT_INI;
            else {
                if (age_actuel < FIN_CROISSANCE_EN)
                    max_length = (LENGHT_ADULTE - EN_LENGHT_INI) / (FIN_CROISSANCE_EN - DEBUT_CROISSANCE_EN) * (age_actuel - DEBUT_CROISSANCE_EN) +  EN_LENGHT_INI;
                else
                    max_length = LENGHT_ADULTE;
            }
            max_length = max_length / 100;
        }

        //compute_length_max_increase
        if (TT_corrige < duree_allongement)
            length_max_increase = max_length * TEff * 1 / duree_allongement;
        else
            length_max_increase = 0;

        double volume = _PI * pow( STEM_RAYON, 2) * length_max_increase;
        demand =  STEM_APPARENT_DENSITY * volume * COUT_RESPI_INTERNODE * 1000;
    }

    void growth() {
        assimilate_supply = demand   * fr_reste;
        length += length_max_increase  * fr_reste;
        biomass += assimilate_supply * (1 / COUT_RESPI_INTERNODE);
        TT_corrige +=  pow( fr_reste, PLASTICITY_INTERNODE_IC ) * gain_TEff_jour;
    }


    void init(double /*t*/, const xpalm::ModelParameters& parameters)
    {
        //        parameters
        STEM_RAYON = parameters.get("STEM_RAYON");
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        COUT_RESPI_INTERNODE = parameters.get("COUT_RESPI_INTERNODE");
        AGE_PARAM = parameters.get("AGE");
        DEBUT_CROISSANCE_EN = parameters.get("DEBUT_CROISSANCE_EN");
        EN_LENGHT_INI = parameters.get("EN_LENGHT_INI");
        FIN_CROISSANCE_EN = parameters.get("FIN_CROISSANCE_EN");
        LENGHT_ADULTE = parameters.get("LENGHT_ADULTE");
        PLASTICITY_INTERNODE_IC = parameters.get("PLASTICITY_INTERNODE_IC");

        //        internals
        age = 0;
        length_max_increase = 0;
        demand = 0;
        max_length = 0;
        length = 0;
        biomass = 0;
        assimilate_supply = 0;
        duree_allongement = 0;
        TT_corrige = 0;
        gain_TEff_jour = 0;
    }

    void compute(double t, bool /* update */)
    {
        //default init
        last_time = t-1;

        growth();
        growth_demand(t);
    }
};
} //namespace model
#endif // INTERNODE_H
