import QtQuick 2.0

Column {
    property var elements
    spacing: 10
    Repeater {
        model: elements
        BoardRow {
            values: elements[index]
        }
    }
}
