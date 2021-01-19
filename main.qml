import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Window 2.13
import io.qt.examples.Tetrix 1.0
import "qml"

Window {
    id: root

    visible: true
    width: 640
    height: 480

    title: qsTr("Hello Tetrix")

    Game {
        id: game
        focus: true
        logic: gameLogic
        anchors.centerIn: parent
    }

    GameLogic {
        id: gameLogic
    }

    Button {
        id: button
        x: 33
        y: 220
        text: game.running ? qsTr("Stop") : qsTr("Start")
        onClicked: {
            game.forceActiveFocus()
            game.running ? gameLogic.pauseGame() : gameLogic.startGame()
        }
    }


}
