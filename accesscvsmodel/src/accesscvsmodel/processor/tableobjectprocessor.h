#ifndef TABLEOBJECTPROCESSOR_H
#define TABLEOBJECTPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "cvsmodel/processor/objectprocessor.h"

#include "util/comptr.h"

namespace DAO {
class TableDefs;
}

class TableObjectProcessor : public ObjectProcessor
{
    Q_OBJECT
public:
    explicit TableObjectProcessor(ProjectContainer *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);

    virtual void determineCodecForProject();

protected:
    // Export table structure (.xml) via ExportXML
    bool exportTableStructure(const QString &objectName, const QString &schemaTarget);

    // Export table data (.data) via ExportXML
    bool exportTableData(const QString &objectName, const QString &dataTarget);

    // Import table data (.data) via ImportXML (append mode)
    bool importTableData(const QString &objectName, const QString &dataTarget);

    // Sanitize table structure (*.xmltmp → *.xml)
    bool sanitizeTableStructure(const QString &objectName);

    // Sanitize table data (*.dattmp → *.dat / *.data)
    bool sanitizeTableData(const QString &objectName);

    // Load TableData targets from Setting (common for TableDef / OdbcTable)
    void loadTableDataTargets(Setting *setting);

    QStringList m_tableDataTargets;   // common for TableDef / OdbcTable

    ComPtr<DAO::TableDefs> m_tableDefs;
};

#endif // TABLEOBJECTPROCESSOR_H
