import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Layouts
import QtWebSockets

import me.blq.socketui.components

ApplicationWindow {
    id: win
    width: 640
    height: 480
    minimumWidth: 400
    minimumHeight: 300
    visible: true
    title: qsTr("SocketUI")

    Material.theme: Material.Dark
    Material.accent: Material.LightBlue

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 8
        anchors {
            leftMargin: 8
            rightMargin: 8
        }

        RowLayout {
            id: connectionRow
            Layout.fillWidth: true

            TextField {
                id: urlText
                Layout.fillWidth: true
                placeholderText: qsTr("Websocket URL (Must start with ws:// or wss://)")
                text: rootState.client.url
                onTextChanged: rootState.client.url = text
                selectByMouse: true
                onAccepted: if (!rootState.client.active)
                                rootState.client.connect()
            }

            Button {
                id: connectButton
                text: !rootState.client.active ? qsTr("Connect") : qsTr("Disconnect")
                onPressed: rootState.client.active ? rootState.client.disconnect() : rootState.client.connect()
            }
        }

        RowLayout {
            Layout.fillWidth: true
            RowLayout {
                Layout.alignment: Qt.AlignLeading
                Label {
                    text: qsTr("Connection Status:")
                }
                Label {
                    text: rootState.client.stateString
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                Layout.alignment: Qt.AlignTrailing
                text: qsTr("Clear")
                onPressed: rootState.clearMessages()

                ToolTip.text: qsTr("Clear the messages list.")
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }
        }

        ListView {
            id: listView

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            boundsBehavior: Flickable.StopAtBounds
            model: rootState.messages

            delegate: Rectangle {
                width: listView.width
                height: rowLayout.height

                color: mouseArea.containsMouse ? Material.listHighlightColor : "transparent"

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    propagateComposedEvents: true
                    hoverEnabled: true
                }

                RowLayout {
                    id: rowLayout
                    width: listView.width
                    spacing: 8
                    Label {
                        function dirColor(dir) {
                            switch (dir) {
                            case MessageModel.Send:
                                return Material.primaryColor
                            case MessageModel.Receive:
                                return Material.accentColor
                            default:
                                return Material.iconColor
                            }
                        }

                        font.family: monospaceFont
                        Layout.alignment: Qt.AlignTop
                        color: dirColor(direction)
                        text: `[${directionString}]`
                    }
                    Label {
                        font.family: monospaceFont
                        Layout.alignment: Qt.AlignTop
                        text: direction
                              !== MessageModel.System ? qsTr(`(${typeString})`) : `     `
                    }
                    Label {
                        Layout.fillWidth: true
                        font.family: monospaceFont
                        text: payload
                        wrapMode: Text.Wrap
                    }
                }
            }

            onCountChanged: if (listView.atYEnd) Qt.callLater(listView.positionViewAtEnd)

            ScrollBar.vertical: ScrollBar {}
        }

        RowLayout {
            id: msgRow
            Layout.fillWidth: true

            function send() {
                if (messageTypeCombo.currentValue === MessageModel.Binary) {
                    rootState.sendBinaryMessage(sendMessageText.text)
                } else {
                    rootState.sendTextMessage(sendMessageText.text)
                }

                sendMessageText.text = ''
            }

            // Move this state into cpp
            ComboBox {
                id: messageTypeCombo
                valueRole: "value"
                textRole: "text"
                model: [{
                        "value": MessageModel.Text,
                        "text": qsTr('Text')
                    }, {
                        "value": MessageModel.Binary,
                        "text": qsTr('Binary')
                    }]
            }
            TextField {
                // Move this state into cpp (text)
                id: sendMessageText
                Layout.fillWidth: true
                placeholderText: qsTr("Message Text")
                selectByMouse: true
                onAccepted: if (rootState.client.state === WebSocketClient.ConnectedState)
                                msgRow.send()
            }
            Button {
                id: sendButton
                enabled: rootState.client.state === WebSocketClient.ConnectedState
                text: qsTr("Send")
                onPressed: msgRow.send()
            }
        }
    }
}
