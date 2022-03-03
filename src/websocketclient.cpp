#include "websocketclient.h"

#include <QStringLiteral>

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
    , m_client(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))
    , m_url("")
{
    QObject::connect(m_client, &QWebSocket::stateChanged, this, [this](QAbstractSocket::SocketState state){
        emit stateChanged();
    });
    QObject::connect(m_client, &QWebSocket::binaryMessageReceived, this, [this](const QByteArray &arr) {
        qDebug() << "Got binary ws message" << arr.toBase64();
        emit binaryMessageReceived(QString::fromUtf8(arr.toBase64()));
    });
    QObject::connect(m_client, &QWebSocket::textMessageReceived, this, [this](const QString &str) {
        qDebug() << "Got text ws message" << str;
        emit textMessageReceived(str);
    });
    QObject::connect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onClientError(QAbstractSocket::SocketError)));
}

const QString &WebSocketClient::url() const
{
    return m_url;
}

void WebSocketClient::setUrl(const QString &newUrl)
{
    if (m_url == newUrl)
        return;
    m_url = newUrl;
    emit urlChanged();
}

bool WebSocketClient::active() const
{
    return m_client->state() != QAbstractSocket::UnconnectedState;
}

WebSocketClient::SocketState WebSocketClient::state() const
{
    return (SocketState) m_client->state();
}

QString WebSocketClient::errorString() const
{
    return m_client->errorString();
}

void WebSocketClient::onClientError(QAbstractSocket::SocketError error)
{
    emit errorStringChanged();
}

const QString &WebSocketClient::stateString() const
{
    static const QString unconnected = QStringLiteral("Unconnected");
    static const QString hostLookup = QStringLiteral("Host Lookup");
    static const QString connecting = QStringLiteral("Connecting");
    static const QString connected = QStringLiteral("Connected");
    static const QString bound = QStringLiteral("Bound");
    static const QString listening = QStringLiteral("Listening");
    static const QString closing = QStringLiteral("Closing");
    static const QString unknown = QStringLiteral("Unknown");

    switch (state())
    {
    case UnconnectedState:
        return unconnected;
    case HostLookupState:
        return hostLookup;
    case ConnectingState:
        return connecting;
    case ConnectedState:
        return connected;
    case BoundState:
        return bound;
    case ListeningState:
        return listening;
    case ClosingState:
        return closing;
    default:
        return unknown;
    }
}

void WebSocketClient::sendTextMessage(QString msg)
{
    m_client->sendTextMessage(msg);
}

void WebSocketClient::sendBinaryMessage(QString msg)
{
    auto bts = QByteArray::fromBase64(msg.toUtf8());
    m_client->sendBinaryMessage(bts);
}

void WebSocketClient::connect()
{
    if (active())
        return;

    m_client->open(m_url);
}

void WebSocketClient::disconnect()
{
    if (!active())
        return;

    m_client->close();
}
