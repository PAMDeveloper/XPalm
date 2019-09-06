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
                     LENGTH_INCREASE_POT,
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
                     FR_RESTE,
                     PHYTOMER_NUMBER };

private:
     const xpalm::ModelParameters _parameters;

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
    double RANG_D_ABLATION;

    //     internals
    double age;
    double length_increase_pot;
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
    double phytomer_number;

public:

    Internode()
    {
        //         internals
        Internal(AGE, &Internode::age);
        Internal(LENGTH_INCREASE_POT, &Internode::length_increase_pot);
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
        External(PHYTOMER_NUMBER, &Internode::phytomer_number);
    }

    virtual ~Internode()
    {
    }


    void growth_demand(double t) {
        gain_TEff_jour = TEff;
        age += 1;
        duree_allongement = 1 / productionSpeed;

        if (TT_corrige < duree_allongement) {
            max_length = compute_length(t - _parameters.beginDate);
        }

        //compute_length_max_increase
        if (TT_corrige < duree_allongement) {
            length_increase_pot = ( max_length * TEff ) / duree_allongement;
        } else {
            length_increase_pot = 0;
        }

        double volume_pot = _PI * pow( STEM_RAYON, 2) * length_increase_pot;
        demand =  volume_pot * STEM_APPARENT_DENSITY * COUT_RESPI_INTERNODE * 1000;
    }


    void growth() {
        assimilate_supply = demand   * fr_reste; //compute_assimilate_supply
        length += length_increase_pot  * fr_reste; //compute_length
        biomass += assimilate_supply * (1 / COUT_RESPI_INTERNODE); //compute_biomass
        TT_corrige +=  pow( fr_reste, PLASTICITY_INTERNODE_IC ) * gain_TEff_jour; //compute_TT_new
    }



    double compute_length(double days) {
        double age_actuel = AGE_PARAM + (days / 365);
        double l;
        if  (age_actuel < DEBUT_CROISSANCE_EN) {
            l = EN_LENGHT_INI;
        } else {
            if (age_actuel < FIN_CROISSANCE_EN) {
                double growth_speed = (LENGHT_ADULTE - EN_LENGHT_INI) / (FIN_CROISSANCE_EN - DEBUT_CROISSANCE_EN); // m.an-1
                double phytomer_age = age_actuel - DEBUT_CROISSANCE_EN;
                l =  ( growth_speed * phytomer_age ) +  EN_LENGHT_INI;
            } else {
                l = LENGHT_ADULTE;
            }
        }
        return l;
    }

    void init_structure(double t) {
        //internode
        duree_allongement = 1 / productionSpeed;
        double date_d_appar =  (RANG_D_ABLATION - phytomer_number) / (10 * productionSpeed ); // 10 days per rank

        if (TT_corrige < duree_allongement) {
            length = compute_length( -date_d_appar );  // 10Cj en moyenne
        } else {
            length = compute_length( -date_d_appar ) * TT_corrige / duree_allongement;
        }

        double volume = _PI * pow( STEM_RAYON, 2) * length;
        biomass = 1000 * volume * STEM_APPARENT_DENSITY;
    }


    void init(double t, const xpalm::ModelParameters& parameters)
    {
        //default init
        last_time = t-1;
       _parameters = parameters;

        //        parameters
        RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        COUT_RESPI_INTERNODE = parameters.get("COUT_RESPI_INTERNODE");
        AGE_PARAM = parameters.get("AGE");
        DEBUT_CROISSANCE_EN = parameters.get("DEBUT_CROISSANCE_EN");
        FIN_CROISSANCE_EN = parameters.get("FIN_CROISSANCE_EN");
        PLASTICITY_INTERNODE_IC = parameters.get("PLASTICITY_INTERNODE_IC");
        STEM_RAYON = parameters.get("STEM_RAYON");
        LENGHT_ADULTE = parameters.get("LENGHT_ADULTE") / 100; //from cm to m
        EN_LENGHT_INI = parameters.get("EN_LENGHT_INI") / 100; //from cm to m


        //        internals
        age = 0;
        length_increase_pot = 0;
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
        growth();
        growth_demand(t);
    }
};
} //namespace model
#endif // INTERNODE_H
