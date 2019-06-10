import QtQuick 2.12
import QtQuick.Controls 2.4

Item {
    id: app
    anchors.fill: parent
    opacity: 0.0

    Behavior on opacity { NumberAnimation { duration: 500 } }

    property var lastPages: []
    property int __currentIndex: 0

    function init()
    {
        opacity = 1.0
        showPage("TablesList.qml")
    }

    function prevPage()
    {
        lastPages.pop()
        pageLoader.setSource(lastPages[lastPages.length-1])
        __currentIndex = lastPages.length-1;
    }

    function showPage(name)
    {
        lastPages.push(name)
        pageLoader.setSource(name)
        if(name === "TablesList.qml")
            __currentIndex = 0;
        if(name === "OrderList.qml")
            __currentIndex = 1
        if(name === "BeverageList.qml")
            __currentIndex = 2;
    }

    TitleBar {
        id: titleBar
        currentIndex: __currentIndex

        onTitleClicked: {
//            if (index < __currentIndex)
//                pageLoader.item.close()
            if (index === 0)
                showPage("TablesList.qml")
            if(index === 1)
                showPage("OrderList.qml")
            if (index === 2)
                showPage("BeverageList.qml")
        }
    }

    Loader {
        id: pageLoader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom

        onStatusChanged: {
            if (status === Loader.Ready)
            {
                pageLoader.item.init();
                pageLoader.item.forceActiveFocus()
            }
        }
    }

    Keys.onReleased: {
        switch (event.key) {
        case Qt.Key_Escape:
        case Qt.Key_Back: {
            if (__currentIndex > 0) {
                pageLoader.item.close()
                event.accepted = true
            } else {
                Qt.quit()
            }
            break;
        }
        default: break;
        }
    }

//    BluetoothAlarmDialog {
//        id: btAlarmDialog
//        anchors.fill: parent
//        visible: !connectionHandler.alive
//    }
}
