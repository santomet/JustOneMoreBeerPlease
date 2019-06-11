import QtQuick 2.12
import "."

GamePage {
    property double totalCost: 0

    function init()
    {
        backend.requestMenu()
    }

    Rectangle {
        id: viewContainer
        anchors.top: parent.top
        anchors.bottom: orderTotal.top
        anchors.topMargin: GameSettings.fieldMargin + messageHeight
        anchors.bottomMargin: GameSettings.fieldMargin
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - GameSettings.fieldMargin*2
        color: GameSettings.viewColor
        radius: GameSettings.buttonRadius

        Text {
            id: tableName
            width: parent.width
            height: GameSettings.fieldHeight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: GameSettings.textColor
            font.pixelSize: GameSettings.smallFontSize;
            text: backend.activeTable.length == 0 ? "Please select table" : "Active table: " + backend.activeTable;
        }


        Text {
            id: title
            anchors.top: tableName.bottom
            width: parent.width
            height: GameSettings.fieldHeight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: GameSettings.textColor
            font.pixelSize: GameSettings.mediumFontSize
            text: qsTr("BEVERAGES MENU")

            BottomLine {
                height: 1;
                width: parent.width
                color: "#898989"
            }
        }


        ListView {
            id: beverages
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: title.bottom
            model: backend.beverages
            clip: true

            delegate: Rectangle {
                id: box
                height:GameSettings.fieldHeight * 1.2
                width: parent.width
                color: {
                    if(modelData.orderCount > 0)
                        index % 2 === 0 ? GameSettings.delegateActive1Color : GameSettings.delegateActive2Color
                    else
                        index % 2 === 0 ? GameSettings.delegate1Color : GameSettings.delegate2Color
                }
                MouseArea {
                    anchors.left: parent.horizontalCenter
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    onClicked: {
                        modelData.orderCount++
                        totalCost += modelData.price
                    }
                }

                MouseArea {
                    anchors.left: parent.left
                    anchors.right: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    onClicked: {
                        if(modelData.orderCount > 0) {
                            modelData.orderCount--
                            totalCost -= modelData.price
                        }
                    }
                }

                Text {
                    id: beverageName
                    font.pixelSize: GameSettings.smallFontSize
                    text: modelData.name
                    anchors.top: parent.top
                    anchors.topMargin: parent.height * 0.1
                    anchors.leftMargin: parent.height * 0.1
                    anchors.left: parent.left
                    color: GameSettings.textColor
                }

                Text {
                    id: beverageSize
                    font.pixelSize: GameSettings.smallFontSize
                    text: modelData.size
                    anchors.top: parent.top
                    anchors.topMargin: parent.height * 0.1
                    anchors.leftMargin: parent.height * 0.1
                    anchors.right: parent.right
                    color: GameSettings.textColor
                }

                Text {
                    id: orderc
                    font.pixelSize: GameSettings.smallFontSize
                    text: modelData.orderCount === 0 ? "" : modelData.orderCount
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.leftMargin: parent.height * 0.1
                    anchors.left: parent.left
                    color: Qt.darker(GameSettings.textColor)
                }

                Text {
                    id: price
                    font.pixelSize: GameSettings.smallFontSize
                    text: modelData.price + " CZK"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.rightMargin: parent.height * 0.1
                    anchors.right: parent.right
                    color: Qt.darker(GameSettings.textColor)
                }
            }
        }
    }

//    GameButton {
//        id: addressTypeButton
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: searchButton.top
//        anchors.bottomMargin: GameSettings.fieldMargin*0.5
//        width: viewContainer.width
//        height: GameSettings.fieldHeight
//        visible: connectionHandler.requiresAddressType // only required on BlueZ
//        state: "public"
//        onClicked: state == "public" ? state = "random" : state = "public"

//        states: [
//            State {
//                name: "public"
//                PropertyChanges { target: addressTypeText; text: qsTr("Public Address") }
//                PropertyChanges { target: deviceHandler; addressType: AddressType.PublicAddress }
//            },
//            State {
//                name: "random"
//                PropertyChanges { target: addressTypeText; text: qsTr("Random Address") }
//                PropertyChanges { target: deviceHandler; addressType: AddressType.RandomAddress }
//            }
//        ]

//        Text {
//            id: addressTypeText
//            anchors.centerIn: parent
//            font.pixelSize: GameSettings.tinyFontSize
//            color: GameSettings.textColor
//        }
//    }

    Text {
        id: orderTotal
        width: parent.width
        anchors.bottom: orderButton.top
        height: GameSettings.fieldHeight
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: GameSettings.textColor
        font.pixelSize: GameSettings.smallFontSize
        text: qsTr("Total: ") + totalCost + " CZK"
    }

    GameButton {
        id: orderButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: GameSettings.fieldMargin
        width: viewContainer.width
        height: GameSettings.fieldHeight
        enabled: totalCost > 0 && backend.activeTable.length > 0
        onClicked: {
            backend.makeOrder()
            totalCost = 0
            app.showPage("OrderList.qml")
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: GameSettings.tinyFontSize
            text: qsTr("MAKE ORDER")
            color: orderButton.enabled ? GameSettings.textColor : GameSettings.disabledTextColor
        }
    }
}
