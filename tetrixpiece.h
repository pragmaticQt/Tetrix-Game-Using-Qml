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
    Q_PROPERTY(QVariantList points READ points NOTIFY pointsChanged)
    Q_PROPERTY(int minX READ minX)
    Q_PROPERTY(int maxX READ maxX)
    Q_PROPERTY(int minY READ minY)
    Q_PROPERTY(int maxY READ maxY)

public:
    TetrixPiece(QObject *parent = nullptr): QObject(parent) { setShape(TetrixShape::NoShape); }



    static constexpr int TetrixPieceBlocks  = 4;
    static const QPoint CoordinatesTable[TetrixShape::Count][TetrixPieceBlocks];

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
        auto randomShapeExceptNoShape = QRandomGenerator::global()->bounded(TetrixShape::Count - 1) + 1;
        setShape(TetrixShape::Value(randomShapeExceptNoShape));
    }

    Q_INVOKABLE void rotate() {
        if (m_shape == TetrixShape::SquareShape || m_shape == TetrixShape::NoShape)
            return;

        if (TetrixShape::ZShape <= m_shape && m_shape < TetrixShape::TShape)
            setShape(TetrixShape::Value(TetrixShape::ZShape + int((m_shape - TetrixShape::ZShape) + (TetrixShape::ZShape1 - TetrixShape::ZShape)) % int(TetrixShape::TShape - TetrixShape::ZShape)));
        else if (TetrixShape::TShape <= m_shape && m_shape < TetrixShape::TetrixShape::Count)
            setShape(TetrixShape::Value(TetrixShape::TShape + int((m_shape - TetrixShape::TShape) + (TetrixShape::TShape1 - TetrixShape::TShape)) % int(TetrixShape::TetrixShape::Count - TetrixShape::TShape)));

        emit pointsChanged();
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
    void pointsChanged();

private:
    void setX(int index, int x) { m_coords[index].setX(x); }
    void setY(int index, int y) { m_coords[index].setY(y); }
    int x(int index) const { return m_coords[index].x(); }
    int y(int index) const { return m_coords[index].y(); }

    //    void rotateLeft() {
    //        for (int i = 0; i < 4; ++i) {
    //            auto tmpY = y(i);
    //            auto tmpX = -x(i);
    //            setX(i, tmpY);
    //            setY(i, tmpX);
    //        }
    //    }
    //    void rotateRight() {
    //        for (int i = 0; i < 4; ++i) {
    //            auto tmpY = -y(i);
    //            auto tmpX = x(i);
    //            setX(i, tmpY);
    //            setY(i, tmpX);
    //        }
    //    }

    TetrixShape::Value m_shape;
    QVector<QPoint> m_coords{TetrixPieceBlocks};
    //    QPoint m_coords[TetrixPieceBlocks];
};

#endif // TETRIXPIECE_H
