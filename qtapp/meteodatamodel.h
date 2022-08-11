#ifndef METEODATAMODEL_H
#define METEODATAMODEL_H

#include <QAbstractTableModel>

class MeteoDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MeteoDataModel(xpalm::ModelParameters * parameters, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    Qt::ItemFlags flags(const QModelIndex &index) const;
//    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
//    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
//    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
//     model::models::ModelParameters getParameters();

private:
    xpalm::ModelParameters * parameters;

    QStringList keys;
};

#endif // METEODATAMODEL_H
