#include "accessobjectproxymodel.h"

#include "cvsmodel_const.h"

AccessObjectProxyModel::AccessObjectProxyModel(QObject *parent)
    : ObjectProxyModel(parent)
{
    QList<Model::ObjectType> objectTypes;
    objectTypes
            << Model::TableDef
            << Model::OdbcTable
            << Model::Query
            << Model::Form
            << Model::Report
            << Model::Macro
            << Model::Module
            << Model::Reference
            << Model::ProjectFile
            << Model::VBProject
           ;

    init(objectTypes);
}

