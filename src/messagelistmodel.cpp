#include "messagelistmodel.h"

MessageListModel::MessageListModel()
{

}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    return m_messages.count();
}

QHash<int, QByteArray> MessageListModel::roleNames() const
{
    return {
        {Roles::Direction, "direction"},
        {Roles::DirectionString, "directionString"},
        {Roles::Payload, "payload"},
        {Roles::Type, "type"},
        {Roles::TypeString, "typeString"},
    };
}

void MessageListModel::append(MessageModel *msg)
{
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append(msg);
    endInsertRows();
}

void MessageListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_messages.size());
    m_messages.clear();
    endRemoveRows();
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > m_messages.count())
        return QVariant();

    auto message = m_messages[index.row()];

    switch (role) {
    case Roles::Direction:
        return message->direction();
    case Roles::DirectionString:
        return message->directionString();
    case Roles::Payload:
        return message->payload();
    case Roles::Type:
        return message->type();
    case Roles::TypeString:
        return message->typeString();
    default:
        return QVariant();
    }
}
