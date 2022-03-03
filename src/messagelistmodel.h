#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include "messagemodel.h"

#include <QAbstractListModel>
#include <QList>

class MessageListModel : public QAbstractListModel
{
public:
    enum Roles {
        Type = Qt::DisplayRole + 1,
        TypeString,
        Payload,
        Direction,
        DirectionString
    };
    Q_ENUM(Roles)

    MessageListModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    void append(MessageModel* msg);
    void clear();
private:
    QList<MessageModel*> m_messages;
};

#endif // MESSAGELISTMODEL_H
