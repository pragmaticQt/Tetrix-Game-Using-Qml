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
        if (event.key === Qt.Key_Space) piece.next();

        if (event.key === Qt.Key_Left) piece.goLeft();

        if (event.key === Qt.Key_Right) piece.goRight();

        if (event.key === Qt.Key_Down) piece.goDown();

    }

    Component.onCompleted: _.updatePiece()

    QtObject {// internal
        id: _

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
    }

    TetrixPiece {
        id: piece
        shape: TetrixPiece.LShape
        onPointsChanged: _.updatePiece()

        function goDown() {
            _.clearOldPiece()
            centerOfPiece.y += 1
            _.updatePiece()
        }

        function goLeft() {
            _.clearOldPiece()
            centerOfPiece.x -= 1
            _.updatePiece()
        }

        function goRight() {
            _.clearOldPiece()
            centerOfPiece.x += 1
            _.updatePiece()
        }

        function next() {
            _.clearOldPiece()
            piece.rotate()
        }
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
