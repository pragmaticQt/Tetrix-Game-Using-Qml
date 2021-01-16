import QtQuick 2.13
import QtQuick.Controls 2.0
import io.qt.examples.Tetrix 1.0


Row {
    property var values
    spacing: 10
    Repeater {
        id: repeater
        model: values
        Cell2 {
            color: values[index]===(0+GameBoard.Occupied) ? "black" : "honeydew"
            size: 50
            column: index
        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
