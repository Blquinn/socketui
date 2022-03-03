#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QObject>

class MessageModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString typeString READ typeString NOTIFY typeChanged)
    Q_PROPERTY(QString payload READ payload WRITE setPayload NOTIFY payloadChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString directionString READ directionString NOTIFY directionChanged)
public:
    enum MessageType
    {
        Text,
        Binary
    };
    Q_ENUM(MessageType)

    enum Direction
    {
        Send,
        Receive,
        System
    };
    Q_ENUM(Direction)

    explicit MessageModel(
            MessageType type,
            const QString &payload,
            Direction direction,
            QObject *parent = nullptr);

    MessageType type() const;
    void setType(MessageType newType);

    Direction direction() const;
    void setDirection(Direction newDirection);

    const QString &payload() const;
    void setPayload(const QString &newPayload);

    const QString &typeString() const;
    const QString &directionString() const;

signals:
    void typeChanged();
    void directionChanged();
    void payloadChanged();
private:
    MessageType m_type;
    Direction m_direction;
    QString m_payload;
};

#endif // MESSAGEMODEL_H
