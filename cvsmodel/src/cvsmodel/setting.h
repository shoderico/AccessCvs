#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QVariant>
#include <QTextStream>
#include <QRegularExpression>

class SettingKeyValue;
class SettingElement;

class SettingNode
{
public:
    enum NodeType
    {
        KeyValue,
        Element,
    };
    SettingNode(NodeType nodeType);
    virtual ~SettingNode(){}

    bool isKeyValue() const;
    bool isElement() const;

    SettingKeyValue *toKeyValue() const;
    SettingElement *toElement() const;

protected:
    NodeType m_nodeType;
};


class SettingKeyValue : public SettingNode
{
public:
    SettingKeyValue(const QString &key, const QVariant &value);

    QString key() const;
    void setKey(const QString &key);

    QVariant value() const;
    void setValue(const QVariant &value);

private:
    QString m_key;
    QVariant m_value;
};


class SettingElement : public SettingNode
{
public:
    SettingElement(const QString &name);
    ~SettingElement();

    QString name() const;
    void setName(const QString &name);

    void append(SettingKeyValue *keyValue);
    void append(SettingElement *element);
    void append(const QString &key, const QVariant &value);
    SettingElement *append(const QString &elementName);

    int count() const;
    SettingNode *at(const int i) const;
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    const QList<SettingNode*> &nodes() const;

private:
    QString m_name;
    QList<SettingNode*> m_nodes;
};


class Setting : public SettingElement
{
public:
    Setting(const QString &fileName, QTextCodec *codec, const bool bom, const QString &lineEnd);

    bool load();
    bool save();

//    SettingElement *root() const = 0; // or Setting is also SettingElement ?

    void setIndent(const int indent);

private:
    void read(SettingElement *parent, QTextStream &stream);
    void write(SettingElement *parent, QTextStream &stream, int depth = 0);

private:
    QString m_fileName;
    QTextCodec *m_codec;
    bool m_bom;
    QString m_lineEnd;
    int m_indent;

    QRegularExpression m_regExpBegin;
    QRegularExpression m_regExpEnd;
    QRegularExpression m_regExpKeyValue;
};

// xml writer/reader : require xml escape... complicated. it cause performance issue.
// vb  writer/reader : no need to escape... simple. fast



#endif // SETTING_H
