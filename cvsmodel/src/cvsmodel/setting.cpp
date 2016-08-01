#include "setting.h"

#include <QFile>

#include "pch.hpp"

SettingNode::SettingNode(SettingNode::NodeType nodeType)
    : m_nodeType(nodeType)
{
}

bool SettingNode::isKeyValue() const
{
    return (m_nodeType == KeyValue);
}

bool SettingNode::isElement() const
{
    return (m_nodeType == Element);
}

SettingKeyValue *SettingNode::toKeyValue() const
{
    SettingNode *pthis = const_cast<SettingNode*>(this);
    return static_cast<SettingKeyValue*>( pthis );
}

SettingElement *SettingNode::toElement() const
{
    SettingNode *pthis = const_cast<SettingNode*>(this);
    return static_cast<SettingElement*>( pthis );
}

SettingKeyValue::SettingKeyValue(const QString &key, const QVariant &value)
    : SettingNode(KeyValue)
    , m_key(key)
    , m_value(value)
{
}

QString SettingKeyValue::key() const
{
    return m_key;
}

void SettingKeyValue::setKey(const QString &key)
{
    m_key = key;
}

QVariant SettingKeyValue::value() const
{
    return m_value;
}

void SettingKeyValue::setValue(const QVariant &value)
{
    m_value = value;
}

SettingElement::SettingElement(const QString &name)
    : SettingNode(Element)
    , m_name(name)
{
}

SettingElement::~SettingElement()
{
    qDeleteAll(m_nodes);
}

QString SettingElement::name() const
{
    return m_name;
}

void SettingElement::setName(const QString &name)
{
    m_name = name;
}

void SettingElement::append(SettingKeyValue *keyValue)
{
    m_nodes.append(keyValue);
}

void SettingElement::append(SettingElement *element)
{
    m_nodes.append(element);
}

void SettingElement::append(const QString &key, const QVariant &value)
{
    m_nodes.append( new SettingKeyValue(key, value) );
}

SettingElement *SettingElement::append(const QString &elementName)
{
    SettingElement *element = new SettingElement(elementName);
    m_nodes.append(element);
    return element;
}

int SettingElement::count() const
{
    return m_nodes.count();
}

SettingNode *SettingElement::at(const int i) const
{
    return m_nodes.at(i);
}

QVariant SettingElement::value(const QString &key, const QVariant &defaultValue)
{
    for (int i = 0 ; i < m_nodes.count() ; ++i)
    {
        SettingNode *node = m_nodes.at(i);
        if (node->isKeyValue() && node->toKeyValue()->key() == key)
        {
            return node->toKeyValue()->value();
        }
    }
    return defaultValue;
}

const QList<SettingNode *> &SettingElement::nodes() const
{
    return m_nodes;
}

Setting::Setting(const QString &fileName, QTextCodec *codec, const bool bom, const QString &lineEnd)
    : SettingElement("")
    , m_fileName(fileName)
    , m_codec(codec)
    , m_bom(bom)
    , m_lineEnd(lineEnd)
    , m_indent(4)
{
}

bool Setting::load()
{
    QFile file(m_fileName);
    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream stream( &file );
    stream.setCodec( m_codec );
    stream.setGenerateByteOrderMark( m_bom );

    m_regExpBegin.setPattern("^\\s*Begin (.*)$");
    m_regExpEnd.setPattern("^\\s*End$");
    m_regExpKeyValue.setPattern("^\\s*([^=]+) =(.*)$");

    read(this, stream);

    file.close();

    return true;
}

bool Setting::save()
{
    QFile file(m_fileName);
    if (file.exists())
        if (!file.remove())
            return false;

    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream( &file );
    stream.setCodec( m_codec );
    stream.setGenerateByteOrderMark( m_bom );

    write(this, stream);

    file.close();

    return true;
}

void Setting::read(SettingElement *parent, QTextStream &stream)
{
    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        QRegularExpressionMatch matchBegin = m_regExpBegin.match(line);
        QRegularExpressionMatch matchEnd = m_regExpEnd.match(line);
        QRegularExpressionMatch matchKeyValue = m_regExpKeyValue.match(line);

        if (matchBegin.hasMatch())
        {
            QString elementName = matchBegin.captured(1);

            SettingElement *element = parent->append( elementName );
            read(element, stream);
            continue;
        }
        else if (matchEnd.hasMatch())
        {
            return;
        }
        else if (matchKeyValue.hasMatch())
        {
            QString key = matchKeyValue.captured(1);
            QString value = matchKeyValue.captured(2);

            parent->append(key, value);
        }
        else
        {
            Q_ASSERT(false);
        }
    }
}

void Setting::write(SettingElement *parent, QTextStream &stream, int depth)
{
    QString indentString = QString(depth * m_indent, ' ');
    for (int i = 0 ; i < parent->count() ; ++i )
    {
        SettingNode *node = parent->at( i );
        if (node->isElement())
        {
            SettingElement *element = node->toElement();

            stream << QString("%1Begin %2%3")
                      .arg(indentString)
                      .arg( element->name() )
                      .arg(m_lineEnd);

            write(element, stream, depth + 1);

            stream << QString("%1End%2")
                      .arg(indentString)
                      .arg(m_lineEnd);
        }
        else if (node->isKeyValue())
        {
            SettingKeyValue *keyValue = node->toKeyValue();

            stream << QString("%1%2 =%3%4")
                      .arg(indentString)
                      .arg( keyValue->key() )
                      .arg( keyValue->value().toString() )
                      .arg(m_lineEnd);
        }
    }
}
