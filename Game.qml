import QtQuick 2.0
import io.qt.examples.Tetrix 1.0

Board {
    focus: true

    property alias logic: connections.target

    property point centerOfPiece: Qt.point(4, 2)
    readonly property size boardSize: Qt.size(10, 6)

    Connections {
        id: connections

        function onAddBlocks(points) {
//            model_.setState(row, column, GameBoard.Occupied)
        }

        function onEraseRow( row ){

        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Space) rotatePiece();

        if (event.key === Qt.Key_Left) moveLeft();

        if (event.key === Qt.Key_Right) moveRight();

        if (event.key === Qt.Key_Down) moveDown();

    }

    Component.onCompleted: updatePiece()

    function moveDown() {
        centerOfPiece.y += 1
        updatePiece()
    }
    function moveLeft() {
        centerOfPiece.x -= 1
        updatePiece()
    }
    function moveRight() {
        centerOfPiece.x += 1
        updatePiece()
    }
    function updatePiece() {

        model_.resetAll()

        var points = piece.points;

        for (let i = 0; i < points.length; i++) {
            var point = points[i]
            var row = centerOfPiece.y+point.y
            var column = centerOfPiece.x+point.x
            model_.setState(row, column, GameBoard.Occupied)
        }
        model_.dataChanged()
    }
    function rotatePiece() {
        piece.rotate()
    }

    TetrixPiece {
        id: piece
        shape: TetrixPiece.LShape
        onPointsChanged: updatePiece()

    }

    GameBoard {
        id: model_
        size: boardSize
        signal dataChanged()
    }

    model: model_
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
