#ifndef ROOTSTATE_H
#define ROOTSTATE_H

#include <QObject>
#include "messagelistmodel.h"
#include "messagemodel.h"
#include "websocketclient.h"

class RootState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageListModel* messages READ messages NOTIFY messagesChanged)
    Q_PROPERTY(WebSocketClient* client READ client NOTIFY clientChanged)
public:
    explicit RootState(QObject *parent = nullptr);

    MessageListModel* messages() const;
    void appendMessage(MessageModel* msg);
    WebSocketClient *client() const;
public slots:
    void sendTextMessage(QString msg);
    void sendBinaryMessage(QString msg);
    void clearMessages();
signals:
    void clientChanged();
    void messagesChanged();
private:
    MessageListModel* m_messages;
    WebSocketClient *m_client;
};

#endif // ROOTSTATE_H
