#include "meteodatamodel.h"
#include <QDebug>

MeteoDataModel::MeteoDataModel(xpalm::ModelParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
//    for(auto it = params.getRawParameters().begin(); it != params.getRawParameters().end(); ++it) {
//        keys << QString::fromStdString(it->first);
//    }
//    qDebug() << keys;
}

int MeteoDataModel::rowCount(const QModelIndex &parent) const {
    return  parameters->getMeteoValues()->size();
}
int MeteoDataModel::columnCount(const QModelIndex &parent) const {
    return 8;
}
QVariant MeteoDataModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        xpalm::Climate c = parameters->getMeteoValues()->at(index.row());
        switch(index.column()){
            case 0: return index.row(); break;
            case 1: return c.TMax; break;
            case 2: return c.TMin; break;
            case 3: return c.HMax; break;
            case 4: return c.HMin; break;
            case 5: return c.Vt; break;
            case 6: return c.Rg; break;
            case 7: return c.Rain; break;
        }
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}
QVariant MeteoDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
            case 0: return "Step"; break;
            case 1: return "TMax"; break;
            case 2: return "TMin"; break;
            case 3: return "HMax"; break;
            case 4: return "HMin"; break;
            case 5: return "Vt"; break;
            case 6: return "Rg"; break;
            case 7: return "Rain"; break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
