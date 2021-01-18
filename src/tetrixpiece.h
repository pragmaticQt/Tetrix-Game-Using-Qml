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
//!

class TetrixShape
{
    Q_GADGET
public:
    enum Value { NoShape = 0,
                 SquareShape,

                 ZShape,
                 SShape,
                 LineShape,

                 ZShape1,
                 SShape1,
                 LineShape1,

                 TShape,
                 LShape,
                 MirroredLShape,
                 TShape1,
                 LShape1,
                 MirroredLShape1,
                 TShape2,
                 LShape2,
                 MirroredLShape2,
                 TShape3,
                 LShape3,
                 MirroredLShape3,
                 Count
    };

    Q_ENUM(Value)
};

class TetrixPiece : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TetrixShape::Value shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(QVariantList points READ points/* NOTIFY pointsChanged*/)
    Q_PROPERTY(int minX READ minX)
    Q_PROPERTY(int maxX READ maxX)
    Q_PROPERTY(int minY READ minY)
    Q_PROPERTY(int maxY READ maxY)

public:
    TetrixPiece(TetrixShape::Value shape = TetrixShape::NoShape, QObject *parent = nullptr):
        QObject(parent), m_shape(shape) { }

    static constexpr int TetrixPieceBlocks  = 4;
    static const QPoint CoordinatesTable[TetrixShape::Count][TetrixPieceBlocks];

    Q_INVOKABLE void setRandomShape(){
        auto randomShapeExceptNoShape = QRandomGenerator::global()->bounded(TetrixShape::Count - 1) + 1;
        setShape(TetrixShape::Value(randomShapeExceptNoShape));
    }

    static TetrixShape::Value nextShapeIfRotated(TetrixShape::Value shape) {
        if (TetrixShape::ZShape <= shape && shape < TetrixShape::TShape)
            return (TetrixShape::Value(TetrixShape::ZShape + int((shape - TetrixShape::ZShape) + (TetrixShape::ZShape1 - TetrixShape::ZShape)) % int(TetrixShape::TShape - TetrixShape::ZShape)));
        else if (TetrixShape::TShape <= shape && shape < TetrixShape::TetrixShape::Count)
            return (TetrixShape::Value(TetrixShape::TShape + int((shape - TetrixShape::TShape) + (TetrixShape::TShape1 - TetrixShape::TShape)) % int(TetrixShape::TetrixShape::Count - TetrixShape::TShape)));
        else//if (m_shape == TetrixShape::SquareShape || m_shape == TetrixShape::NoShape)
            return shape;
    }

    Q_INVOKABLE void rotate() {
        if (m_shape == TetrixShape::SquareShape || m_shape == TetrixShape::NoShape)
            return;

        setShape(nextShapeIfRotated(m_shape));

        emit shapeChanged();
    }

    void setShape(const TetrixShape::Value& shape){

        if (shape != m_shape) {

            for (int i = 0; i < TetrixPieceBlocks; i++) {
                m_coords[i] = CoordinatesTable[shape][i];
            }

            m_shape = shape;
            emit shapeChanged();
        }
    }
    TetrixShape::Value shape() const { return m_shape; }

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
//    void pointsChanged();

private:
    void setX(int index, int x) { m_coords[index].setX(x); }
    void setY(int index, int y) { m_coords[index].setY(y); }
    int x(int index) const { return m_coords[index].x(); }
    int y(int index) const { return m_coords[index].y(); }

    TetrixShape::Value m_shape = TetrixShape::NoShape;
    QVector<QPoint> m_coords {TetrixPieceBlocks};
};

#endif // TETRIXPIECE_H