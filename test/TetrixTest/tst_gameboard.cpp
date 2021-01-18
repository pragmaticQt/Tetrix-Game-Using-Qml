#include <QtTest>

// add necessary includes here
#include "gameboard.h"
//#include "tetrixpiece.h"

class tstGameBoard : public QObject
{
    Q_OBJECT

public:
    tstGameBoard(){;}
    ~tstGameBoard(){;}

private slots:
    void initTestCase(){;}
    void cleanupTestCase(){;}
    //        0 1 2 3
    //      0 ----------> X
    //      1 |     |
    //      2 |---(3, 2)
    //      3 |
    //        V Y
    void tc00_None_canGoUp(){
        GameBoardListModel board;
        board.setSize(QSize(4, 4));
        for (auto piece = TetrixShape::NoShape; piece < TetrixShape::Count; piece = TetrixShape::Value(piece + 1))
            QCOMPARE(board.canGoUp(TetrixShape::LShape, QPoint(1, 1)), false);

    }
    void tc01_L_can_Rotate(){
        //       _ _ _      _ _ _
        //      |o|o|_|    |_|_|_|
        //      |_|o|_| -> |o|o|o|
        //      |_|o|_|    |o|_|_|

        GameBoardListModel board;
        board.setSize(QSize(3, 3));
        QCOMPARE(board.canRotate(TetrixShape::LShape, QPoint(1, 1)), true);
    }
    void tc02_L_cannot_Rotate(){
        //       _ _ _      _ _ _
        //      |_|o|o|    |_|_|_|
        //      |_|_|o| -> |_|o|o|o
        //      |_|_|o|    |_|o|_|
        GameBoardListModel board;
        board.setSize(QSize(3, 3));
        QCOMPARE(board.canRotate(TetrixShape::LShape, QPoint(2, 1)), false);
    }
    void tc03_L_can_GoRight(){
        //       _ _ _      _ _ _
        //      |o|o|_|    |_|o|o|
        //      |_|o|_| -> |_|_|o|
        //      |_|o|_|    |_|_|o|

        GameBoardListModel board;
        board.setSize(QSize(3, 3));
        QCOMPARE(board.canGoRight(TetrixShape::LShape, QPoint(1, 1)), true);
    }
    void tc04_L_cannot_GoRight(){
        //       _ _ _      _ _ _
        //      |_|o|o|    |_|_|o|o
        //      |_|_|o| -> |_|_|_|o
        //      |_|_|o|    |_|_|_|o
        GameBoardListModel board;
        board.setSize(QSize(3, 3));
        QCOMPARE(board.canRotate(TetrixShape::LShape, QPoint(2, 1)), false);
    }
    void tc05_L_cannot_GoLeft(){
        //       _ _ _      _ _ _
        //      |o|o|_|   o|o|_|_|
        //      |_|o|_| -> |o|_|_|
        //      |_|o|_|    |o|_|_|

        GameBoardListModel board;
        board.setSize(QSize(3, 3));
        QCOMPARE(board.canGoLeft(TetrixShape::LShape, QPoint(1, 1)), false);
    }
    void tc06_L_can_GoLeft(){
        //       _ _ _      _ _ _
        //      |_|o|o|    |o|o|_|
        //      |_|_|o| -> |_|o|_|
        //      |_|_|o|    |_|o|_|
        GameBoardListModel board;
        board.setSize(QSize(3, 3));
        QCOMPARE(board.canGoLeft(TetrixShape::LShape, QPoint(2, 1)), true);
    }
};

QTEST_APPLESS_MAIN(tstGameBoard)

#include "tst_gameboard.moc"
