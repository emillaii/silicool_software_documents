import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

RowLayout {
    implicitWidth: 400
    implicitHeight: 300

    property string category: ""
    property var logHandler: null

    function setLogModel(model)
    {
        logView.model = model
    }

    Rectangle{
        id: loglevelSelector

        height: parent.height
        border.width: 1
        width: 80
        color: "transparent"

        ColumnLayout{
            anchors.fill: parent

            RadioButton{
                text: qsTr("Debug")
                onClicked: {
                    logHandler.setLogLevel(category, 0)
                }
            }
            RadioButton{
                text: qsTr("Info")
                onClicked: {
                    logHandler.setLogLevel(category, 1)
                }
            }
            RadioButton{
                text: qsTr("Warn")
                onClicked: {
                    logHandler.setLogLevel(category, 2)
                }
            }
        }
    }

    Rectangle{
        Layout.fillWidth: true
        height: parent.height

        border.width: 1
        color: "transparent"

        ListView{
            id: logView
            anchors.fill: parent

            clip: true

            delegate: Text {
                width: parent.width - 3
                color: logColor
                text: logStr
                wrapMode: Text.Wrap
                lineHeight: 1.2
                lineHeightMode: Text.ProportionalHeight
            }

            MouseArea{
                z: 1
                anchors.fill: parent
                onDoubleClicked: {
                    model.onClearLog()
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                z: 2
                width: 15
            }

            MouseArea{
                id: logViewMouseArea
                z: 3
                anchors.fill: parent
                hoverEnabled: true
                onPressed: {
                    mouse.accepted = false
                }
            }
        }
    }

    Timer {
        id: scrollTimer;
        repeat: true;
        interval: 200;
        onTriggered: {
            if(!logViewMouseArea.containsMouse)
            {
                logView.positionViewAtEnd()
            }
        }
    }

    Component.onCompleted: {
        scrollTimer.start()
    }
}

