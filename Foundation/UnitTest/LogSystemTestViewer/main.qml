import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.0
import "LogViewer"

Window {
    visible: true
    width: 1600
    height: 900
    title: qsTr("Hello World")
    color: "black"

    ColumnLayout{
        anchors.fill: parent

        Rectangle{
            width: parent.width
            height: 300
        }
        LogViewContainer{
            logToViewerHandler: logHandler

            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: {
                init()
            }
        }
    }
}
