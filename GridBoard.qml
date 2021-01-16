import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root

    readonly property int effectiveGridWidth: 2
    readonly property int effectiveGridHeight: 3
    
    readonly property int blockSize: 30
    readonly property int blockSpace: 1

    property color emptyColor: "grey"
    property color occupidColor: "red"

    width: 640
    height: 480

    Grid {
        width: effectiveGridWidth * blockSize + spacing * (effectiveGridWidth + 1)
        height: effectiveGridHeight * blockSize + spacing * (effectiveGridWidth + 1)
        spacing: blockSpace

        anchors.centerIn: parent
        columns: effectiveGridWidth
        Repeater {
            model: effectiveGridWidth * effectiveGridHeight
            Rectangle {
                width: root.blockSize
                height: width
                color: {
                    //index
                    var i = Math.floor(index / effectiveGridWidth)
                    var j = index % effectiveGridWidth
                    return root.occupied && root.occupied[i][j] ? root.occupidColor : root.emptyColor
                }
            }
        }
    }

    //    Column {
    //        id: colLines
    //        anchors.centerIn: parent
    //        spacing: blockSize
    //        Repeater {
    //            id: repeater2
    //            model: gridHeight
    //            Rectangle {
    //                width: (gridWidth-1) * (root.blockSize+root.lineWidth)+root.lineWidth
    //                height: root.lineWidth
    //                color: "black"
    //            }
    //        }
    //    }

    //    Row {
    //        id: rowLines
    //        anchors.centerIn: parent

    //        spacing: blockSize
    //        Repeater {
    //            id: repeater
    //            model: gridWidth
    //            Rectangle {
    //                width: root.lineWidth
    //                height: (gridHeight-1) * (root.blockSize+root.lineWidth)+root.lineWidth
    //                color: "black"
    //            }
    //        }
    //    }
}




