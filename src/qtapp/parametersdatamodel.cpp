#include "parametersdatamodel.h"
#include <QDebug>
ParametersDataModel::ParametersDataModel(xpalm::ModelParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
    for(auto it = params->getRawParameters()->begin(); it != params->getRawParameters()->end(); ++it) {
        keys << QString::fromStdString(it->first);
    }
}

int ParametersDataModel::rowCount(const QModelIndex &parent) const {
    return keys.size();
}
int ParametersDataModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant ParametersDataModel::data(const QModelIndex &index, int role) const{
    if(index.column() == 0 && role == Qt::DisplayRole)
        return keys[index.row()];


    if(index.column() == 1 && (role == Qt::DisplayRole || role == Qt::EditRole)) {
//        try
//        {
          return parameters->get(keys[index.row()].toStdString());

//        }
//        catch(...) {
//            return QString::fromStdString(parameters->get<std::string>(keys[index.row()].toStdString()));
//        }

    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}
QVariant ParametersDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
        case 0:return "Name"; break;
        case 1: return "Value"; break;
        }


    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags ParametersDataModel::flags(const QModelIndex &index) const {
    if(index.column() == 1)
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index);
}

bool ParametersDataModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    if(role != Qt::EditRole)
        return false;
    std::string key = data(index(idx.row(),0), Qt::DisplayRole).toString().toStdString();
    bool ok;
    double d = value.toDouble(&ok);
    if(ok)
        parameters->mParams[key] = d;
    return ok;
}
