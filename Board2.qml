import QtQuick 2.0

ListView {
    id: root

    property int cellSize: 100
    spacing: 10
    interactive: false

    delegate: BoardRow {
        row: index
        values: {
            var result = root.model.getRow(index)
            return result
        }
    }
}
