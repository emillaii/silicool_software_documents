import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.0
import "LogViewer"

Window {
    visible: true
    width: 1000
    height: 700
    title: qsTr("LogViewer")
    color: "black"

    LogViewContainer{
        logToViewerHandler: logHandler

        anchors.fill: parent

        Component.onCompleted: {
            init()
        }
    }
}
