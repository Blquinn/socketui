#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QQuickItem>
#include <QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(SocketState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString stateString READ stateString NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)
    Q_PROPERTY(bool canConnect READ canConnect NOTIFY stateChanged)
public:

    // Copied from QAbstractSocket::SocketState
    enum SocketState {
        UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ListeningState,
        ClosingState
    };
    Q_ENUM(SocketState)

    WebSocketClient(QObject *parent = nullptr);

    const QString &url() const;
    void setUrl(const QString &newUrl);

    bool active() const;
    void setActive(bool newActive);

    const SocketState &state() const;
    void setState(const SocketState &newState);

    const QString &errorString() const;
    void setErrorString(const QString &newErrorString);

    const QString &stateString() const;

    bool canConnect() const;

public slots:
    void sendTextMessage(QString msg);
    void sendBinaryMessage(QString msg);
signals:
    void urlChanged();
    void activeChanged();
    void textMessageReceived(QString msg);
    void binaryMessageReceived(QString b64Message);
    void stateChanged();
    void errorStringChanged();
private slots:
    void onClientError(QAbstractSocket::SocketError error);
private:
    QWebSocket *m_client;
    QString m_url;
    bool m_active;
    SocketState m_state;
    QString m_errorString;
    QString m_stateString;
    bool m_canConnect;
};

#endif // WEBSOCKETCLIENT_H
