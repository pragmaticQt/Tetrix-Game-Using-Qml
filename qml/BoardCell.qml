import QtQuick 2.0
import io.qt.examples.Tetrix 1.0

Rectangle {
    id: cell

    property alias target: connection.target
    property point pt: Qt.point(0, 0)
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
        var status = target.getState(pt)
        switch(status+Cell.Empty) {
        default:
        case Cell.Empty:
            return "red"
        case Cell.Occupied:
        case Cell.Filled:
            return "black"
        }
    }
}
