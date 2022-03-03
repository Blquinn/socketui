#ifndef ROOTSTATE_H
#define ROOTSTATE_H

#include <QObject>
#include "messagemodel.h"
#include "websocketclient.h"

class RootState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<MessageModel*> messages READ messages NOTIFY messagesChanged)
    Q_PROPERTY(WebSocketClient* client READ client NOTIFY clientChanged)
public:
    explicit RootState(QObject *parent = nullptr);

    const QList<MessageModel*> &messages() const;
    void appendMessage(MessageModel* msg);
    WebSocketClient *client() const;
public slots:
    void sendTextMessage(QString msg);
    void sendBinaryMessage(QString msg);
signals:
    void messagesChanged();
    void clientChanged();

private:
    QList<MessageModel*> m_messages;
    WebSocketClient *m_client;
};

#endif // ROOTSTATE_H
