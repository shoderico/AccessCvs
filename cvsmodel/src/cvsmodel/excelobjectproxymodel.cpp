#include "excelobjectproxymodel.h"

ExcelObjectProxyModel::ExcelObjectProxyModel(QObject *parent)
    : ObjectProxyModel(parent)
{
    QList<Model::ObjectType> objectTypes;
    objectTypes
            << Model::Reference
            << Model::VBProject
           ;

    init(objectTypes);
}

