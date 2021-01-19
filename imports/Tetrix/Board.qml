import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3


Pane {
    id: root

    property alias title: title.text
    property alias textContent: content.text
    property alias nextPiece: piece.currentPiece

    width: 200
    height: 110

    background: Rectangle {
        anchors.fill: root
        //        color: "lightgrey"
    }

    Text {
        id: title
        text: qsTr("SCORE")
        font.pixelSize: 12
        font.family: "Arial"

        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        id: content

        visible: !!text

        //        text: qsTr("1216")
        font.pixelSize: 50
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.family: "Arial"

        anchors.right: outline.right
        anchors.rightMargin: 5
        anchors.verticalCenter: outline.verticalCenter
    }

    NextPiece {
        id: piece
        currentPiece: NextPiece.NoShape
        anchors.centerIn: outline
        visible: currentPiece !== NextPiece.NoShape
        //        Rectangle {
        //            anchors.fill: parent
        //            opacity: 0.5
        //            border.color: "red"
        //        }
    }

    Rectangle {
        id: outline

        border.width: 1
        border.color: "grey"
        color: "transparent"

        anchors.top: title.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
