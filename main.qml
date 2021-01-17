import QtQuick 2.13
import QtQuick.Controls 2.0
import QtQuick.Window 2.13
import io.qt.examples.Tetrix 1.0


Window {
    id: root

    visible: true
    width: 640
    height: 480

    title: qsTr("Hello Tetrix")

    //    Game {
    //        logic: gameLogic
    //    }

    //    GameLogic {
    //        id: gameLogic
    //    }

    Board2 { // its size automatically determined by elements
        anchors.fill: parent
        model: listModel
    }
}
