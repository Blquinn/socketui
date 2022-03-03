#include "websocketclient.h"

#include <QStringLiteral>

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
    , m_client(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))
    , m_url("")
    , m_active(false)
    , m_state((SocketState) m_client->state())
{
//    QObject::connect(m_client, &QWebSocket::stateChanged, this, &WebSocketClient::setState);
    QObject::connect(m_client, &QWebSocket::stateChanged, this, [this](QAbstractSocket::SocketState state){
        setState((WebSocketClient::SocketState) state);
    });

    QObject::connect(m_client, &QWebSocket::binaryMessageReceived, this, [this](const QByteArray &arr) {
        qDebug() << "Got binary ws message" << arr.toBase64();
        emit binaryMessageReceived(QString::fromUtf8(arr.toBase64()));
    });

    QObject::connect(m_client, &QWebSocket::textMessageReceived, this, [this](const QString &str) {
        qDebug() << "Got text ws message" << str;
        emit textMessageReceived(str);
    });
    // TODO: Get errors
//    QObject::connect(m_client, &QWebSocket::error, this, &WebSocketClient::onClientError);
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
    return m_active;
}

void WebSocketClient::setActive(bool newActive)
{
    if (m_active == newActive)
        return;

    // Connect to url
    if (canConnect()) {
        m_client->open(m_url);
    } else {
        m_client->close();
    }

    m_active = newActive;
    emit activeChanged();
}

const WebSocketClient::SocketState &WebSocketClient::state() const
{
    return m_state;
}

void WebSocketClient::setState(const SocketState &newState)
{
    auto ns = (SocketState) newState;
    if (m_state == ns)
        return;

    m_state = ns;
    emit stateChanged();
}

const QString &WebSocketClient::errorString() const
{
    return m_errorString;
}

void WebSocketClient::setErrorString(const QString &newErrorString)
{
    if (m_errorString == newErrorString)
        return;
    m_errorString = newErrorString;
    emit errorStringChanged();
}

void WebSocketClient::onClientError(QAbstractSocket::SocketError error)
{
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

    switch (m_state)
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

bool WebSocketClient::canConnect() const
{
    return m_state == UnconnectedState;
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
