import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: lovViewContainer

    Layout.fillWidth: true
    Layout.fillHeight: true

    TabBar{
        id: tabBar

        function addTabBar(name)
        {
            var newTabBar = Qt.createQmlObject('import QtQuick.Controls 2.5;TabButton{}', tabBar, "dynamicCreation")
            if(newTabBar === null)
            {
                console.error("Create tab bar failed!" + name)
            }
            else
            {
                newTabBar.text = name
            }
        }
    }
    StackLayout{
        id: logViewers
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        function addLogView(category, model)
        {
            var component = Qt.createComponent("LogView.qml")
            var item = component.createObject(logViewers)
            if(item === null)
            {
                console.error("Create LogView failed!" + category)
            }
            else
            {
                item.category = category
                item.logHandler = logToViewerHandler
                item.setLogModel(model)
                item.fillWidth = true
                item.fillHeight = true
            }
        }
    }

    Connections{
        target: logToViewerHandler
        onNewLogModelAttached:{
            tabBar.addTabBar(category)
            logViewers.addLogView(category, model)
        }
    }

    Component.onCompleted: {
        console.log(width + " " + height)
    }
}
