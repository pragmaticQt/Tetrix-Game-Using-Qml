import QtQuick 2.13
import QtQuick.Controls 2.0
import io.qt.examples.Tetrix 1.0


Row {
    id: root
    property var values
    property int row: 0

    spacing: ListView.view.spacing

    Repeater {
        id: repeater
        model: values
        delegate: Cell {
            size: root.ListView.view.cellSize
            row: root.row
            column: index
            target: root.ListView.view.model
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
