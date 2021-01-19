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
        text: {
            switch (game.lifeCycle) {
            default:
            case Game.Ready:
            case Game.Done:
                return qsTr("Start")
            case Game.Running:
                return qsTr("Pause")
            case Game.Paused:
                return qsTr("Resume")
            }
        }

        onClicked: {
            game.forceActiveFocus()

            switch (game.lifeCycle) {
            default:
            case Game.Ready:
            case Game.Done:
                gameLogic.startGame()
                break;
            case Game.Running:
                gameLogic.pauseGame()
                break;
            case Game.Paused:
                gameLogic.resumeGame()
                break;
            }
        }
    }


}
