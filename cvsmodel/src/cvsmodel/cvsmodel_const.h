#ifndef CVSMODEL_CONST_H
#define CVSMODEL_CONST_H

#include <QFlag>

namespace Model {

//----------------------------------------------------------------------------------------------------------------------
enum SelectObjectType
{
  //NoObjectTypes = 0,
    TableDefObjectType  = 0x1,
    QueryObjectType     = 0x2,
    FormObjectType      = 0x4,
    ReportObjectType    = 0x8,
    MacroObjectType     = 0x10,
    ModuleObjectType    = 0x20,
    ReferenceObjectType = 0x40,
    ProjectFileType     = 0x80,
    VBProjectType       = 0x100,
    AllObjectTypes
        = TableDefObjectType
        | QueryObjectType
        | FormObjectType
        | ReportObjectType
        | MacroObjectType
        | ModuleObjectType
        | ReferenceObjectType
        | ProjectFileType
        | VBProjectType
        ,
    RelationObjectType = -1, // unused
};
Q_DECLARE_FLAGS(SelectObjectTypes, SelectObjectType)


//----------------------------------------------------------------------------------------------------------------------
enum ObjectType
{
    Unknwon = 0,
    Unkown_OT = 0,
    TableDef = TableDefObjectType,
    Query = QueryObjectType,
    Form = FormObjectType,
    Report = ReportObjectType,
    Macro = MacroObjectType,
    Module = ModuleObjectType,
    Relation = RelationObjectType,
    Reference = ReferenceObjectType,
    ProjectFile = ProjectFileType,
    VBProject = VBProjectType,
};


//----------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------
enum ObjectCheckState
{
    State_NoChange = 0,
    State_On = 1,
    State_Off = 2,
};

//----------------------------------------------------------------------------------------------------------------------
enum ObjectExistence
{
    OE_Unchecked = State_NoChange,
    Unchecked_OE = State_NoChange,
    Present = State_On,
    Absent  = State_Off,
};

//----------------------------------------------------------------------------------------------------------------------
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


//----------------------------------------------------------------------------------------------------------------------
enum Column
{
    NameColumn = 0,
    HasDataColumn,
    InProjectColumn,
    InSourceDirColumn,
    DifferentColumn,
    CreateDateColumn,
    UpdateDateColumn,
    ExportDateColumn,
    ObjectTypeColumn,
    ColumnCount
};


//----------------------------------------------------------------------------------------------------------------------
enum ItemsType
{
  //NoItems          = 0,
    InBoth           = 1,
    InBoth_Different = 2,
    InBoth_Same      = 4,
    InBoth_NotSame   = 8,
    InProjectOnly    = 16,
    InSourceDirOnly = 32,
    AllItems         = 63,
};
Q_DECLARE_FLAGS(ItemsTypes, ItemsType)


//----------------------------------------------------------------------------------------------------------------------
enum ObjectDifferenceType
{
    SameContentsType = 1,
    DifferentContentsTypes = 2,
    AllDifferenceTypes = 3,
};
Q_DECLARE_FLAGS(ObjectDifferenceTypes, ObjectDifferenceType)



} // namespace Model

Q_DECLARE_OPERATORS_FOR_FLAGS(Model::ItemsTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(Model::SelectObjectTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(Model::ObjectDifferenceTypes)

#endif // CVSMODEL_CONST_H

