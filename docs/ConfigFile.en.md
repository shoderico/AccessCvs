# How to Configure the Config File

## File Location and Name

- File location: Same folder as the source-controlled file
- File name: <source-controlled file name (including extension)>.config

Example:
```
C:\MyProjects\MyAccessApp\
├── MyApp.accdb
├── MyApp.accdb.config
```


## Configurable Values in the Config File

### ProjectPath
- Location for source code and temporary folders.
- Specify as a relative path from the folder containing the source-controlled file.
- Default value: `.` (same folder as the source-controlled file)

#### Example of `ProjectPath` Settings
When the source-controlled file is located at `C:\MyProjects\MyAccessApp\MyApp.accdb`:

|ProjectPath|Actual Value|
|----|----|
|`.` (default)|`C:\MyProjects\MyAccessApp`|
|`project`|`C:\MyProjects\MyAccessApp\project`|
|`../project`|`C:\MyProjects\project`|

### SourcePathName
- Folder name for placing source code.
- A folder named `SourcePathName` is created under `ProjectPath`, and source code is placed under this folder.
- You can also specify a relative path starting from `ProjectPath`.
- Default value: `source`

#### Example of `SourcePathName` Settings
When the source-controlled file is located at `C:\MyProjects\MyAccessApp\MyApp.accdb` and `ProjectPath` is set to `.`:

|SourcePathName|Actual Value|
|----|----|
|`source` (default)|`C:\MyProjects\MyAccessApp\source`|
|`src`|`C:\MyProjects\MyAccessApp\src`|
|`../src`|`C:\MyProjects\src`|


### TempPathName
- Folder name for placing temporary files.
- A folder named `TempPathName` is created under `ProjectPath`, and temporary files are placed under this folder.
- This folder cannot be shared with other source-controlled files. If multiple files are source-controlled, set different values for each.
- You can also specify a relative path starting from `ProjectPath`.
- Default value: `.accesscvs`

#### Example of `TempPathName` Settings
When the source-controlled file is located at `C:\MyProjects\MyAccessApp\MyApp.accdb` and `ProjectPath` is set to `.`:

|TempPathName|Actual Value|
|----|----|
|`.accesscvs` (default)|`C:\MyProjects\MyAccessApp\.accesscvs`|
|`.temp`|`C:\MyProjects\MyAccessApp\.temp`|
|`../.temp`|`C:\MyProjects\.temp`|


## Actual Settings and Folder Structure

### When All Values Are Default

#### Folder Structure
```
C:/MyProjects/MyAccessApp/
├── MyApp.accdb
├── MyApp.accdb.config
├── .accesscvs/
└── source/
    ├── Project.settings
    ├── TableDef.settings
    ├── OdbcTable.settings
    ├── tabledefs/
    │   └── *.xml
    ├── forms/
    └── ...
```

### When Only `ProjectPath` Is Specified

#### Config File
```
ProjectPath=../MyProjectSources
```

#### Folder Structure
```
C:/MyProjects/MyAccessApp/
├── MyApp.accdb
└── MyApp.accdb.config

C:/MyProjects/MyProjectSources/
├── .accesscvs/
└── source/
    ├── Project.settings
    ├── TableDef.settings
    ├── OdbcTable.settings
    ├── tabledefs/
    │   └── *.xml
    ├── forms/
    └── ...
```

### When `SourcePathName` and `TempPathName` Are Also Specified

By specifying relative paths for `SourcePathName` or `TempPathName`, you can flexibly control the placement of source code and temporary files.

#### Config File
```
ProjectPath=../MyProjectSources
SourcePathName=MyApp.accdb
TempPathName=../.accesscvs/MyApp.accdb
```

#### Folder Structure
```
C:/MyProjects/MyAccessApp/
├── MyApp.accdb
└── MyApp.accdb.config

C:/MyProjects/
├── .accesscvs/
│   └── MyApp.accdb/
└── MyProjectSources/
    └── MyApp.accdb/
        ├── Project.settings
        ├── TableDef.settings
        ├── OdbcTable.settings
        ├── tabledefs/
        │   └── *.xml
        ├── forms/
        └── ...
```