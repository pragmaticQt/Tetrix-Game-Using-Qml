import QtQuick 2.0

ListView {
    id: root

    property int cellSize: 50
    spacing: 2
    interactive: false

    delegate: BoardRow {
        row: index
        values: {
            var result = root.model.getRow(index)
            return result
        }
    }
}
