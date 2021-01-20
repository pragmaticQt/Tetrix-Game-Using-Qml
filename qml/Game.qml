import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtMultimedia 5.0
import io.qt.examples.Tetrix 1.0

Frame {
    id: root

    property int lifeCycle: Game.LifeCycle.Ready
    readonly property alias nextShape: piece.nextShape
    readonly property alias score: listModel.score

    enum LifeCycle {
        Ready,
        Running,
        Paused,
        Done
    }

    signal gamePaused()
    signal gameOver()

    property alias logic: conn.target

    width: board.cellSize * board.size.width + (board.size.width - 1) * board.spacing + 24
    height: board.cellSize * board.size.height + (board.size.height - 1) * board.spacing + 24

    Keys.onPressed: {

        if (event.key === Qt.Key_Up/* || event.key === Qt.Key_Space*/) piece.tryRotate()

        if (event.key === Qt.Key_Left) piece.tryGoLeft()

        if (event.key === Qt.Key_Right) piece.tryGoRight()

        if (event.key === Qt.Key_Down) piece.tryGoDown()

    }

    Audio {
        id: bkgMusic
        autoPlay: false
        autoLoad: true
        source: Qt.resolvedUrl("../assets/tetris.mp3")
        loops: Audio.Infinite
        onError: console.debug(errorString)
    }

    SoundEffect {
        id: clearSE
        source: Qt.resolvedUrl("../assets/clear.wav")
    }
    SoundEffect {
        id: landSE
        source: Qt.resolvedUrl("../assets/fall.wav")
    }
    SoundEffect {
        id: gameoverSE
        source: Qt.resolvedUrl("../assets/gameover.wav")
    }

    Connections {
        id: conn
        onStartGame: { // do init and this calls once per game

            listModel.score = 0
            listModel.resetAll()
            listModel.dataChanged()

            piece.reset()
            root.lifeCycle = Game.Running
            bkgMusic.play()
        }
        onPauseGame: {
            piece.dropping = false
            root.lifeCycle = Game.Paused
            bkgMusic.pause()
        }
        onResumeGame: {
            piece.dropping = true
            root.lifeCycle = Game.Running
            bkgMusic.play()
        }
        onStopGame: {
            bkgMusic.stop()
            root.gameOver()

        }
    }
    Timer {
        id: timer
        repeat: true
        running: piece.dropping
        interval: 500
        onTriggered: {
            //            console.debug("onTriggered")
            piece.tryGoDown()
        }
    }
    TetrixPiece {
        id: piece

        property int nextShape: getRandomShape()

        property bool landed: false
        property bool dropping: false
        property point centerPt: board.startPoint

        onLandedChanged: {
            if (landed) {
                landSE.play()
                listModel.landPiece(piece.shape, piece.centerPt)

                if (listModel.getState(board.startPoint) === Cell.Filled) {
                    gameLogic.stopGame()
                    gameoverSE.play()
                    root.lifeCycle = Game.Done
                }
                else {
                    reset()
                }

            }
        }

        //        Component.onCompleted: setRandomShape()
        //        shape: TetrixPiece.LShape
        onShapeChanged: fillAndUpdate()

        function reset() {
            centerPt = board.startPoint

            shape = nextShape
            nextShape = getRandomShape()

            landed = false
            dropping = true
        }

        // public
        function tryRotate() {
            if (listModel.canRotate(piece.shape, piece.centerPt))
                next()
        }
        function tryGoLeft() {
            if (listModel.canGoLeft(piece.shape, piece.centerPt))
                goLeft()
        }
        function tryGoRight() {

            if (listModel.canGoRight(piece.shape, piece.centerPt))
                goRight()
        }
        function tryGoDown() {
            if ( listModel.canGoDown(piece.shape, piece.centerPt) )
                goDown()
            else {
                dropping = false
                landed = true
            }
        }
        // private
        function goDown() {
            clear()
            centerPt.y += 1
            fillAndUpdate()
        }

        function goLeft() {
            clear()
            centerPt.x -= 1
            fillAndUpdate()
        }

        function goRight() {
            clear()
            centerPt.x += 1
            fillAndUpdate()
        }

        function next() {
            clear()
            rotate()
        }

        function fillAndUpdate() {
            fill()
            update()
        }

        function update() {
            listModel.dataChanged()
        }

        function fill() {
            listModel.fillPiece(shape, centerPt)
        }

        function clear() {
            listModel.clearPiece(shape, centerPt)
        }

    }

    Board2 {
        id: board

        /*readonly */property size size: board.model.size
        readonly property point startPoint: board.model.startPoint

        anchors.centerIn: parent
        anchors.fill: parent

        cellSize: 20
        spacing: 2

        model: listModel
    }

    GameBoardListModel {
        id: listModel

        property int score: 0
//        readonly property point startPoint: Qt.point(5, 1)

//        size: Qt.size(10, 20)

        signal dataChanged()

        onLinesCleared: {
            score += calcScore(lines)
            clearSE.play()
        }

        function calcScore(lines) {
            if (lines === 1)
                return 100
            else if (lines === 2)
                return 200
            else if (lines === 3)
                return 400
            else if (lines === 4)
                return 800
            else
                return 0
        }
    }

}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
