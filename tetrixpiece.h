#ifndef TETRIXPIECE_H
#define TETRIXPIECE_H

#include <QObject>
#include <QtCore>
#include <iterator>
#include <algorithm>

using std::min_element;
using std::max_element;
using std::cbegin;
using std::cend;
//! [0]
class TetrixPiece : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TetrixShape shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(QVariantList points READ points NOTIFY pointsChanged)
    Q_PROPERTY(int minX READ minX)
    Q_PROPERTY(int maxX READ maxX)
    Q_PROPERTY(int minY READ minY)
    Q_PROPERTY(int maxY READ maxY)

public:
    TetrixPiece(QObject *parent = nullptr): QObject(parent) { setShape(TetrixShape::NoShape); }

    enum TetrixShape { NoShape = 0,
                       ZShape,
                       SShape,
                       LineShape,
                       TShape,
                       SquareShape,
                       LShape,
                       MirroredLShape };
    Q_ENUM(TetrixShape)

    static constexpr int TetrixShapeCount  = 8;
    static constexpr int TetrixPieceBlocks  = 4;
    static const QPoint coordsTable_[TetrixShapeCount][TetrixPieceBlocks];

    Q_INVOKABLE void setRandomShape(){
        auto randomShapeExceptNoShape = QRandomGenerator::global()->bounded(TetrixShapeCount - 1) + 1;
        setShape(TetrixShape(randomShapeExceptNoShape));
    }

    Q_INVOKABLE void rotate() {
        if (pieceShape == SquareShape || pieceShape == NoShape)
            return;

        if (pieceShape == ZShape || pieceShape == SShape || pieceShape == LineShape)
            if (coordsTable_[pieceShape][0] == coords[0])
                rotateLeft();
            else
                rotateRight();
        else
            rotateLeft();

        emit pointsChanged();
    }

    void setShape(TetrixShape shape){

        if (shape != pieceShape) {

            for (int i = 0; i < TetrixPieceBlocks; i++) {
                coords[i] = coordsTable_[shape][i];
            }

            pieceShape = shape;
            emit shapeChanged();
        }
    }
    TetrixShape shape() const { return pieceShape; }

    QVariantList points() const { return QVariantList{ coords[0], coords[1], coords[2], coords[3]}; }


    int minX() const {
        auto result = min_element(cbegin(coords), cend(coords),
                                  [](const auto& e1, const auto& e2) { return e1.x() < e2.x();});
        return result->x();
    }
    int maxX() const {
        auto result = max_element(cbegin(coords), cend(coords),
                                  [](const auto& e1, const auto& e2) { return e1.x() > e2.x();});
        return result->x();
    }
    int minY() const {
        auto result = min_element(cbegin(coords), cend(coords),
                                  [](const auto& e1, const auto& e2) { return e1.y() < e2.y();});
        return result->y();
    }
    int maxY() const {
        auto result = max_element(cbegin(coords), cend(coords),
                                  [](const auto& e1, const auto& e2) { return e1.y() > e2.y();});
        return result->y();
    }

signals:
    void shapeChanged();
    void pointsChanged();

private:
    void setX(int index, int x) { coords[index].setX(x); }
    void setY(int index, int y) { coords[index].setY(y); }
    int x(int index) const { return coords[index].x(); }
    int y(int index) const { return coords[index].y(); }

    void rotateLeft() {
        for (int i = 0; i < 4; ++i) {
            auto tmpY = y(i);
            auto tmpX = -x(i);
            setX(i, tmpY);
            setY(i, tmpX);
        }
    }
    void rotateRight() {
        for (int i = 0; i < 4; ++i) {
            auto tmpY = -y(i);
            auto tmpX = x(i);
            setX(i, tmpY);
            setY(i, tmpX);
        }
    }

    TetrixShape pieceShape;
    QPoint coords[TetrixPieceBlocks];
};

#endif // TETRIXPIECE_H
