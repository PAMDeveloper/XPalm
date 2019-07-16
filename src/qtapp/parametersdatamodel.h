#ifndef PARAMETERSDATAMODEL_H
#define PARAMETERSDATAMODEL_H

#include <QAbstractTableModel>

class ParametersDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ParametersDataModel(xpalm::ModelParameters * parameters, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

private:
    xpalm::ModelParameters * parameters;
    QStringList keys;
};

#endif // PARAMETERSDATAMODEL_H
