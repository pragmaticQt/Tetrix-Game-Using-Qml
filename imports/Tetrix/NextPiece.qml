import QtQuick 2.12
import io.qt.examples.Tetrix 1.0

Item {
//    enum Shape {
//        NoShape = 0,
//        ZShape,
//        SShape,
//        LineShape,
//        TShape,
//        SquareShape,
//        LShape,
//        MirroredLShape
//    }

    property var currentPiece//: TetrixShape.MirroredLShape

    width: 60
    height: 60

    //    Row {
    //        spacing: 4
    Row {
        id: zShape
        spacing: 1
        visible: currentPiece === TetrixShape.ZShape || currentPiece === TetrixShape.ZShape1
        anchors.centerIn: parent
        Rectangle {
            width: 12
            height: width
            color: "green"
        }
        Column {
            spacing: 1
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
        }
        Rectangle {
            width: 12
            height: width
            color: "green"
            anchors.bottom: parent.bottom
        }
    }

    Row {
        id: sShape
        spacing: 1
        visible: currentPiece === TetrixShape.SShape || currentPiece === TetrixShape.SShape1
        anchors.centerIn: parent
        Rectangle {
            width: 12
            height: width
            color: "green"
            anchors.bottom: parent.bottom
        }
        Column {
            spacing: 1
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
        }
        Rectangle {
            width: 12
            height: width
            color: "green"
        }
    }

    Column {
        id: lineShape
        spacing: 1
        visible: currentPiece === TetrixShape.LineShape || currentPiece === TetrixShape.LineShape1
        anchors.centerIn: parent
        Repeater {
            model: 4
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
        }
    }

    Row {
        id: tShape
        spacing: 1
        visible: currentPiece === TetrixShape.TShape
                 || currentPiece === TetrixShape.TShape1
                 || currentPiece === TetrixShape.TShape2
                 || currentPiece === TetrixShape.TShape3
        anchors.centerIn: parent
        Rectangle {
            width: 12
            height: width
            color: "green"
        }
        Column {
            spacing: 1
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
        }
        Rectangle {
            width: 12
            height: width
            color: "green"
        }
    }

    Grid {
        id: squareShape
        spacing: 1
        columns: 2
        visible: currentPiece === TetrixShape.SquareShape
        anchors.centerIn: parent
        Repeater {
            model: 4
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
        }
    }

    Column {
        id: lShape
        spacing: 1
        visible: currentPiece === TetrixShape.LShape
                 || currentPiece === TetrixShape.LShape1
                 || currentPiece === TetrixShape.LShape2
                 || currentPiece === TetrixShape.LShape3
        anchors.centerIn: parent
        Repeater {
            model: 2
            Rectangle {
                width: 12
                height: width
                color: "green"
            }
        }
        Row {
            spacing: 1
            Repeater {
                model: 2
                Rectangle {
                    width: 12
                    height: width
                    color: "green"
                }
            }
        }
    }

    Row {
        id: mirroLShape
        spacing: 1
        visible: currentPiece === TetrixShape.MirroredLShape
                 || currentPiece === TetrixShape.MirroredLShape1
                 || currentPiece === TetrixShape.MirroredLShape2
                 || currentPiece === TetrixShape.MirroredLShape3
        anchors.centerIn: parent
        Rectangle {
            width: 12
            height: width
            color: "green"
            anchors.bottom: parent.bottom
        }
        Column {
            spacing: 1
            Repeater {
                model: 3
                Rectangle {
                    width: 12
                    height: width
                    color: "green"
                }
            }
        }
    }
    //    }
}
