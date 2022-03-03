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

            // TODO: Move to cpp
            function connect() {
                if (rootState.client.canConnect) {
                    rootState.client.url = urlText.text
                    rootState.client.active = true
                } else {
                    rootState.client.active = false
                }
            }

            TextField {
                id: urlText
                Layout.fillWidth: true
                placeholderText: qsTr("Websocket URL (Must start with ws:// or wss://)")
                text: 'ws://localhost:10000/'
                selectByMouse: true
                onAccepted: if (rootState.client.canConnect)
                                connectionRow.connect()
            }

            Button {
                id: connectButton
                text: rootState.client.canConnect ? qsTr("Connect") : qsTr(
                                                        "Disconnect")
                onPressed: connectionRow.connect()
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
                onPressed: messages.clear()

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
                        color: dirColor(modelData.direction)
                        text: `[${modelData.directionString}]`
                    }
                    Label {
                        font.family: monospaceFont
                        Layout.alignment: Qt.AlignTop
                        text: modelData.direction
                              !== MessageModel.System ? qsTr(`(${modelData.typeString})`) : `     `
                    }
                    Label {
                        Layout.fillWidth: true
                        font.family: monospaceFont
                        text: modelData.payload
                        wrapMode: Text.Wrap
                    }
                }
            }

            // TODO: Only scroll down if view is already positioned at the end.
            onCountChanged: Qt.callLater(listView.positionViewAtEnd)

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
