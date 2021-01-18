import QtQuick 2.0
import io.qt.examples.Tetrix 1.0

Column {
    id: root

    property int cellSpacing: 2
    property int cellSize: 50

    property var model

    spacing: cellSpacing

    Repeater {
        model: root.model.rowCount()

        Row {
            spacing: root.cellSpacing
            property int row_: index

            Repeater {
                model: root.model.columnCount()

                Cell {
                    target: root.model
                    row: row_
                    column: index

                    size: cellSize
                }
            } //Repeater
        } //Row
    } //Repeater
} //Column
