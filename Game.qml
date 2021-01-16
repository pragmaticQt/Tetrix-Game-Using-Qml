import QtQuick 2.0
import io.qt.examples.Tetrix 1.0

Board {
    id: root

    property alias logic: connections.target

    property point centerOfPiece: startPoint
    readonly property size boardSize: Qt.size(11, 10)
    readonly property point startPoint: Qt.point(5, 1)

    signal gameRunning()
    signal gamePaused()
    signal gameOver()

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

    focus: true
    Keys.onPressed: {
        if (event.key === Qt.Key_Space) pieceController.tryRotate()

        if (event.key === Qt.Key_Left) pieceController.tryGoLeft()

        if (event.key === Qt.Key_Right) pieceController.tryGoRight()

        if (event.key === Qt.Key_Down) pieceController.tryGoDown()

    }

    Component.onCompleted: { _.fillAndUpdate(piece.points); }

    QtObject {// internal
        id: _

        function fillAndUpdate(points) {
            fill(points)
            update()
        }

        function update() {
            gameBoard.dataChanged()
        }

        function fill(points) {
            //var points = piece.points

            for (let i = 0; i < 4; ++i) {
                var point = points[i]
                point.x += centerOfPiece.x
                point.y += centerOfPiece.y
            }

            gameBoard.fill(points)
        }

        function clear(points) {
            //var points = piece.points

            for (let i = 0; i < 4; ++i) {
                var point = points[i]
                point.x += centerOfPiece.x
                point.y += centerOfPiece.y
            }

            gameBoard.reset(points)
        }
    }

    QtObject {
        id: pieceController

        signal tryRotate()
        signal tryGoLeft()
        signal tryGoRight()
        signal tryGoDown()

        onTryRotate: {
            piece.next()
        }
        onTryGoLeft: {
            piece.goLeft()
        }
        onTryGoRight: {
            piece.goRight()
        }
        onTryGoDown: {
            piece.goDown()
        }
    }

    TetrixPiece {
        id: piece
        shape: TetrixPiece.LShape
        onPointsChanged: _.fillAndUpdate(piece.points)

        function goDown() {
            _.clear(piece.points)
            centerOfPiece.y += 1
            _.fillAndUpdate(piece.points)
        }

        function goLeft() {
            _.clear(piece.points)
            centerOfPiece.x -= 1
            _.fillAndUpdate(piece.points)
        }

        function goRight() {
            _.clear(piece.points)
            centerOfPiece.x += 1
            _.fillAndUpdate(piece.points)
        }

        function next() {
            _.clear(piece.points)
            piece.rotate()
        }
    }

    GameBoard {
        id: gameBoard
        size: boardSize
        signal dataChanged()
    }

    model: gameBoard
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
