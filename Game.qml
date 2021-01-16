import QtQuick 2.0
import io.qt.examples.Tetrix 1.0

Board {
    focus: true

    property alias logic: connections.target

    property point centerOfPiece: Qt.point(4, 4)
    readonly property size boardSize: Qt.size(10, 10)

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
        clearOldPiece()
        centerOfPiece.y += 1
        updatePiece()
    }
    function moveLeft() {
        clearOldPiece()
        centerOfPiece.x -= 1
        updatePiece()
    }
    function moveRight() {
        clearOldPiece()
        centerOfPiece.x += 1
        updatePiece()
    }
    function updatePiece() {
        fillNewPiece()
        model_.dataChanged()
    }
    function fillNewPiece() {
        var points = piece.points

        for (let i = 0; i < 4; ++i) {
            var point = points[i]
            point.x += centerOfPiece.x
            point.y += centerOfPiece.y
        }

        model_.fill(points)
    }

    function clearOldPiece() {
        var points = piece.points

        for (let i = 0; i < 4; ++i) {
            var point = points[i]
            point.x += centerOfPiece.x
            point.y += centerOfPiece.y
        }

        model_.reset(points)
    }

    function rotatePiece() {
        clearOldPiece()
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
