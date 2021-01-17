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

    QtObject {
        id: pieceController

        signal tryRotate()
        signal tryGoLeft()
        signal tryGoRight()
        signal tryGoDown()

    }

    Board2 {
        id: board
        anchors.centerIn: parent
        anchors.fill: parent
        cellSize: 40
        spacing: 1

        property point centerOfPiece: startPoint
        readonly property size size: Qt.size(6, 10)
        readonly property point startPoint: Qt.point(4, 1)

        Connections {
            id: connections

            function onStartGame() {
                centerOfPiece = startPoint
            }

            function onStopGame(){
            }

            function onPauseGame() {
            }

            function onEesumeGame() {
            }
        }

        Component.onCompleted: { _.fillAndUpdate(piece.points); }

        QtObject {// internal
            id: _

            function fillAndUpdate(points) {
                fill(points)
                update()
            }

            function update() {
                listModel.dataChanged()
            }

            function fill(points) {
                //var points = piece.points

                for (let i = 0; i < 4; ++i) {
                    var point = points[i]
                    point.x += board.centerOfPiece.x
                    point.y += board.centerOfPiece.y
                }

                listModel.fill(points)
            }

            function clear(points) {
                //var points = piece.points

                for (let i = 0; i < 4; ++i) {
                    var point = points[i]
                    point.x += board.centerOfPiece.x
                    point.y += board.centerOfPiece.y
                }

                listModel.reset(points)
            }
        }

        Connections {

            target: pieceController
            onTryRotate: {
                // only if piece has enough room
                // e.g.the rotated bound is inside the board still
                var bound = piece.getBoundOfNextRotation()
                if (0 <= bound.minX + board.centerOfPiece.x
                        && bound.maxX + board.centerOfPiece.x < board.size.width
                        && bound.maxY + board.centerOfPiece.y < board.size.height)
                    piece.next()
            }
            onTryGoLeft: {
                if (0 < piece.minX + board.centerOfPiece.x)
                    piece.goLeft()
            }
            onTryGoRight: {

                if ( piece.maxX + board.centerOfPiece.x < board.size.width-1)
                    piece.goRight()
            }
            onTryGoDown: {
                if ( piece.maxY + board.centerOfPiece.y < board.size.height-1)
                    piece.goDown()
            }
        }

        TetrixPiece {
            id: piece
            shape: TetrixPiece.LShape
            onPointsChanged: _.fillAndUpdate(piece.points)

            function goDown() {
                _.clear(piece.points)
                board.centerOfPiece.y += 1
                _.fillAndUpdate(piece.points)
            }

            function goLeft() {
                _.clear(piece.points)
                board.centerOfPiece.x -= 1
                _.fillAndUpdate(piece.points)
            }

            function goRight() {
                _.clear(piece.points)
                board.centerOfPiece.x += 1
                _.fillAndUpdate(piece.points)
            }

            function next() {
                _.clear(piece.points)
                piece.rotate()
            }
        }

        //        GameBoard {
        //            id: gameBoard
        //            size: board.size
        //            signal dataChanged()
        //        }
        GameBoardListModel {
            id: listModel
            size: board.size
            signal dataChanged()
        }
        model: listModel//gameBoard
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
