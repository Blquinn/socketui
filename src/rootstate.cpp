#include "rootstate.h"

RootState::RootState(QObject *parent)
    : QObject{parent}
    , m_client(new WebSocketClient(this))
    , m_messages(new MessageListModel())
{
    QObject::connect(m_client, &WebSocketClient::textMessageReceived, this, [this](const QString &msg) {
        auto msgModel = new MessageModel(
                              MessageModel::MessageType::Text,
                              msg,
                              MessageModel::Direction::Receive,
                              this);
        appendMessage(msgModel);
    });
    QObject::connect(m_client, &WebSocketClient::binaryMessageReceived, this, [this](const QString &msg) {
        auto msgModel = new MessageModel(
                              MessageModel::MessageType::Binary,
                              msg,
                              MessageModel::Direction::Receive,
                              this);
        appendMessage(msgModel);
    });
    QObject::connect(m_client, &WebSocketClient::stateChanged, this, [this]() {
        auto msgModel = new MessageModel(
                              MessageModel::MessageType::Binary,
                              QString("State changed to %1").arg(m_client->stateString()),
                              MessageModel::Direction::System,
                              this);
        appendMessage(msgModel);
    });
    QObject::connect(m_client, &WebSocketClient::errorStringChanged, this, [this]() {
        auto msgModel = new MessageModel(
                              MessageModel::MessageType::Binary,
                              QString("Socket Error: %1").arg(m_client->errorString()),
                              MessageModel::Direction::System,
                              this);
        appendMessage(msgModel);
    });
}

MessageListModel * RootState::messages() const
{
    return m_messages;
}

void RootState::appendMessage(MessageModel* msg)
{
    m_messages->append(msg);
}

WebSocketClient *RootState::client() const
{
    return m_client;
}

void RootState::sendTextMessage(QString msg)
{
        auto msgModel = new MessageModel(
                              MessageModel::MessageType::Text,
                              msg,
                              MessageModel::Direction::Send,
                              this);
        appendMessage(msgModel);
        m_client->sendTextMessage(msg);
}

void RootState::sendBinaryMessage(QString msg)
{
        auto msgModel = new MessageModel(
                              MessageModel::MessageType::Binary,
                              msg,
                              MessageModel::Direction::Send,
                              this);
        appendMessage(msgModel);
        m_client->sendBinaryMessage(msg);
}

void RootState::clearMessages()
{
    m_messages->clear();
}
