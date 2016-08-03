#ifndef EXCELOBJECTPROXYMODEL_H
#define EXCELOBJECTPROXYMODEL_H

#include "excelcvsmodel_global.h"
#include "cvsmodel/objectproxymodel.h"

class EXCELCVSMODEL_SHARED_EXPORT ExcelObjectProxyModel : public ObjectProxyModel
{
    Q_OBJECT
public:
    explicit ExcelObjectProxyModel(QObject *parent = 0);

signals:

public slots:
};

#endif // EXCELOBJECTPROXYMODEL_H
