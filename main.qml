import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.2
import "."

ApplicationWindow {
    id: wroot
    visible: true
    width: 720 * .7
    height: 1240 * .7
    title: qsTr("Just One More Beer Please")
    color: GameSettings.backgroundColor

    Component.onCompleted: {
        GameSettings.wWidth = Qt.binding(function() {return width})
        GameSettings.wHeight = Qt.binding(function() {return height})
    }


    Loader {
        id: splashLoader
        anchors.fill: parent
        source: "SplashScreen.qml"
        asynchronous: false
        visible: true

        onStatusChanged: {
            if (status === Loader.Ready) {
                appLoader.setSource("App.qml");
            }
        }
    }

    Connections {
        target: splashLoader.item
        onReadyToGo: {
            splashLoader.visible = false
            splashLoader.setSource("")
            appLoader.visible = true
            appLoader.item.init()
            appLoader.item.forceActiveFocus();
        }
    }

    Loader {
        id: appLoader
        anchors.fill: parent
        visible: false
        asynchronous: true
        onStatusChanged: {
            if (status === Loader.Ready)
                splashLoader.item.appReady()
            if (status === Loader.Error)
                splashLoader.item.errorInLoadingApp();
        }
    }


}
