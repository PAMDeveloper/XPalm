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
                     DEMAND,
                     MAX_LENGTH,
                     LENGTH,
                     BIOMASS,
                     ASSIMILATE_SUPPLY,
                     DUREE_ALLONGEMENT,
                     TT_CORRIGE,
                     GAIN_TEFF_JOUR };

    enum externals { TEFF,
                     FR_RESTE,
                     TREE_AGE};

private:
     xpalm::ModelParameters _parameters;

    //      parameters
    double STEM_RAYON;
    double STEM_APPARENT_DENSITY;
    double COUT_RESPI_INTERNODE;
    double AGE_PARAM;
    double DEBUT_CROISSANCE_EN;
    double EN_LENGHT_INI;
    double FIN_CROISSANCE_EN;
    double EN_LENGHT_ADULTE;
    double PLASTICITY_INTERNODE_IC;
    double RANG_D_ABLATION;

    //     internals
    double phytomer_number;
    double production_speed;
    double tree_age_at_phyto_creation;

    double max_length_pot;

    double age;
    double demand;
    double length_increase_pot;
    double length;
    double biomass;
    double assimilate_supply;
    double duree_allongement;
    double TT_corrige;
    double gain_TEff_jour;

    //     externals
    double TEff;
    double fr_reste;
    double tree_age;


public:

    Internode(double production_speed, int phytomer_number, double tree_age_at_phyto_creation):
        production_speed(production_speed),
        phytomer_number(phytomer_number),
        tree_age_at_phyto_creation(tree_age_at_phyto_creation)
    {
        //         internals
        Internal(LENGTH_INCREASE_POT, &Internode::length_increase_pot);
        Internal(DEMAND, &Internode::demand);
        Internal(MAX_LENGTH, &Internode::max_length_pot);
        Internal(LENGTH, &Internode::length);
        Internal(BIOMASS, &Internode::biomass);
        Internal(ASSIMILATE_SUPPLY, &Internode::assimilate_supply);
        Internal(DUREE_ALLONGEMENT, &Internode::duree_allongement);
        Internal(TT_CORRIGE, &Internode::TT_corrige);
        Internal(GAIN_TEFF_JOUR, &Internode::gain_TEff_jour);


        //          externals
        External(TEFF, &Internode::TEff);
        External(FR_RESTE, &Internode::fr_reste);
        External(TREE_AGE, &Internode::tree_age);

    }

    virtual ~Internode()
    {
    }


    void init(double t, const xpalm::ModelParameters& parameters)
    {
        //default init
        last_time = t-1;
       _parameters = parameters;

        //        parameters
        COUT_RESPI_INTERNODE = parameters.get("COUT_RESPI_INTERNODE");
        DEBUT_CROISSANCE_EN = parameters.get("DEBUT_CROISSANCE_EN") * 365;
        FIN_CROISSANCE_EN = parameters.get("FIN_CROISSANCE_EN") * 365;
        PLASTICITY_INTERNODE_IC = parameters.get("PLASTICITY_INTERNODE_IC");
        STEM_RAYON = parameters.get("STEM_RAYON");
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        EN_LENGHT_ADULTE = parameters.get("EN_LENGHT_ADULTE") / 100; //from cm to m
        EN_LENGHT_INI = parameters.get("EN_LENGHT_INI") / 100; //from cm to m

        //        internals
        length_increase_pot = 0;
        demand = 0;
        length = 0;
        biomass = 0;
        assimilate_supply = 0;
        TT_corrige = 0;
        gain_TEff_jour = 0;


        max_length_pot = age_relative_var(tree_age_at_phyto_creation, DEBUT_CROISSANCE_EN, FIN_CROISSANCE_EN, EN_LENGHT_INI, EN_LENGHT_ADULTE);
        duree_allongement = 1 / production_speed;

        double TEff_ini = parameters.get("T_EFF_INI");
        double phytomer_age = tree_age - tree_age_at_phyto_creation;
        double fr_allongement = min(1, (phytomer_age * TEff_ini) / duree_allongement );
        length = fr_allongement * max_length_pot;
        double volume = _PI * pow( STEM_RAYON, 2) * length;
        biomass = 1000 * volume * STEM_APPARENT_DENSITY;
    }

    void compute(double t, bool /* update */)
    {
        age += 1;

//        growth();
        assimilate_supply = demand * fr_reste; //compute_assimilate_supply
        length += length_increase_pot * fr_reste; //compute_length
        biomass += assimilate_supply / COUT_RESPI_INTERNODE; //compute_biomass
        TT_corrige +=  gain_TEff_jour * pow( fr_reste, PLASTICITY_INTERNODE_IC ); //compute_TT_new

//        growth_demand(t);

        //compute_length_max_increase
        if (TT_corrige < duree_allongement) {
            length_increase_pot = ( max_length_pot * TEff ) / duree_allongement;
        } else {
            length_increase_pot = 0;
        }

        double volume_pot = _PI * pow( STEM_RAYON, 2) * length_increase_pot;
        demand =  volume_pot * STEM_APPARENT_DENSITY * COUT_RESPI_INTERNODE * 1000;

        gain_TEff_jour = TEff;
    }
};
} //namespace model
#endif // INTERNODE_H
