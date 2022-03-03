#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QQuickItem>
#include <QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool active READ active NOTIFY stateChanged)
    Q_PROPERTY(SocketState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString stateString READ stateString NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
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

    SocketState state() const;

    QString errorString() const;

    const QString &stateString() const;

public slots:
    void sendTextMessage(QString msg);
    void sendBinaryMessage(QString msg);
    void connect();
    void disconnect();
signals:
    void urlChanged();
    void textMessageReceived(QString msg);
    void binaryMessageReceived(QString b64Message);
    void stateChanged();
    void errorStringChanged();
private slots:
    void onClientError(QAbstractSocket::SocketError error);
private:
    QWebSocket *m_client;
    QString m_url;
};

#endif // WEBSOCKETCLIENT_H
