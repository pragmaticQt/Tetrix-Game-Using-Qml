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
            if (0 <= bound.minX + piece.centerOfPiece.x
                    && bound.maxX + piece.centerOfPiece.x < board.size.width
                    && bound.maxY + piece.centerOfPiece.y < board.size.height)
                piece.next()
        }
        onTryGoLeft: {
            if (0 < piece.minX + piece.centerOfPiece.x)
                piece.goLeft()
        }
        onTryGoRight: {

            if ( piece.maxX + piece.centerOfPiece.x < board.size.width-1)
                piece.goRight()
        }
        onTryGoDown: {
            if ( piece.maxY + piece.centerOfPiece.y < board.size.height-1)
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
                point.x += piece.centerOfPiece.x
                point.y += piece.centerOfPiece.y
            }

            listModel.fill(points)
        }

        function clear(points) {
            //var points = piece.points

            for (let i = 0; i < 4; ++i) {
                var point = points[i]
                point.x += piece.centerOfPiece.x
                point.y += piece.centerOfPiece.y
            }

            listModel.reset(points)
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
        property point centerOfPiece: board.startPoint
        function goDown() {
            _.clear(piece.points)
            piece.centerOfPiece.y += 1
            _.fillAndUpdate(piece.points)
        }

        function goLeft() {
            _.clear(piece.points)
            piece.centerOfPiece.x -= 1
            _.fillAndUpdate(piece.points)
        }

        function goRight() {
            _.clear(piece.points)
            piece.centerOfPiece.x += 1
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
