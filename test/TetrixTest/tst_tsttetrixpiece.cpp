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
        QCOMPARE(spy.count(), 1);
        //        QVERIFY(spy.wait());
    }
    void tc04_SquarePiece_Rotated_shapeChanged_NoEmit(){
        TetrixPiece piece {TetrixShape::SquareShape};
        QSignalSpy spy(&piece, SIGNAL(shapeChanged()));

        piece.rotate();

        QVERIFY(spy.isValid());
        QCOMPARE(spy.count(), 0);
    }
    void tc05_ZShapePiece_Rotated_shapeChanged_Emit(){
        TetrixPiece piece {TetrixShape::ZShape};
        QSignalSpy spy(&piece, SIGNAL(shapeChanged()));

        piece.rotate();

        QVERIFY(spy.isValid());
        QCOMPARE(spy.count(), 1);
    }
    void tc06_ZShape_and_ZShape1_WhenRotated_Exchanged(){
        TetrixPiece piece {TetrixShape::ZShape};

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::ZShape1);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::ZShape);
    }
    void tc07_SShape_and_SShape1_WhenRotated_Exchanged(){
        TetrixPiece piece {TetrixShape::SShape};

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::SShape1);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::SShape);
    }
    void tc08_LineShape_and_LineShape1_WhenRotated_Exchanged(){
        TetrixPiece piece {TetrixShape::LineShape};

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::LineShape1);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::LineShape);
    }
    void tc09_TShape_TShape1_TShape2_TShape3_WhenRotated_Exchanged(){
        TetrixPiece piece {TetrixShape::TShape};

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::TShape1);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::TShape2);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::TShape3);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::TShape);
    }
    void tc10_LShape_LShape1_LShape2_LShape3_WhenRotated_Exchanged(){
        TetrixPiece piece {TetrixShape::LShape};

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::LShape1);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::LShape2);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::LShape3);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::LShape);
    }
    void tc11_MirroredLShape_MirroredLShape1_MirroredLShape2_MirroredLShapee3_WhenRotated_Exchanged(){
        TetrixPiece piece {TetrixShape::MirroredLShape};

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::MirroredLShape1);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::MirroredLShape2);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::MirroredLShape3);

        piece.rotate();

        QCOMPARE(piece.shape(), TetrixShape::MirroredLShape);
    }
};


QTEST_APPLESS_MAIN(tstTetrixPiece)

#include "tst_tsttetrixpiece.moc"
