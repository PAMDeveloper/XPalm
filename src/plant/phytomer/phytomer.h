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
                     STATE,
                     DEMAND,
                     NUMBER,
                     DATE,
                     STEP_APP};
private:
//     submodels
    std::unique_ptr < Leaf > leaf;
    std::unique_ptr < Internode > internode;
    std::unique_ptr < Inflo > inflo;

//     internals
    double rank;
    phytomer::phytomer_state state;
    double demand;
    double number;
    double date;
    double step_apparition;

public:
    Phytomer(int rank, int number, phytomer::phytomer_state state, double date) :
        rank(rank), state(state), number(number),
        date(date),
        leaf(new Leaf()),
        internode(new Internode()),
        inflo(new Inflo())
    {
//         submodels
        setsubmodel(LEAF, leaf.get());
        setsubmodel(INTERNODE, internode.get());
        setsubmodel(INFLO, inflo.get());

//         internals
        Internal(RANK, &Phytomer::rank);
        Internal(STATE, &Phytomer::state);
        Internal(DEMAND, &Phytomer::demand);
        Internal(NUMBER, &Phytomer::number);
        Internal(DATE, &Phytomer::date);
        Internal(STEP_APP, &Phytomer::step_apparition);
    }

    virtual ~Phytomer()
    {
        leaf.reset(nullptr);
        internode.reset(nullptr);
        inflo.reset(nullptr);
    }

    void init_structure(double t) {
        leaf->init_structure(t);
        internode->init_structure(t);
        inflo->init_structure(t);
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
//        parameters
        double INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");
        double RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        double PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");

        step_apparition = (t - parameters.beginDate) - (INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION - number) / (10.0 * PRODUCTION_SPEED_INITIAL);

//         submodels
        leaf->init(t, parameters);
        internode->init(t, parameters);
        inflo->init(t, parameters);
    }

    void compute(double t, bool /* update */)
    {

        leaf->put(t, Leaf::PHYTOMER_RANK, rank);
        leaf->put(t, Leaf::PHYTOMER_STATE, state);
        leaf->put(t, Leaf::BUNCH_SEXE, inflo->get<inflo::inflo_sex, Inflo>(t, Inflo::SEX));
        leaf->put(t, Leaf::BUNCH_AVORT, inflo->get<inflo::inflo_sex, Inflo>(t, Inflo::AVORT));
        leaf->put(t, Leaf::BUNCH_STATUT, inflo->get<inflo::inflo_states, Inflo>(t, Inflo::STATUS));
        (*leaf)(t);

        internode->put(t, Internode::PHYTOMER_NUMBER, number);
        (*internode)(t);

        inflo::inflo_states bunch_statut = inflo.get()->get< inflo::inflo_states, Inflo >(t, Inflo::STATUS);
        if(!bunch_statut.is(inflo::RECOLTE)) {
            inflo->put(t, Inflo::PHYTOMER_RANK, rank);
            (*inflo)(t);
        }
    }


    Leaf * leaf_model() const { return leaf.get(); }
    Inflo * bunch_model() const { return inflo.get(); }
    Internode * internode_model() const { return internode.get(); }

};

} //namespace model
#endif // PHYTOMER_H
