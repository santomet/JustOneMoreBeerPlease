import QtQuick 2.12

GamePage {

    Rectangle {
        id: viewContainer
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: GameSettings.fieldMargin + messageHeight
        anchors.bottomMargin: GameSettings.fieldMargin
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - GameSettings.fieldMargin*2
        color: GameSettings.viewColor
        radius: GameSettings.buttonRadius


        Text {
            id: title
            width: parent.width
            height: GameSettings.fieldHeight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: GameSettings.textColor
            font.pixelSize: GameSettings.mediumFontSize
            text: qsTr("TABLES")

            BottomLine {
                height: 1;
                width: parent.width
                color: "#898989"
            }
        }


        ListView {
            id: tables
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: title.bottom
            model: backend.tables
            clip: true

            delegate: Rectangle {
                id: box
                height:GameSettings.fieldHeight * 1.2
                width: parent.width
                color: index % 2 === 0 ? GameSettings.delegate1Color : GameSettings.delegate2Color

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        backend.setActiveTable(modelData.tableName)
                        if(modelData.waitingOrder) {
                            app.showPage("OrderList.qml")
                        }
                        else
                            app.showPage("BeverageList.qml")
                    }
                }

                Text {
                    id: table
                    font.pixelSize: GameSettings.smallFontSize
                    text: modelData.tableName
                    anchors.top: parent.top
                    anchors.topMargin: parent.height * 0.1
                    anchors.leftMargin: parent.height * 0.1
                    anchors.left: parent.left
                    color: GameSettings.textColor
                }

                Text {
                    id: tableNotify
                    font.pixelSize: GameSettings.smallFontSize
                    text: modelData.waitingOrder ? "UNFULFILLED ORDERS!" : ""
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.rightMargin: parent.height * 0.1
                    anchors.right: parent.right
                    color: Qt.darker(GameSettings.infoColor)
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

//    GameButton {
//        id: searchButton
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: GameSettings.fieldMargin
//        width: viewContainer.width
//        height: GameSettings.fieldHeight
//        enabled: !deviceFinder.scanning
//        onClicked: deviceFinder.startSearch()

//        Text {
//            anchors.centerIn: parent
//            font.pixelSize: GameSettings.tinyFontSize
//            text: qsTr("START SEARCH")
//            color: searchButton.enabled ? GameSettings.textColor : GameSettings.disabledTextColor
//        }
//    }
}
