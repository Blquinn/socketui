#include "messagemodel.h"

MessageModel::MessageModel(MessageType type, const QString &payload, Direction direction, QObject *parent)
    : QObject{parent}
    , m_type(type)
    , m_payload(payload)
    , m_direction(direction)
{
}

MessageModel::MessageType MessageModel::type() const
{
    return m_type;
}

void MessageModel::setType(MessageModel::MessageType newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

MessageModel::Direction MessageModel::direction() const
{
    return m_direction;
}

void MessageModel::setDirection(MessageModel::Direction newDirection)
{
    if (m_direction == newDirection)
        return;
    m_direction = newDirection;
    emit directionChanged();
}

const QString &MessageModel::payload() const
{
    return m_payload;
}

void MessageModel::setPayload(const QString &newPayload)
{
    if (m_payload == newPayload)
        return;
    m_payload = newPayload;
    emit payloadChanged();
}

const QString &MessageModel::typeString() const
{
    static const QString txt = QStringLiteral("txt");
    static const QString bin = QStringLiteral("bin");

    switch (m_type)
    {
    case MessageModel::MessageType::Text:
        return txt;
    default:
        return bin;
    }
}

const QString &MessageModel::directionString() const
{
    static const QString send = QStringLiteral("send");
    static const QString receive = QStringLiteral("recv");
    static const QString system = QStringLiteral("syst");

    switch (m_direction)
    {
    case MessageModel::Direction::Send:
        return send;
    case MessageModel::Direction::Receive:
        return receive;
    default:
        return system;
    }
}
