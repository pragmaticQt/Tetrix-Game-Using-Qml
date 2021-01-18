#include <QtTest>

// add necessary includes here
#include "tetrixpiece.h"

class tstTetrixPiece : public QObject
{
    Q_OBJECT

public:
    tstTetrixPiece(){;}
    ~tstTetrixPiece(){;}

private slots:
    void initTestCase(){;}
    void cleanupTestCase(){;}

    void tc01_Default_Is_NoShape(){
        TetrixPiece piece;
        QVERIFY(piece.shape()==TetrixShape::NoShape);
    }

    void tc02_SquarePiece_Rotated_ShapeNoChange(){
        TetrixPiece piece {TetrixShape::SquareShape};
        piece.rotate();
        QVERIFY(piece.shape()==TetrixShape::SquareShape);
    }

    void tc03_SetShapeCalled_shapeChanged_Emit(){
        TetrixPiece piece;
        QSignalSpy spy(&piece, SIGNAL(shapeChanged()));
        piece.setShape(TetrixShape::SquareShape);
        QVERIFY(spy.isValid());
//        QVERIFY(spy.wait());
    }

};


QTEST_APPLESS_MAIN(tstTetrixPiece)

#include "tst_tsttetrixpiece.moc"
