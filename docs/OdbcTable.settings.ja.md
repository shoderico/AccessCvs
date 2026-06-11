# OdbcTable.settings 仕様

このファイルは、Access の ODBC リンクテーブル（外部データソースに接続されたテーブル）を CVS 管理の対象にするかどうかを制御する設定ファイルです。

## ファイルの場所

プロジェクトのルートディレクトリに配置します。

```
<プロジェクトルート>/
├── OdbcTable.settings   ← このファイル
├── odbctables/          ← 出力先フォルダ
└── ...
```

## 書式

UTF-8（BOM なし）、改行コードは CRLF（`\r\n`）です。

キーと値は `key =value` の形式で記述します。複数のテーブルを指定する場合は、`Begin TableData ... End` のブロックで囲みます。

```
ToBeManaged =true

Begin TableData
    TableName =T_ExternalCustomers
    TableName =T_ExternalOrders
End
```




## 設定項目

### ToBeManaged

ODBC リンクテーブルを CVS 管理の対象にするかどうかを指定します。

| 値     | 意味                                   |
|--------|----------------------------------------|
| `true` | ODBC リンクテーブルを出力・管理する    |
| `false`（デフォルト） | ODBC リンクテーブルは無視する |


### TableData セクション（データ出力対象）

`TableData` セクションを追加することで、ODBC リンクテーブルの **テーブルデータ（.dat）** を出力対象にできます。

```
Begin TableData
    TableName =T_ExternalCustomers
    TableName =T_ExternalOrders
End
```

| キー        | 意味                                           |
|-------------|------------------------------------------------|
| `TableName` | データを出力する ODBC テーブルの名前           |

- このセクションを指定しない場合、データ（.dat）は出力されません。
- ODBC プロパティ（.odbc）と 構造（.xml）は常に Export の対象となります。




## デフォルト動作

この設定ファイルが存在しない場合、または `ToBeManaged` が指定されていない場合は、**`false`（無視）** として扱われます。

つまり、**デフォルトでは ODBC リンクテーブルの情報は一切出力されません**。

また、`TableData` セクションが空の場合は、**データ出力は行われません**。

ODBC プロパティ（.odbc）と テーブル構造（`.xml`）は常に CVS 管理の対象となりますが、**テーブルデータ（`.dat`）は明示的に指定したテーブルにのみ出力**されます。



## 出力先フォルダ

**`odbctables/`** フォルダに出力されます。




## 出力される情報

`ToBeManaged: true` の場合、ODBC リンクテーブルごとに以下の **3 種類** のファイルが出力されます。

### ファイルの種類と拡張子

| ファイル種別     | 拡張子   | 内容                                           | 出力条件                     |
|------------------|----------|------------------------------------------------|------------------------------|
| ODBC プロパティ  | `.odbc`  | 接続情報（Setting 形式）                       | 常に                         |
| テーブル構造     | `.xml`   | テーブル構造（ExportXML）                      | 常に（Export のみ）          |
| テーブルデータ   | `.dat`   | テーブルデータ（ExportXML）                    | `TableData` セクションで指定されたテーブルのみ |

### ファイル名例

```
Customers.odbc
Customers.xml
Customers.dat     ← TableData で指定している場合のみ
```

### ODBC プロパティ（.odbc）の内容

```
Begin TableDef
    Connect =ODBC;DSN=...;UID=...;PWD=...
    SourceTableName =元のテーブル名
    Attributes =0
End
```

#### 各フィールドの意味

| フィールド          | 内容                                           |
|---------------------|------------------------------------------------|
| `Connect`           | ODBC 接続文字列（`ODBC;` で始まる）            |
| `SourceTableName`   | リンク先の元のテーブル名                       |
| `Attributes`        | DAO の Attributes 値（`dbAttachSavePWD` など） |


### テーブル構造（.xml）とデータ（.dat）

- `.xml` は `ExportXML` で出力され、`TableDefSanitizer` でサニタイズされます。
- `.dat` は `TableData` セクションで指定されたテーブルに対してのみ出力されます。
- **構造（.xml）は Export のみ**で、Import 時には使用されません（Export-only）。






## CVS からインポートしたときの動作

`ToBeManaged =true` にしている場合、CVS からプロジェクトに取り込む（インポートする）ときに、以下のような処理が行われます：

1. プロジェクト内に同じ名前のテーブルがすでに存在する場合、一度削除されます。
2. ODBC リンクテーブルとして新しく作成され、保存されていた接続情報（接続文字列や元のテーブル名など）が設定されます。
3. 同じ名前の `.dat` ファイルが存在し、かつそのテーブルが `TableData` セクションで指定されている場合、データが追記されます。
4. `.xml` ファイルは Import 時には使用されません（構造の復元は行われません）。


## 通常の TableDef との違い

| 項目               | TableDefProcessor（通常テーブル） | OdbcTableProcessor（ODBC リンク） |
|--------------------|-----------------------------------|-----------------------------------|
| デフォルトで処理   | する                              | **しない**                        |
| 設定ファイル       | `TableDef.settings`               | `OdbcTable.settings`              |
| 出力フォルダ       | `tabledefs/`                      | `odbctables/`                     |
| 出力形式           | `.xml` + `.dat`                   | `.odbc` + `.xml` + `.dat`        |
| テーブル構造出力   | 可能                              | 可能（Export のみ）               |
| データ出力         | 可能（`.dat`、TableData 指定時） | 可能（`.dat`、TableData 指定時） |
| 構造の Import      | 可能                              | 不可（Export-only）               |

## 注意点

- ODBC リンクテーブルは外部データソースへの依存が強いため、誤って CVS 管理下に置かないようデフォルトでオフになっています。
- 接続文字列にパスワードが含まれる場合、`Attributes` に `dbAttachSavePWD` がセットされている可能性があります。
- 構造（.xml）は Export のみで、Import 時には使用されません（Export-only）。接続情報（.odbc）からリンクを再作成します。
- データ（.dat）は `TableData` セクションで指定したテーブルのみ出力・インポートされます。
- 構造やデータの変更を追跡したい場合は、別途 `TableDef` としてローカルテーブルにインポートするか、外部ツールで管理することを検討してください。
