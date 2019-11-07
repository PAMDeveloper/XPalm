#ifndef PHYTOMER_H
#define PHYTOMER_H
/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>
#include <plant/phytomer/leaf.h>
#include <plant/phytomer/internode.h>
#include <plant/phytomer/inflo.h>

namespace model {

class Phytomer : public CoupledModel < Phytomer >
{
public:
    enum submodels { LEAF, INFLO, INTERNODE };

    enum internals { RANK,
                     NUMBER,
                     STATE,
                     AGE_AT_CREATION
//                     DEMAND
                   };

    enum external { YOUNG_PHYTO_NB };

private:
    //     submodels
    std::unique_ptr < Leaf > leaf;
    std::unique_ptr < Internode > internode;
    std::unique_ptr < Inflo > inflo;

    //parameters
    double INACTIVE_PHYTOMER_NUMBER;

    //     internals
    phytomer::phytomer_state state;
    double rank;
    double number;
    double tree_age_at_creation;
    double age;

    //   externals
    int youngest_phytomer_number;

public:
    Phytomer(int number, int rank, phytomer::phytomer_state state, double tree_age_at_creation,
             double tree_age, double prod_speed, double flo_tt, double harv_tt, double inflo_factor) :
        number(number),
        rank(rank),
        state(state),
        age(tree_age - tree_age_at_creation),
        tree_age_at_creation(tree_age_at_creation),
        internode(new Internode(prod_speed, number, tree_age_at_creation)),
        leaf(new Leaf(prod_speed, flo_tt, harv_tt, inflo_factor)),
        inflo(new Inflo(prod_speed, flo_tt, harv_tt, inflo_factor))
    {

        //         submodels
        setsubmodel(LEAF, leaf.get());
        setsubmodel(INTERNODE, internode.get());
        setsubmodel(INFLO, inflo.get());

        //         internals
        Internal(RANK, &Phytomer::rank);
        Internal(STATE, &Phytomer::state);
        Internal(NUMBER, &Phytomer::number);
        Internal(AGE_AT_CREATION, &Phytomer::tree_age_at_creation);

        // externals
        Internal(YOUNG_PHYTO_NB, &Phytomer::youngest_phytomer_number);
    }

    virtual ~Phytomer()
    {
        leaf.reset(nullptr);
        internode.reset(nullptr);
        inflo.reset(nullptr);
    }

    Leaf * leaf_model() const { return leaf.get(); }
    Inflo * inflo_model() const { return inflo.get(); }
    Internode * internode_model() const { return internode.get(); }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        //parameters
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");

        //submodels
        leaf->init(t, parameters);
        internode->init(t, parameters);
        inflo->init(t, parameters);
    }


//    double TT_since_rank1 = TEff_ini * phytomer_age; //AGE

    void compute(double t, bool /* update */)
    {
        if(state == phytomer::DEAD) //TODO remove to include leaf/internode in demand when inactive
            return;

        age++;
        rank = youngest_phytomer_number - number - INACTIVE_PHYTOMER_NUMBER + 1;
        if(rank == 1 && state == phytomer::INACTIVE)
            state = phytomer::ACTIVE;


        if(state != phytomer::ACTIVE) //TODO remove to include leaf/internode in demand when inactive
            return;

        (*leaf)(t);
        (*internode)(t);
        (*inflo)(t);

        compute_IC();
    }


    void compute_IC(){
        if (rank > ICsex_RANG_DEBUT && rank < ICsex_RANG_FIN ) {
            nb_joursICsex += 1;
            ICsex_tot += tree_IC;
            ICsex = ICsex_tot / nb_joursICsex;
        }

        if (rank > ICabort_RANG_DEBUT && rank < ICabort_RANG_FIN) {
            nb_joursICabort += 1;
            ICabort_tot += tree_IC;
            ICabort = ICabort_tot / nb_joursICabort;
        }

        if (rank > IC_spikelet_RANG_DEBUT && rank < IC_spikelet_RANG_FIN){
            nb_joursIC_spikelet += 1;
            IC_spikelet_tot += tree_IC;
            IC_spikelet = IC_spikelet_tot / nb_joursIC_spikelet;
        }

        if (TT_corrige > TT_ini_flowering - DEBUT_RANG_SENSITIVITY_NOUAISON / production_speed &&
                TT_corrige < TT_ini_flowering - FIN_RANG_SENSITIVITY_NOUAISON / production_speed &&
                TT_corrige < TT_ini_flowering) {
            nb_joursIC_setting += 1;
            IC_setting_tot += tree_IC;
            IC_setting = (IC_setting_tot) / nb_joursIC_setting;
        }
    }

};

} //namespace model
#endif // PHYTOMER_H
