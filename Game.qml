import QtQuick 2.0
import QtQuick.Controls 2.0
import io.qt.examples.Tetrix 1.0

Item {
    id: root

    width: board.cellSize * board.size.width + (board.size.width - 1) * board.spacing
    height: board.cellSize * board.size.height + (board.size.height - 1) * board.spacing
    signal gameRunning()
    signal gamePaused()
    signal gameOver()

    signal scoreChanged(int score)
    signal nextPiece(int shape)

    property alias logic: connections.target

    focus: true
    Keys.onPressed: {
        if (event.key === Qt.Key_Space) pieceController.tryRotate()

        if (event.key === Qt.Key_Left) pieceController.tryGoLeft()

        if (event.key === Qt.Key_Right) pieceController.tryGoRight()

        if (event.key === Qt.Key_Down) pieceController.tryGoDown()

    }

    Timer {
        id: timer
        repeat: true
        running: true
        interval: 1000
        onTriggered: pieceController.tryGoDown()
    }

    QtObject {
        id: pieceController

        signal tryRotate()
        signal tryGoLeft()
        signal tryGoRight()
        signal tryGoDown()

        onTryRotate: {
            // only if piece has enough room
            // e.g.the rotated bound is inside the board still
            var bound = piece.getBoundOfNextRotation()
            if (0 <= bound.minX + piece.centerPt.x
                    && bound.maxX + piece.centerPt.x < board.size.width
                    && bound.maxY + piece.centerPt.y < board.size.height)
                piece.next()
        }
        onTryGoLeft: {
            if (0 < piece.minX + piece.centerPt.x)
                piece.goLeft()
        }
        onTryGoRight: {

            if ( piece.maxX + piece.centerPt.x < board.size.width-1)
                piece.goRight()
        }
        onTryGoDown: {
            if ( listModel.canGoDown(piece.shape, piece.centerPt) )
                piece.goDown()
        }
    }
    Connections {
        id: connections

        function onStartGame() {
            //                centerOfPiece = startPoint
        }

        function onStopGame(){
        }

        function onPauseGame() {
        }

        function onEesumeGame() {
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

        Component.onCompleted: { _.fillAndUpdate(piece.points); }

        model: listModel//gameBoard
    }

    QtObject {// internal
        id: _

        function fillAndUpdate() {
            fill()
            update()
        }

        function update() {
            listModel.dataChanged()
        }

        function fill() {
            listModel.fillPiece(piece.shape, piece.centerPt)
        }

        function clear() {
            listModel.clearPiece(piece.shape, piece.centerPt)
        }
    }

    GameBoardListModel {
        id: listModel
        size: board.size
        signal dataChanged()
    }
    TetrixPiece {
        id: piece
        shape: TetrixPiece.LShape

        onPointsChanged: _.fillAndUpdate(piece.points)

        property point centerPt: board.startPoint

        function goDown() {
            _.clear(piece.points)
            piece.centerPt.y += 1
            _.fillAndUpdate(piece.points)
        }

        function goLeft() {
            _.clear(piece.points)
            piece.centerPt.x -= 1
            _.fillAndUpdate(piece.points)
        }

        function goRight() {
            _.clear(piece.points)
            piece.centerPt.x += 1
            _.fillAndUpdate(piece.points)
        }

        function next() {
            _.clear(piece.points)
            piece.rotate()
        }
    }

}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
