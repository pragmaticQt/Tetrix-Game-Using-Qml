import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Window 2.13
import io.qt.examples.Tetrix 1.0
import "imports/Tetrix"
import "qml"
import QtQuick.Layouts 1.3


Window {
    id: root

    visible: true
    width: 640
    height: 480

    title: qsTr("Hello Tetrix")

    GameLogic {
        id: gameLogic
    }

    Row {
        spacing: 20
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Game {
            id: game
            focus: true
            logic: gameLogic
            anchors.verticalCenter: parent.verticalCenter
        }
        Column {
            anchors.verticalCenter: parent.verticalCenter
            NextPieceBoard {
                nextPiece: game.nextShape
            }
            ScoreBoard {
                textContent: game.score
            }
//            LevelBoard {
//                textContent: "I"
//            }

            Button {
                id: button
                anchors.horizontalCenter: parent.horizontalCenter
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
    }
}
