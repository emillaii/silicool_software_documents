import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ColumnLayout{
        Button{
            text: "sensorLoader"
            onClicked: {
                sensorLoader.startWriteLog()
            }
        }
        Button{
            text: "sensorPickArm"
            onClicked: {
                sensorPickArm.startWriteLog()
            }
        }
        Button{
            text: "sensorTrayLoader"
            onClicked: {
                sensorTrayLoader.startWriteLog()
            }
        }
    }
}
