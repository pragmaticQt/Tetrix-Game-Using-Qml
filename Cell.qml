import QtQuick 2.0
import io.qt.examples.Tetrix 1.0

Rectangle {
    id: cell

    property alias target: connection.target
    property int row: 0
    property int column: 0
    property int size: 10

    width: size
    height: size

    color: getCorrectColor()

    Connections {
        id: connection
        onDataChanged: {
            cell.color = cell.getCorrectColor()
        }
    }

    function getCorrectColor() {
        var status = target.data(target.index(row, column))
        switch(status+GameBoard.Empty) {
        default:
        case GameBoard.Empty:
            return "honeydew"
        case GameBoard.Occupied:
            return "black"
        }
    }
}
