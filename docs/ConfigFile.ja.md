# Configファイルの設定方法

## ファイルの場所・ファイル名

- ファイルの場所 : ソース管理対象ファイルと同じフォルダ
- ファイル名 : <ソール管理対象ファイル名（拡張子含む）>.config

例）
```
C:\MyProjects\MyAccessApp\
├── MyApp.accdb
├── MyApp.accdb.config
```


## Configファイルで設定可能な値

### ProjectPath
- ソースコード・一時フォルダの配置場所。
- ソースコード管理対象ファイルが配置されているフォルダからの相対パスで記述する。
- 既定値 : `.` (ソースコード管理対象ファイルと同じフォルダ)

#### `ProjectPath` の設定例
ソースコード管理対象ファイルが `C:\MyProjects\MyAccessApp\MyApp.accdb` に配置されている場合：

|ProjectPath|実際の値|
|----|----|
|`.` (既定値)|`C:\MyProjects\MyAccessApp`|
|`project`|`C:\MyProjects\MyAccessApp\project`|
|`../project`|`C:\MyProjects\project`|

### SourcePathName
- ソースコードを配置するフォルダ名。
- `ProjectPath` の下に `SourcePathName` という名称のフォルダが作成され、このフォルダの下にソースコードが配置される。
- `ProjectPath` を起点とした、相対パスを記述することも可能。
- 既定値 : `source`

#### `SourcePathName` の設定例
ソースコード管理対象ファイルが `C:\MyProjects\MyAccessApp\MyApp.accdb` に配置され、`ProjectPath` に `.` が設定されている場合

|SourcePathName|実際の値|
|----|----|
|`source` (既定値)|`C:\MyProjects\MyAccessApp\source`|
|`src`|`C:\MyProjects\MyAccessApp\src`|
|`../src`|`C:\MyProjects\src`|


### TempPathName
- 一時ファイルを配置するフォルダ名。
- `ProjectPath` の下に `TempPathName` という名称のフォルダが作成され、このフォルダの下に一時ファイルが配置される。
- このフォルダは、他のソースコード管理対象ファイルと共有は出来ません。複数のファイルをソースコード管理対象とする場合は、それぞれ異なる値を設定して下さい。
- `ProjectPath` を起点とした、相対パスを記述することも可能。
- 既定値 : `.accesscvs`

#### `TempPathName` の設定例
ソースコード管理対象ファイルが `C:\MyProjects\MyAccessApp\MyApp.accdb` に配置され、`ProjectPath` に `.` が設定されている場合

|TempPathName|実際の値|
|----|----|
|`.accesscvs` (既定値)|`C:\MyProjects\MyAccessApp\.accesscvs`|
|`.temp`|`C:\MyProjects\MyAccessApp\.temp`|
|`../.temp`|`C:\MyProjects\.temp`|


## 実際の設定とフォルダ構成

### すべて既定値の場合

#### フォルダ構成
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

### `ProjectPath` のみ指定

#### Config ファイル
```
ProjectPath=../MyProjectSources
```

#### フォルダ構成
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

### `SourcePathName`, `TempPathName` も指定

`SourcePathName` や `TempPathName` に相対パスを指定することで、ソースコードと一時ファイルの配置場所を柔軟に制御できます。

#### Config ファイル
```
ProjectPath=../MyProjectSources
SourcePathName=MyApp.accdb
TempPathName=../.accesscvs/MyApp.accdb
```

#### フォルダ構成
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
