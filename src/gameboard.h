#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QAbstractItemModel>
#include <QSize>
#include <QPoint>
#include <QDebug>
#include <vector>
#include <algorithm>
#include "tetrixpiece.h"

class Cell {
    Q_GADGET
public:
    enum Value {
        Undefined = -1, // out of range
        Empty = 0,      // inrange,
        Occupied,       // piece is drawn here temperarily
        Filled          // piece can't move across it
    };
    Q_ENUM(Value)
};

class GameBoardListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize)

public:
    GameBoardListModel(QObject *parent = nullptr):
        QAbstractListModel(parent)
    {}

    QSize size() const { return QSize(m_board.size() > 0 ? m_board[0].size() : 0, m_board.size()); }
    void setSize(QSize size) {
        if (this->size() != size) {
            // resize board and fill each cell Empty
            auto rows = size.height();
            auto columns = size.width();

            m_board.clear();
            m_board.reserve(rows);
            for (auto i = 0; i < rows; ++i) {
                QVector<QVariant> v(columns, Cell::Empty);
                m_board.push_back(v.toList());
            }
        }
    }


    Q_INVOKABLE bool canRotate(int shape, const QPoint& originPt) const {

        auto nextShape = TetrixPiece::nextShapeIfRotated((TetrixShape::Value)shape);
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)nextShape];

        return std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(pt);}) &&
               std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return getState(pt)==Cell::Filled;});
    }
    // never go up
    Q_INVOKABLE bool canGoUp(int shape, const QPoint& originPt) const {
        Q_UNUSED(shape)
        Q_UNUSED(originPt)
        return false;
    }
    Q_INVOKABLE bool canGoLeft(int shape, const QPoint& originPt) const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];

        return std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(pt);})&&
               std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange({pt.x()-1, pt.y()});})&&
               std::none_of(std::begin(coords), std::end(coords),
                           [&](const auto &point){ auto pt = point + originPt;return this->getState({pt.x()-1, pt.y()}) == Cell::Filled;});
    }
    Q_INVOKABLE bool canGoRight(int shape, const QPoint& originPt) const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];

        return std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(pt);})&&
               std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange({pt.x()+1, pt.y()});})&&
               std::none_of(std::begin(coords), std::end(coords),
                           [&](const auto &point){ auto pt = point + originPt;return this->getState({pt.x()+1, pt.y()}) == Cell::Filled;});
    }
    // piece movement detection
    Q_INVOKABLE bool canGoDown(int shape, const QPoint& originPt) const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];

        return std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(pt);})&&
               std::none_of(std::begin(coords), std::end(coords),
                            [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(QPoint(pt.x(), pt.y()+1));})&&
               std::none_of(std::begin(coords), std::end(coords),
                           [&](const auto &point){ auto pt = point + originPt;return this->getState(QPoint(pt.x(), pt.y()+1)) == Cell::Filled;});
    }
    //
    // cells operations
    Q_INVOKABLE  void landPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        //        QVector<bool> affectedRows;
        for ( auto &point: coords ) {
            auto pt = point + originPt;

            if (outOfRange(pt))
                continue;

            m_board[pt.y()][pt.x()] = Cell::Filled;

        }

        auto linesFilled = 0;

        for ( auto &row: m_board ) {
            auto b = std::all_of(std::begin(row), std::end(row), [&](const auto&cell) {return cell==Cell::Filled;});
            if (b) {
                ++linesFilled;
                resetRow(row);
            }
        }

        if (linesFilled) {
            std::stable_partition(std::begin(m_board), std::end(m_board), [](const auto& row) {
                return std::all_of(std::begin(row), std::end(row), [&](const auto&cell) {return cell==Cell::Empty;});
            });
            emit linesCleared(linesFilled);
        }

    }

    Q_INVOKABLE  void fillPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        for ( auto &point: coords ) {
            auto pt = point + originPt;

            if (outOfRange(pt))
                continue;

            m_board[pt.y()][pt.x()] = Cell::Occupied;
        }
    }

    Q_INVOKABLE  void clearPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        for ( auto &point: coords ) {
            auto pt = point + originPt;

            if (outOfRange(pt))
                continue;

            m_board[pt.y()][pt.x()] = Cell::Empty;
        }
    }

    Q_INVOKABLE  void resetAll() { for ( auto &row: m_board ) resetRow(row); }

    void resetRow(auto &row) {
        std::fill(std::begin(row), std::end(row), Cell::Empty);
    }
    // edge detections
    bool outRight(const QPoint& pt) const {
        if( pt.x() >= size().width())
            return true;

        return false;
    }
    bool outLeft(const QPoint& pt) const {
        if( pt.x() < 0)
            return true;

        return false;
    }
    bool outTop(const QPoint& pt) const {
        if( pt.y() <0)
            return true;

        return false;
    }
    bool outBottom(const QPoint& pt) const {
        if( pt.y() >= size().height())
            return true;

        return false;
    }
    bool outOfRange(const QPoint& pt) const {
        if(outBottom(pt) || outTop(pt) || outLeft(pt) || outRight(pt))
            return true;

        return false;
    }
    Q_INVOKABLE int/*Cell::Value*/ getState(const QPoint& pt) const {

        if (outOfRange(pt)) return Cell::Undefined;

        return Cell::Value(m_board[pt.y()][pt.x()].toInt());
    }
    Q_INVOKABLE void setState(const QPoint& pt, /*Cell::Value*/int state) {

        if(outOfRange(pt) || state==Cell::Undefined || getState(pt) == state)
            return;

        m_board[pt.y()][pt.x()] = state;
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const {
        Q_UNUSED(parent)
        return m_board.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const {
        if(!index.isValid())
            return QVariant();

        if(role != Qt::DisplayRole)
            return QVariant();

        if( index.row() >= m_board.size() || index.row() < 0)
            return QVariant();

        return QVariant::fromValue(m_board.at(index.row()));
    }

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                  const QModelIndex &destinationParent, int destinationChild) Q_DECL_OVERRIDE {
        Q_UNUSED(sourceParent)
        Q_UNUSED(sourceRow)
        Q_UNUSED(count)
        Q_UNUSED(destinationParent)
        Q_UNUSED(destinationChild)
        return false;
    }

signals:
    void linesCleared(int lines);

private:
    QList<QVariantList> m_board;

};
#endif // GAMEBOARD_H
