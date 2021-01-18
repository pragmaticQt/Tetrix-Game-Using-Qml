import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12

import io.qt.examples.Tetrix 1.0

Item {
    id: root

    signal gameRunning()
    signal gamePaused()
    signal gameOver()

    signal scoreChanged(int score)
    signal nextPiece(int shape)

    property alias logic: conn.target

    width: board.cellSize * board.size.width + (board.size.width - 1) * board.spacing
    height: board.cellSize * board.size.height + (board.size.height - 1) * board.spacing
    focus: true
    Keys.onPressed: {

        if (event.key === Qt.Key_Space) {
            if (piece.dropping)
                logic.pauseGame()
            else
                logic.resumeGame()
        }

        if (event.key === Qt.Key_Up) pieceController.tryRotate()

        if (event.key === Qt.Key_Left) pieceController.tryGoLeft()

        if (event.key === Qt.Key_Right) pieceController.tryGoRight()

        if (event.key === Qt.Key_Down) pieceController.tryGoDown()

    }
    Connections {
        id: conn
        onStartGame: { // do init and this calls once per game
            piece.dropping = true
        }
        onPauseGame: {
            piece.dropping = false
        }
        onResumeGame: {
            piece.dropping = true
        }
    }
    Timer {
        id: timer
        repeat: true
        running: piece.dropping
        interval: 1000
        onTriggered: {
            console.debug("onTriggered")
            pieceController.tryGoDown()
        }
    }
    TetrixPiece {
        id: piece

        property bool dropping: false
        property point centerPt: board.startPoint

        shape: TetrixPiece.LShape
        onShapeChanged: fillAndUpdate()

        // public
        function goDown() {
            clear()
            centerPt.y += 1
            fillAndUpdate()
        }

        function goLeft() {
            clear()
            centerPt.x -= 1
            fillAndUpdate()
        }

        function goRight() {
            clear()
            centerPt.x += 1
            fillAndUpdate()
        }

        function next() {
            clear()
            rotate()
        }

        // private
        function fillAndUpdate() {
            fill()
            update()
        }

        function update() {
            listModel.dataChanged()
        }

        function fill() {
            listModel.fillPiece(shape, centerPt)
        }

        function clear() {
            listModel.clearPiece(shape, centerPt)
        }

    }

    QtObject {
        id: pieceController

        signal tryRotate()
        signal tryGoLeft()
        signal tryGoRight()
        signal tryGoDown()

        onTryRotate: {
            if (listModel.canRotate(piece.shape, piece.centerPt))
                piece.next()
        }
        onTryGoLeft: {
            if (listModel.canGoLeft(piece.shape, piece.centerPt))
                piece.goLeft()
        }
        onTryGoRight: {

            if (listModel.canGoRight(piece.shape, piece.centerPt))
                piece.goRight()
        }
        onTryGoDown: {
            if ( listModel.canGoDown(piece.shape, piece.centerPt) )
                piece.goDown()
            else {
                piece.dropping = false
            }
        }
    }

    Board2 {
        id: board
        anchors.centerIn: parent
        anchors.fill: parent
        cellSize: 40
        spacing: 1

        readonly property size size: Qt.size(6, 10)
        readonly property point startPoint: Qt.point(4, 1)

        Component.onCompleted: { piece.shapeChanged() }

        model: listModel
    }

    GameBoardListModel {
        id: listModel
        size: board.size
        signal dataChanged()
    }

}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
