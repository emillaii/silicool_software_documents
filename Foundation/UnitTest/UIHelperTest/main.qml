import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    SilicolMsgBox{
        id: silicolMsgBox

        Component.onCompleted: {
            uiOperation.setGrabMouseItem(silicolMsgBox.contentItem)
        }
    }

    Connections{
        target: msgBoxModel
        onMsgBoxCountChanged:{
            if(count > 0)
            {
                silicolMsgBox.msgBoxCount = count
                silicolMsgBox.open()
            }
            else{
                silicolMsgBox.close()
            }
        }
    }

    ColumnLayout{
        RowLayout{
            Label{
                text: "uiHelperTest"
            }
            Button{
                text: "showRandanMsgBox"
                onClicked: {
                    uiHelperTest.showRandanMsgBox()
                }
            }
            Button{
                text: "getRandanUIRsp"
                onClicked: {
                    uiHelperTest.getRandanUIRsp()
                }
            }
        }
        RowLayout{
            Label{
                text: "uihelperThdTest"
            }
            Button{
                text: "showRandanMsgBox"
                onClicked: {
                    uihelperThdTest.showRandanMsgBox()
                }
            }
            Button{
                text: "getRandanUIRsp"
                onClicked: {
                    uihelperThdTest.getRandanUIRsp()
                }
            }
        }
    }
}
