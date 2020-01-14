import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: lovViewContainer

    property var logToViewerHandler: null

    function init()
    {
        var defaultCategoryName = logToViewerHandler.getDefaultCategoryName()
        defaultTabButton.text = defaultCategoryName
        defaultTabButton.width = getTextWidth(defaultCategoryName)
        defaultLogView.category = defaultCategoryName
        defaultLogView.logHandler = logToViewerHandler
        defaultLogView.setLogModel(logToViewerHandler.getLogModel(defaultCategoryName))
    }

    function getTextWidth(text)
    {
        textLengthCalculator.text = text
        return textLengthCalculator.width * 1.3 + 6
    }

    Text {
        id: textLengthCalculator
        visible: false
    }

    TabBar{
        id: tabBar

        TabButton{
            id: defaultTabButton
        }

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
                newTabBar.width = getTextWidth(name)
            }
        }
    }

    StackLayout{
        id: logViewers
        anchors.left: parent.left
        anchors.top: tabBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 3
        currentIndex: tabBar.currentIndex

        LogView{
            id: defaultLogView
        }

        function addLogView(category)
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
                item.setLogModel(logToViewerHandler.getLogModel(category))
                if(category === "Warn")
                {
                    item.loglevelSelector.visible = false
                }
            }
        }
    }

    Connections{
        target: logToViewerHandler
        onNewLogModelAttached:{
            tabBar.addTabBar(category)
            logViewers.addLogView(category)
        }
    }
}
