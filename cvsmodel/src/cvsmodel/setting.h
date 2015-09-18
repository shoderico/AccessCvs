#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QVariant>

class SettingKeyValue;
class SettingElement;

class SettingNode
{
public:
    SettingNode();
    virtual ~SettingNode();

    bool isKeyValue() const = 0;
    bool isElement() const = 0;

    SettingKeyValue *toKeyValue() = 0;
    SettingElement *toElement() = 0;
};


class SettingKeyValue : public SettingNode
{
public:
    SettingKeyValue(const QString &key, const QVariant &value);

    QString key() const = 0;
    void setKey(const QString &key) = 0;

    QVariant value() const = 0;
    void setValue(const QVariant &value) = 0;
};


class SettingElement : public SettingNode
{
public:
    SettingElement(const QString &name);

    QString name() const = 0;
    void setName(const QString &name) = 0;

    void append(SettingKeyValue *keyValue) = 0;
    void append(SettingElement *element) = 0;
    void append(const QString &key, const QVariant &value) = 0;
    SettingElement *append(const QString &elementName) = 0;

    int count() const = 0;
    SettingNode *at(const int i) const = 0;
};


class Setting
{
public:
    Setting(const QString &fileName, QTextCodec *codec, const bool bom, const QString &lineEnd);

    bool load() = 0;
    bool save() = 0;

    SettingElement *root() const = 0; // or Setting is also SettingElement ?

    void setIndent(const int indent);

};

// xml writer/reader : require xml escape... complicated. it cause performance issue.
// vb  writer/reader : no need to escape... simple. fast



#endif // SETTING_H
