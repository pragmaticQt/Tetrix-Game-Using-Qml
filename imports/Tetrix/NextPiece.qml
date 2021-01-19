import QtQuick 2.12

Item {
    enum Shape {
        NoShape = 0,
        ZShape,
        SShape,
        LineShape,
        TShape,
        SquareShape,
        LShape,
        MirroredLShape
    }

    property var currentPiece: NextPiece.MirroredLShape

    width: 60
    height: 60

    //    Row {
    //        spacing: 4
    Row {
        id: zShape
        spacing: 1
        visible: currentPiece === NextPiece.ZShape
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
        visible: currentPiece === NextPiece.SShape
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
        visible: currentPiece === NextPiece.LineShape
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
        visible: currentPiece === NextPiece.TShape
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
        visible: currentPiece === NextPiece.SquareShape
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
        visible: currentPiece === NextPiece.LShape
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
        visible: currentPiece === NextPiece.MirroredLShape
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
