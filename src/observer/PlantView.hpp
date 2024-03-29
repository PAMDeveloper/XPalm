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

#ifndef MODEL_OBSERVER_PLANT_VIEW_HPP
#define MODEL_OBSERVER_PLANT_VIEW_HPP

#include <plant/processes/WaterBalance.h>
#include <plant/processes/meteo.h>
#include <plant/processes/reserve.h>
#include <plant/root.h>
#include <plant/tree.h>

using namespace model;
namespace observer {

class PlantView : public View
{
public:
    PlantView()
    {
        //METEO
        selector("RG", artis::kernel::DOUBLE, {Tree::METEO, Meteo::RG});
        selector("RAIN", artis::kernel::DOUBLE, {Tree::METEO, Meteo::RAIN});
        selector("FTSW", artis::kernel::DOUBLE, {Tree::BH,WaterBalance::FTSW});
        selector("TEFF", artis::kernel::DOUBLE, {Tree::METEO,Meteo::TEFF});

        //TREE STRUCTURE
        selector("NB_LEAVES", artis::kernel::DOUBLE,{Tree::NB_LEAVES});
        selector("NB_MALES", artis::kernel::DOUBLE,{Tree::NB_MALES});
        selector("NB_BUNCHES", artis::kernel::DOUBLE,{Tree::NB_BUNCHES});
        selector("NB_ABORTED", artis::kernel::DOUBLE,{Tree::NB_ABORTED});
        selector("LAI", artis::kernel::DOUBLE,{Tree::LAI});
        selector("NEWPHYTOMEREMERGENCE", artis::kernel::DOUBLE,{Tree::NEWPHYTOMEREMERGENCE});
        selector("TOTALPHYTOMERNUMBER", artis::kernel::DOUBLE,{Tree::TOTALPHYTOMERNUMBER});
        selector("PLANTLEAFAREA", artis::kernel::DOUBLE,{Tree::PLANTLEAFAREA});
        selector("LASTLEAFAREA", artis::kernel::DOUBLE,{Tree::LASTLEAFAREA});
        selector("TREE_AGE", artis::kernel::DOUBLE,{Tree::TREE_AGE});
        selector("NEWPHYTOMER", artis::kernel::DOUBLE,{Tree::NEWPHYTOMER});
        selector("STEM_HEIGHT", artis::kernel::DOUBLE,{Tree::TRUNK_HEIGHT});

        //TREE BIOMASS
        selector("BIOMASS_BUNCH_OIL_HARVESTED", artis::kernel::DOUBLE,{Tree::BUNCH_OIL_BIOMASS_HARVESTED});
        selector("BIOMASS_BUNCH_NONOIL_HARVESTED", artis::kernel::DOUBLE,{Tree::BUNCH_NONOIL_BIOMASS_HARVESTED});
        selector("BIOMASS_TRUNK", artis::kernel::DOUBLE,{Tree::TRUNK_BIOMASS});
//        selector("FRACTION_NON_STR_BIOMASS_TOTAL", artis::kernel::DOUBLE,{Tree::FRACTION_NON_STR_BIOMASS_TOTAL});
        selector("BIOMASS_LEAVES_STRUCTURAL_HARVESTED", artis::kernel::DOUBLE,{Tree::LEAVES_STRUCTURAL_BIOMASS_HARVESTED});
        selector("BIOMASS_LEAVES_NON_STRUCTURAL_HARVESTED", artis::kernel::DOUBLE,{Tree::LEAVES_NON_STRUCTURAL_BIOMASS_HARVESTED});
        selector("BIOMASS_MALE_HARVESTED", artis::kernel::DOUBLE,{Tree::MALE_BIOMASS_HARVESTED});
        selector("BIOMASS_PEDUNCLE_HARVESTED", artis::kernel::DOUBLE,{Tree::PEDUNCLE_BIOMASS_HARVESTED});

        //TREE PHYSIOL
        selector("ASSIM", artis::kernel::DOUBLE,{Tree::ASSIM});
//        selector("ASSIM_EXCESS", artis::kernel::DOUBLE,{Tree::ASSIM_EXCESS});
        selector("GROWTH_OFFER", artis::kernel::DOUBLE,{Tree::GROWTH_OFFER});
        selector("RESERVE_STEM", artis::kernel::DOUBLE,{Tree::RES_STEM});
        selector("RESERVE_LEAVES", artis::kernel::DOUBLE,{Tree::RES_LEAVES});
        selector("RESERVE_TOTAL", artis::kernel::DOUBLE,{Tree::RES_TOTAL});
//        selector("CARBON_BALANCE", artis::kernel::DOUBLE,{Tree::C_BALANCE});
        selector("RIE", artis::kernel::DOUBLE,{Tree::EI});
//        selector("FRACTION_OFFER_TO_BUNCHES", artis::kernel::DOUBLE,{Tree::FR_FRUITS});
        selector("DEMAND_GROWTH", artis::kernel::DOUBLE,{Tree::GROWTH_DEMAND});
        selector("DEMAND_INTERNODE", artis::kernel::DOUBLE,{Tree::INTERNODE_DEMAND});
        selector("DEMAND_LEAVES", artis::kernel::DOUBLE,{Tree::LEAVES_DEMAND});
        selector("DEMAND_MALES", artis::kernel::DOUBLE,{Tree::MALE_DEMAND});
        selector("DEMAND_PEDUNCLES", artis::kernel::DOUBLE,{Tree::PEDUNCLE_DEMAND});
        selector("DEMAND_BUNCHES", artis::kernel::DOUBLE,{Tree::BUNCH_DEMAND});
        selector("COMPETITION_INDEX", artis::kernel::DOUBLE,{Tree::IC});
//        selector("RESERVE_LEAVES_EXCESS", artis::kernel::DOUBLE,{Tree::LEAVES_RESERVE_EXCESS});
        selector("MAINTENANCE_RESPIRATION", artis::kernel::DOUBLE,{Tree::RESPI_MAINTENANCE});
//        selector("AV_SEXRATIO", artis::kernel::DOUBLE,{Tree::AV_SEXRATIO});
//        selector("AV_ABORTION", artis::kernel::DOUBLE,{Tree::AV_ABORTION});


        //DEMAND
        //        selector("PEDUNCLE_DEMAND", artis::kernel::DOUBLE,{Tree::PEDUNCLE_DEMAND});
        //        selector("INTERNODE_DEMAND", artis::kernel::DOUBLE,{Tree::INTERNODE_DEMAND});
        //        selector("LEAVES_DEMAND", artis::kernel::DOUBLE,{Tree::LEAVES_DEMAND});
        //        selector("MALE_DEMAND", artis::kernel::DOUBLE,{Tree::MALE_DEMAND});
        //        selector("BUNCH_DEMAND", artis::kernel::DOUBLE,{Tree::BUNCH_DEMAND});




        //PlantModel
        //        selector("BIOMLEAF", artis::kernel::DOUBLE, {
        //                     PlantModel::LEAF_BIOM_STRUCT });
        //        selector("BIOMIN", artis::kernel::DOUBLE, {
        //                     PlantModel::INTERNODE_BIOM_STRUCT });
        //        selector("SENESC_DW", artis::kernel::DOUBLE, {
        //                     PlantModel::SENESC_DW });
        //        selector("SENESC_DW_SUM", artis::kernel::DOUBLE, {
        //                     PlantModel::SENESC_DW_SUM });
        //        selector("PHT", artis::kernel::DOUBLE, {
        //                     PlantModel::HEIGHT });
        //        selector("PHT_P", artis::kernel::DOUBLE, {
        //                     PlantModel::HEIGHT_P });
        //        selector("TT_LIG", artis::kernel::DOUBLE, {
        //                     PlantModel::TT_LIG });
        //        selector("TT", artis::kernel::DOUBLE, {
        //                     PlantModel::TT});
        //        selector("DD", artis::kernel::DOUBLE, {
        //                     PlantModel::DD});
        //        selector("EDD", artis::kernel::DOUBLE, {
        //                     PlantModel::EDD});
        //        selector("SLA", artis::kernel::DOUBLE, {
        //                     PlantModel::SLA});
        //        selector("TILLERNB_1", artis::kernel::DOUBLE, {
        //                     PlantModel::TILLERNB_1});
        //        selector("NBLEAF", artis::kernel::DOUBLE, {
        //                     PlantModel::NBLEAF});
        //        selector("BIOMAERO2", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMAERO2});
        //        selector("BIOMLEAFMAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMLEAFMAINSTEM});
        //        selector("BIOMINMAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMINMAINSTEM});
        //        selector("BIOMMAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMMAINSTEM});
        //        selector("AREALFEL", artis::kernel::DOUBLE, {
        //                     PlantModel::AREALFEL});
        //        selector("BIOMINMAINSTEMSTRUCT", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMINMAINSTEMSTRUCT});
        //        selector("BIOMLEAFMAINSTEMSTRUCT", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMLEAFMAINSTEMSTRUCT});
        //        selector("MAINSTEM_STOCK_IN", artis::kernel::DOUBLE, {
        //                     PlantModel::MAINSTEM_STOCK_IN});
        //        selector("DEADLEAFNB", artis::kernel::INT, {
        //                     PlantModel::DEAD_LEAF_NB});
        //        selector("INTERNODE_LENGTH_MAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::INTERNODE_LENGTH_MAINSTEM});
        //        selector("PANICLEMAINSTEMDW", artis::kernel::DOUBLE, {
        //                     PlantModel::PANICLE_MAINSTEM_DW});
        //        selector("PANICLEDW", artis::kernel::DOUBLE, {
        //                     PlantModel::PANICLE_DW});
        //        selector("PHENOSTAGE", artis::kernel::INT, {
        //                     PlantModel::PHENOSTAGE});
        //        selector("APPSTAGE", artis::kernel::DOUBLE, {
        //                     PlantModel::APPSTAGE});
        //        selector("LIGSTAGE", artis::kernel::DOUBLE, {
        //                     PlantModel::LIGSTAGE});
        //        selector("STOCKIN", artis::kernel::DOUBLE, {
        //                     PlantModel::INTERNODE_STOCK_SUM});
        //        selector("LIG", artis::kernel::DOUBLE, {
        //                     PlantModel::LIG});
        //        selector("APP", artis::kernel::DOUBLE, {
        //                     PlantModel::APP});
        //        selector("NBLEAFMAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::VISI});
        //        selector("BOOL_CROSSED_PLASTO", artis::kernel::DOUBLE, {
        //                     PlantModel::BOOL_CROSSED_PLASTO});
        //        selector("BOOL_CROSSED_LIGULO", artis::kernel::DOUBLE, {
        //                     PlantModel::BOOL_CROSSED_LIGULO});
        //        selector("BOOL_CROSSED_PHYLOO", artis::kernel::DOUBLE, {
        //                     PlantModel::BOOL_CROSSED_PHYLLO});
        //        selector("PANICLENB", artis::kernel::DOUBLE, {
        //                     PlantModel::PANICLENB});
        //        selector("SLAPLANT", artis::kernel::DOUBLE, {
        //                     PlantModel::SLAPLANT});
        //        selector("PLA", artis::kernel::DOUBLE, {
        //                     PlantModel::PAI});
        //        selector("BIOMLEAFTOT", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMLEAFTOT});
        //        selector("BIOMINSHEATHMAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMINSHEATHMS});
        //        selector("BIOMINSHEATH", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMINSHEATH});
        //        selector("BIOMAEROTOT", artis::kernel::DOUBLE, {
        //                     PlantModel::BIOMAEROTOT});
        //        selector("TOTAL_LENGTH_MAINSTEM", artis::kernel::DOUBLE, {
        //                     PlantModel::TOTAL_LENGTH_MAINSTEM});
        //        selector("INTERC1", artis::kernel::DOUBLE, {
        //                     PlantModel::INTERC1});
        //        selector("INTERC2", artis::kernel::DOUBLE, {
        //                     PlantModel::INTERC2});
        //        selector("MS_LEAF2_LEN", artis::kernel::DOUBLE, {
        //                     PlantModel::MS_LEAF2_LEN});

        //        //AssimilationModel
        //        selector("ASSIM", artis::kernel::DOUBLE, {
        //                     PlantModel::ASSIMILATION,
        //                     AssimilationModel::ASSIM});
        //        selector("LAI", artis::kernel::DOUBLE, {
        //                     PlantModel::ASSIMILATION,
        //                     AssimilationModel::LAI});

        //        //StockModel
        //        selector("DAYDEMAND", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::DAY_DEMAND});
        //        selector("IC", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::IC});
        //        selector("TESTIC", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::TEST_IC});
        //        selector("RESERVOIRDISPO", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::RESERVOIR_DISPO});
        //        selector("SEEDRES", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::SEED_RES});
        //        selector("STOCK", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::STOCK});
        //        selector("SUPPLY", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::SUPPLY});
        //        selector("SURPLUS", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::SURPLUS});
        //        selector("DEFICIT", artis::kernel::DOUBLE, {
        //                     PlantModel::STOCK,
        //                     PlantStockModel::DEFICIT});

        //        //RootModel
        //        selector("R_D", artis::kernel::DOUBLE, {
        //                     PlantModel::ROOT,
        //                     RootModel::ROOT_DEMAND_COEF});
        //        selector("ROOTBIOMASS", artis::kernel::DOUBLE, {
        //                     PlantModel::ROOT,
        //                     RootModel::ROOT_BIOMASS});

        //        //Waterbalance
        //        selector("FCSTRI", artis::kernel::DOUBLE, {
        //                     PlantModel::WATER_BALANCE,
        //                     WaterBalanceModel::FCSTRI});
        //        selector("FCSTRL", artis::kernel::DOUBLE, {
        //                     PlantModel::WATER_BALANCE,
        //                     WaterBalanceModel::FCSTRL});
        //        selector("FCSTRA", artis::kernel::DOUBLE, {
        //                     PlantModel::WATER_BALANCE,
        //                     WaterBalanceModel::FCSTRA});
    }

    virtual ~PlantView()
    { }
};

}

#endif
