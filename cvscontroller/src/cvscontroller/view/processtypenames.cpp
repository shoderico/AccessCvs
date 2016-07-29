#include "processtypenames.h"

#include "cvsmodel/objectmodel.h"

#include "pch.hpp"

ProcessTypeNames::ProcessTypeNames(QObject *parent) : QObject(parent)
{
    m_processTypeNames[ Model::RefreshProcess ] = tr("RefreshProcess");
    m_processTypeNames[ Model::ExportProcess ] = tr("ExportProcess");
    m_processTypeNames[ Model::ImportProcess ] = tr("ImportProcess");
    m_processTypeNames[ Model::LoadItemFromProjectProcess ] = tr("LoadItemFromProjectProcess");
    m_processTypeNames[ Model::LoadItemFromSourceDirProcess ] = tr("LoadItemFromSourceDirProcess");
    m_processTypeNames[ Model::ExportFromProjectToTempDirProcess ] = tr("ExportFromProjectToTempDirProcess");
    m_processTypeNames[ Model::ImportFromTempDirToProjectProcess ] = tr("ImportFromTempDirToProjectProcess");
    m_processTypeNames[ Model::CopyFromTempDirToSourceDirProcess ] = tr("CopyFromTempDirToSourceDirProcess");
    m_processTypeNames[ Model::CopyFromSourceDirToTempDirProcess ] = tr("CopyFromSourceDirToTempDirProcess");
    m_processTypeNames[ Model::SanitizeTempDirProcess ] = tr("SanitizeTempDirProcess");
    m_processTypeNames[ Model::DesanitizeTempDirProcess ] = tr("DesanitizeTempDirProcess");
    m_processTypeNames[ Model::CompareTempDirProcess ] = tr("CompareTempDirProcess");
    m_processTypeNames[ Model::DeleteFromSourceDirProcess ] = tr("DeleteFromSourceDirProcess");
    m_processTypeNames[ Model::DeleteFromProjectProcess ] = tr("DeleteFromProjectProcess");
    m_processTypeNames[ Model::DeleteFromTempDirProcess ] = tr("DeleteFromTempDirProcess");
    m_processTypeNames[ Model::UpdateItemsDifferenceByFileTimeProcess ] = tr("UpdateItemsDifferenceByFileTimeProcess");
    m_processTypeNames[ Model::UpdateFileTimeInTempDirByExportDateProcess ] = tr("UpdateFileTimeInTempDirByExportDateProcess");
    m_processTypeNames[ Model::UpdateItemsExportDateProcess ] = tr("UpdateItemsExportDateProcess");
    m_processTypeNames[ Model::UpdateFileTimeInTempDirProcess ] = tr("UpdateFileTimeInTempDirProcess");
    m_processTypeNames[ Model::UpdateItemsInProjectProcess ] = tr("UpdateItemsInProjectProcess");
    m_processTypeNames[ Model::UpdateItemsInSourceDirProcess ] = tr("UpdateItemsInSourceDirProcess");
    m_processTypeNames[ Model::UpdateItemsDifferenceProcess ] = tr("UpdateItemsDifferenceProcess");
    m_processTypeNames[ Model::UpdateItemsDifferenceAsIsProcess ] = tr("UpdateItemsDifferenceAsIsProcess");
    m_processTypeNames[ Model::DeleteItemsProcess ] = tr("DeleteItemsProcess");
    m_processTypeNames[ Model::UpdateItemsCreateUpdateDateFromProjectProcess ] = tr("UpdateItemsCreateUpdateDateFromProjectProcess");
}

QString ProcessTypeNames::value(const int type) const
{
    return m_processTypeNames.value(type);
}

