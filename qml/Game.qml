import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12

import io.qt.examples.Tetrix 1.0

Item {
    id: root

    property bool running: false

    signal gamePaused()
    signal gameOver()

    signal scoreChanged(int score)
    signal nextPiece(int shape)

    property alias logic: conn.target

    width: board.cellSize * board.size.width + (board.size.width - 1) * board.spacing
    height: board.cellSize * board.size.height + (board.size.height - 1) * board.spacing

    Keys.onPressed: {

        if (event.key === Qt.Key_Space) {
            if (piece.dropping)
                logic.pauseGame()
            else
                logic.resumeGame()
        }

        if (event.key === Qt.Key_Up) piece.tryRotate()

        if (event.key === Qt.Key_Left) piece.tryGoLeft()

        if (event.key === Qt.Key_Right) piece.tryGoRight()

        if (event.key === Qt.Key_Down) piece.tryGoDown()

    }
    Connections {
        id: conn
        onStartGame: { // do init and this calls once per game
            piece.dropping = true
            root.running = true
        }
        onPauseGame: {
            piece.dropping = false
            root.running = false
        }
        onResumeGame: {
            piece.dropping = true
            root.running = true
        }
    }
    Timer {
        id: timer
        repeat: true
        running: piece.dropping
        interval: 1000
        onTriggered: {
//            console.debug("onTriggered")
            piece.tryGoDown()
        }
    }
    TetrixPiece {
        id: piece

        property bool landed: false
        property bool dropping: false
        property point centerPt: board.startPoint

        onLandedChanged: {
            if (landed) {
                listModel.landPiece(piece.shape, piece.centerPt)

                if (listModel.getState(board.startPoint) === Cell.Filled) {
                    root.running = false
                    root.gameOver()
                }
                else {
                    reset()
                }

            }
        }

        Component.onCompleted: setRandomShape()
        //        shape: TetrixPiece.LShape
        onShapeChanged: fillAndUpdate()

        function reset() {
            centerPt = board.startPoint
            setRandomShape()

            landed = false
            dropping = true
        }

        // public
        function tryRotate() {
            if (listModel.canRotate(piece.shape, piece.centerPt))
                next()
        }
        function tryGoLeft() {
            if (listModel.canGoLeft(piece.shape, piece.centerPt))
                goLeft()
        }
        function tryGoRight() {

            if (listModel.canGoRight(piece.shape, piece.centerPt))
                goRight()
        }
        function tryGoDown() {
            if ( listModel.canGoDown(piece.shape, piece.centerPt) )
                goDown()
            else {
                dropping = false
                landed = true
            }
        }
        // private
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

    Board2 {
        id: board
        anchors.centerIn: parent
        anchors.fill: parent
        cellSize: 30
        spacing: 1

        readonly property size size: Qt.size(10, 12)
        readonly property point startPoint: Qt.point(5, 1)

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
