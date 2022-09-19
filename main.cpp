#include <iostream>

using namespace std;

//#include <chrono>
//#include <ctime>
//#include <qmath.h>

#include <observer/PlantView.hpp>
#include <observer/PhytomerView.hpp>

//using namespace artis::kernel;
#include <artis/observer/Output.hpp>
#include <utils/resultparser.h>

typedef artis::observer::Output<artis::utils::DoubleTime,
        ModelParameters> AnOutput;

int main(int argc, char* argv[])
{
    string folder_path = argv[1];

    xpalm::ModelParameters parameters;
    utils::ParametersReader reader;
    reader.loadParametersFromFiles(folder_path, parameters);

    ::Trace::trace().clear();
    GlobalParameters globalParameters;
    XPalmContext context(parameters.get("BeginDate"), parameters.get("EndDate"));
    Tree * m = new Tree;
    XPalmSimulator simulator(m, globalParameters);
    simulator.attachView("plant", new observer::PlantView());
    simulator.attachView("phytomers", new observer::PhytomerView());
    simulator.init(parameters.get("BeginDate"), parameters);
    simulator.run(context);

    AnOutput output(simulator.observer());
    output(".");

#ifdef WITH_TRACE
    trace_model = new VisibleTraceModel(::Trace::trace().elements());
    ui->tableView->setModel(trace_model);
    show_trace();
#endif



    //InputReader inputReader(argv[1]);//argv[1]->metafile
    //inputReader.read();
    //inputReader.configopti=argv[2];//argv[2]->configureDE
    //inputReader.opti=initialiseOptiFromFile(inputReader.configopti.c_str());
    //MultiCalibration<pair<vector<string>,vector<vector<double> > > > multiCal;

    //transformeInputReaderToSamaraMultiCalibration(&inputReader,&multiCal);

    //for(int i=0;i<multiCal.calibration.size();i++)
    //{
        //multiCal.saveEvolutionFile=//un nom;
        //multiCal.calibration[i]->saveFile=//un nom;
    //}

    //multiCal.resolution();

    return 0;
}
