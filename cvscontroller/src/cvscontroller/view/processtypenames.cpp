#include "processtypenames.h"

#include "cvsmodel/objectmodel.h"

ProcessTypeNames::ProcessTypeNames(QObject *parent) : QObject(parent)
{
    m_processTypeNames[ ObjectModel::RefreshProcess ] = tr("RefreshProcess");
    m_processTypeNames[ ObjectModel::ExportProcess ] = tr("ExportProcess");
    m_processTypeNames[ ObjectModel::ImportProcess ] = tr("ImportProcess");
    m_processTypeNames[ ObjectModel::LoadItemFromProjectProcess ] = tr("LoadItemFromProjectProcess");
    m_processTypeNames[ ObjectModel::LoadItemFromSourceDirProcess ] = tr("LoadItemFromSourceDirProcess");
    m_processTypeNames[ ObjectModel::ExportFromProjectToTempDirProcess ] = tr("ExportFromProjectToTempDirProcess");
    m_processTypeNames[ ObjectModel::ImportFromTempDirToProjectProcess ] = tr("ImportFromTempDirToProjectProcess");
    m_processTypeNames[ ObjectModel::CopyFromTempDirToSourceDirProcess ] = tr("CopyFromTempDirToSourceDirProcess");
    m_processTypeNames[ ObjectModel::CopyFromSourceDirToTempDirProcess ] = tr("CopyFromSourceDirToTempDirProcess");
    m_processTypeNames[ ObjectModel::SanitizeTempDirProcess ] = tr("SanitizeTempDirProcess");
    m_processTypeNames[ ObjectModel::DesanitizeTempDirProcess ] = tr("DesanitizeTempDirProcess");
    m_processTypeNames[ ObjectModel::CompareTempDirProcess ] = tr("CompareTempDirProcess");
    m_processTypeNames[ ObjectModel::DeleteFromSourceDirProcess ] = tr("DeleteFromSourceDirProcess");
    m_processTypeNames[ ObjectModel::DeleteFromProjectProcess ] = tr("DeleteFromProjectProcess");
    m_processTypeNames[ ObjectModel::DeleteFromTempDirProcess ] = tr("DeleteFromTempDirProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsDifferenceByFileTimeProcess ] = tr("UpdateItemsDifferenceByFileTimeProcess");
    m_processTypeNames[ ObjectModel::UpdateFileTimeInTempDirByExportDateProcess ] = tr("UpdateFileTimeInTempDirByExportDateProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsExportDateProcess ] = tr("UpdateItemsExportDateProcess");
    m_processTypeNames[ ObjectModel::UpdateFileTimeInTempDirProcess ] = tr("UpdateFileTimeInTempDirProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsInProjectProcess ] = tr("UpdateItemsInProjectProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsInSourceDirProcess ] = tr("UpdateItemsInSourceDirProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsDifferenceProcess ] = tr("UpdateItemsDifferenceProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsDifferenceAsIsProcess ] = tr("UpdateItemsDifferenceAsIsProcess");
    m_processTypeNames[ ObjectModel::DeleteItemsProcess ] = tr("DeleteItemsProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsCreateUpdateDateFromProjectProcess ] = tr("UpdateItemsCreateUpdateDateFromProjectProcess");
}

QString ProcessTypeNames::value(const int type) const
{
    return m_processTypeNames.value(type);
}

