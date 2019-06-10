import QtQuick 2.12
import "."

Rectangle    {
    id: titleBar
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: GameSettings.fieldHeight
    color: GameSettings.viewColor

    property var __titles: ["TABLES", "ORDERS", "MENU"]
    property int currentIndex: 0

    signal titleClicked(int index)

    Repeater {
        model: __titles.length
        Text {
            width: titleBar.width / __titles.length
            height: titleBar.height
            x: index * width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: __titles[index]
            font.pixelSize: GameSettings.tinyFontSize
            color: titleBar.currentIndex === index ? GameSettings.textColor : GameSettings.disabledTextColor

            MouseArea {
                anchors.fill: parent
                onClicked: titleClicked(index)
            }
        }
    }


    Item {
        anchors.bottom: parent.bottom
        width: parent.width / __titles.length
        height: parent.height
        x: currentIndex * width

        BottomLine{}

        Behavior on x { NumberAnimation { duration: 200 } }
    }

}
