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
    static const QPoint CoordinatesTable[TetrixShapeCount][TetrixPieceBlocks];

    Q_INVOKABLE QVariantMap getBoundOfNextRotation() const {
        TetrixPiece result;

        result.m_coords = m_coords;
        result.setShape(m_shape);
        result.rotate();

        QMap<QString, QVariant> map;
        map["minX"] = result.minX();
        map["maxX"] = result.maxX();
        map["minY"] = result.minY();
        map["maxY"] = result.maxY();

        return map;
    }

    Q_INVOKABLE void setRandomShape(){
        auto randomShapeExceptNoShape = QRandomGenerator::global()->bounded(TetrixShapeCount - 1) + 1;
        setShape(TetrixShape(randomShapeExceptNoShape));
    }

    Q_INVOKABLE void rotate() {
        if (m_shape == SquareShape || m_shape == NoShape)
            return;

        if (m_shape == ZShape || m_shape == SShape || m_shape == LineShape)
            if (CoordinatesTable[m_shape][0] == m_coords[0])
                rotateLeft();
            else
                rotateRight();
        else
            rotateLeft();

        emit pointsChanged();
    }

    void setShape(TetrixShape shape){

        if (shape != m_shape) {

            for (int i = 0; i < TetrixPieceBlocks; i++) {
                m_coords[i] = CoordinatesTable[shape][i];
            }

            m_shape = shape;
            emit shapeChanged();
        }
    }
    TetrixShape shape() const { return m_shape; }

    QVariantList points() const { return QVariantList{ m_coords[0], m_coords[1], m_coords[2], m_coords[3]}; }


    int minX() const {
        auto result = min_element(cbegin(m_coords), cend(m_coords),
                                  [](const auto& e1, const auto& e2) { return e1.x() < e2.x();});
        return result->x();
    }
    int maxX() const {
        auto result = max_element(cbegin(m_coords), cend(m_coords),
                                  [](const auto& e1, const auto& e2) { return e1.x() < e2.x();});
        return result->x();
    }
    int minY() const {
        auto result = min_element(cbegin(m_coords), cend(m_coords),
                                  [](const auto& e1, const auto& e2) { return e1.y() < e2.y();});
        return result->y();
    }
    int maxY() const {
        auto result = max_element(cbegin(m_coords), cend(m_coords),
                                  [](const auto& e1, const auto& e2) { return e1.y() < e2.y();});
        return result->y();
    }

signals:
    void shapeChanged();
    void pointsChanged();

private:
    void setX(int index, int x) { m_coords[index].setX(x); }
    void setY(int index, int y) { m_coords[index].setY(y); }
    int x(int index) const { return m_coords[index].x(); }
    int y(int index) const { return m_coords[index].y(); }

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

    TetrixShape m_shape;
    QVector<QPoint> m_coords{TetrixPieceBlocks};
    //    QPoint m_coords[TetrixPieceBlocks];
};

#endif // TETRIXPIECE_H
