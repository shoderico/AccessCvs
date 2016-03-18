#ifndef CVSMODEL_CONST_H
#define CVSMODEL_CONST_H


namespace Model {
enum ObjectType
{
    Unknwon = 0,
    Unkown_OT = 0,
    TableDef,
    Query,
    Form,
    Report,
    Macro,
    Module,
//    TableData,
    Relation,
    Reference,
    ProjectFile,
    VBProject
};

enum GitStatus
{
    GS_Unknown = 0,
    Unknown_GS = 0,
    Unmodified,
    Modified,
    Added,
    Deleted,
    Renamed,
    Copied,
    UpdatedButMerged,
    Untracked,
    Ignored
};

enum ObjectCheckState
{
    State_NoChange = 0,
    State_On = 1,
    State_Off = 2,
};

enum ObjectExistence
{
    OE_Unchecked = State_NoChange,
    Unchecked_OE = State_NoChange,
    Present = State_On,
    Absent  = State_Off,
};

enum ObjectDifference
{
    OD_Unchecked = State_NoChange,
    Unchecked_OD = State_NoChange,
    DifferentContents = State_On,
    SameContents = State_Off,
};



//----------------------------------------------------------------------------------------------------------------------
// progress notification
enum ProcessType
{
    RefreshProcess,
    ExportProcess,
    ImportProcess,

    LoadItemFromProjectProcess,
    LoadItemFromSourceDirProcess,

    ExportFromProjectToTempDirProcess,
    ImportFromTempDirToProjectProcess,

    CopyFromTempDirToSourceDirProcess,
    CopyFromSourceDirToTempDirProcess,

    SanitizeTempDirProcess,
    DesanitizeTempDirProcess,
    CompareTempDirProcess,

    DeleteFromSourceDirProcess,
    DeleteFromProjectProcess,

    DeleteFromTempDirProcess,

    UpdateItemsDifferenceByFileTimeProcess,
    UpdateFileTimeInTempDirByExportDateProcess,
    UpdateItemsExportDateProcess,
    UpdateFileTimeInTempDirProcess,

    UpdateItemsInProjectProcess,
    UpdateItemsInSourceDirProcess,
    UpdateItemsDifferenceProcess,
    UpdateItemsDifferenceAsIsProcess,
    DeleteItemsProcess,
    UpdateItemsCreateUpdateDateFromProjectProcess,
};







} // namespace Model

#endif // CVSMODEL_CONST_H

