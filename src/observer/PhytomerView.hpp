/**
 * @file model/observer/PlantView.hpp
 * @author The XPalm Development Team
 * See the AUTHORS file
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

#ifndef MODEL_OBSERVER_PHYTOMER_VIEW_HPP
#define MODEL_OBSERVER_PHYTOMER_VIEW_HPP

#include <plant/processes/meteo.h>
#include <plant/processes/reserve.h>
#include <plant/root.h>
#include <plant/tree.h>

using namespace model;
namespace observer {

class PhytomerView : public View
{
public:
    PhytomerView()
    {

        //PHYTOMERS
        //        selector("DEMAND_BUNCH", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::BUNCH_DEMAND});
        //        selector("DEMAND_MALE", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::MALE_DEMAND});

        selector("LEAF_RANK", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::RANK});
        selector("LEAF_NUMBER", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::NUMBER});
        selector("TREE_AGE_AT_CREATION", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::TREE_AGE_AT_CREATION});

        selector("STATE", artis::kernel::INT, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::INFLO_STATUS});
        selector("INFLO_DEV_FACTOR", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::INFLO_DEV_FACTOR});
        //        selector("IC_SEX", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::IC_SEX});
        //        selector("IC_ABORTION", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::IC_ABORT});
//        selector("TT_INI_ABORTION", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::TT_INI_ABORTION});
//        selector("TT_INI_FLOWERING", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::TT_INI_FLOWERING});

        selector("FRUIT_NUMBER", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::BUNCH, Bunch::FRUIT_NUMBER});

        selector("BIOMASS_OIL", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::BUNCH, Bunch::OIL_BIOMASS});
        selector("BIOMASS_MALE", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::MALE_BIOMASS});
        selector("BIOMASS_BUNCH", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::BUNCH_BIOMASS});
        selector("BIOMASS_STALK", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::PEDUNCLE_BIOMASS});


        selector("DEMAND_MALE", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::MALE_DEMAND});
        selector("DEMAND_BUNCH", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::BUNCH_DEMAND});
        selector("DEMAND_STALK", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::PEDUNCLE_DEMAND});

        selector("LEAF_AREA", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::LEAF, Leaf::LEAFAREA});

        selector("SEX_RATIO", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::SEX_RATIO});
        selector("ABORTION_RATE", artis::kernel::DOUBLE, {Tree::PHYTOMERS, Phytomer::INFLO, Inflo::ABORTION_RATE});

    }

    virtual ~PhytomerView()
    { }
};

}

#endif
