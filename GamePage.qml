import QtQuick 2.12

import "."

Item {
    anchors.fill: parent

    property string errorMessage: ""
    property string infoMessage: ""
    property real messageHeight: msg.height
    property bool hasError: errorMessage != ""
    property bool hasInfo: infoMessage != ""

    function init()
    {
    }

    function close()
    {
        app.prevPage()
    }

    Rectangle {
        id: msg
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: GameSettings.fieldHeight
        color: hasError ? GameSettings.errorColor : GameSettings.infoColor
        visible: hasError || hasInfo

        Text {
            id: error
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            minimumPixelSize: 5
            font.pixelSize: GameSettings.smallFontSize
            fontSizeMode: Text.Fit
            color: GameSettings.textColor
            text: hasError ? errorMessage : infoMessage
        }
    }
}
