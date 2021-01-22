import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtMultimedia 5.0
import io.qt.examples.Tetrix 1.0

Frame {
    id: root

    property int lifeCycle: Game.LifeCycle.Ready
    readonly property alias nextShape: listModel.nextShape
    readonly property alias score: listModel.score

    enum LifeCycle {
        Ready,
        Running,
        Paused,
        Done
    }

    signal gameStarted()
    signal gamePaused()
    signal gameResumed()
    signal gameOver()

    property alias logic: conn.target

    width: board.cellSize * listModel.size.width + (listModel.size.width - 1) * board.spacing + 24
    height: board.cellSize * listModel.size.height + (listModel.size.height - 1) * board.spacing + 24

    Keys.onPressed: {
        if (event.key === Qt.Key_Space) piece.hardDrop()

        if (event.key === Qt.Key_Up) piece.rotate()

        if (event.key === Qt.Key_Left) piece.goLeft()

        if (event.key === Qt.Key_Right) piece.goRight()

        if (event.key === Qt.Key_Down) piece.softDrop()

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
            listModel.start()
            listModel.dataChanged()

            piece.resume()
            root.lifeCycle = Game.Running
            bkgMusic.play()

            root.gameStarted()
        }
        onPauseGame: {
            piece.pause()
            root.lifeCycle = Game.Paused
            bkgMusic.pause()

            root.gamePaused()
        }
        onResumeGame: {
            piece.resume()
            root.lifeCycle = Game.Running
            bkgMusic.play()

            root.gameResumed()
        }
        onStopGame: {
            piece.pause()
            root.lifeCycle = Game.Done
            bkgMusic.stop()
            gameoverSE.play()

            root.gameOver()
        }
    }

    Timer {
        id: timer

        repeat: true
        running: piece.isDropping
        interval: 500
        onTriggered: piece.softDrop()
    }

    QtObject {// wrap a piece inside model
        id: piece

        property bool isDropping: false

        function resume() {
            isDropping = true
        }

        function pause() {
            isDropping = false
        }

        function rotate() {
            listModel.rotateTetrimino()
            update()
        }

        function hardDrop() {
            listModel.hardDrop()
            update()
        }

        function softDrop() {
            listModel.softDrop()
            update()
        }

        function goLeft() {
            listModel.goLeft()
            update()
        }

        function goRight() {
            listModel.goRight()
            update()
        }

        function update() {
            listModel.dataChanged()
        }
    }

    Board2 {
        id: board

        anchors.centerIn: parent
        anchors.fill: parent

        cellSize: 20
        spacing: 2

        model: listModel
    }

    GameBoardModel {
        id: listModel
        property int score: 0

        signal dataChanged()

        onLinesCleared: {
            score += calcScore(lines.length)
            clearSE.play()
        }

        Component.onCompleted: {
            listModel.gameOver.connect(gameLogic.stopGame)
            listModel.pieceLanded.connect(function(){
                landSE.play()
                piece.resume()
            })
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
