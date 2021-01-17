#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QAbstractItemModel>
#include <QSize>
#include <QPoint>
#include <QDebug>
#include <vector>
#include <algorithm>
#include "tetrixpiece.h"

class GameBoard : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize)

public:
    GameBoard(QObject *parent = nullptr):
        QAbstractTableModel(parent)
    {}

    QSize size() const { return m_size; }
    void setSize(QSize size) {

        if (m_size != size) {
            //            qDebug() << "setSize" << size;
            m_size = size;
            // resize board and fill each cell Empty
            m_board.clear();
            m_board.resize(m_size.height());
            for (auto & cell: m_board) {
                cell.resize(m_size.width());
                std::fill(begin(cell), end(cell), Empty);
            }
            //            qDebug() << "m_board" << m_board;
        }
    }

    enum State
    {
        Empty = 0,
        Occupied
    };
    Q_ENUM(State)

    Q_INVOKABLE  void fill(QVariantList points) {
        //        qDebug() << points;
        for ( auto &point: points ) {
            auto pt = point.toPoint();
            m_board[pt.y()][pt.x()] = Occupied;
        }
    }
    Q_INVOKABLE  void reset(QVariantList points) {
        //        qDebug() << points;
        for ( auto &point: points ) {
            auto pt = point.toPoint();
            m_board[pt.y()][pt.x()] = Empty;
        }
    }
    Q_INVOKABLE  void resetAll() { for ( auto &row: m_board ) std::fill(begin(row), end(row), Empty); }
    Q_INVOKABLE State getState(int row, int col) const {
        if((size_t) row >= m_board.size() || row < 0)
            return Empty;

        if((size_t) col >= m_board.at(0).size() || col < 0)
            return Empty;

        return m_board.at(row).at(col);
    }
    Q_INVOKABLE void setState(int row, int col, State state) {

        if(getState(row, col) == state)
            return;
        //        qDebug() << "setState" << row << col << state;
        m_board.at(row).at(col) = state;
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const {
        return m_board.size();
    }
    int columnCount(const QModelIndex& parent = QModelIndex()) const {
        return m_board.at(0).size();
    }
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const {
        if(!index.isValid())
            return QVariant();

        if(role != Qt::DisplayRole)
            return QVariant();

        if((size_t) index.row() >= m_board.size() || index.row() < 0)
            return QVariant();

        if((size_t) index.column() >= m_board.at(0).size() || index.column() < 0)
            return QVariant();

        return QVariant::fromValue(getState(index.row(), index.column()));
    }

signals:

private:
    std::vector<std::vector<State>> m_board;
    QSize m_size;
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
                QVector<QVariant> v(columns, Empty);
                m_board.push_back(v.toList());
            }
        }
    }

    enum State // for cell
    {
        Undefined = -1, // out of range
        Empty = 0,      // inrange,
        Occupied,       // piece is drawn here temperarily
        Filled          // piece can't move across it
    };
    Q_ENUM(State)
    // piece movement detection
    Q_INVOKABLE bool canGoDown(int shape, const QPoint& originPt) const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];

        return std::none_of(std::begin(coords), std::end(coords),
                               [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(pt.y(), pt.x());})&&
               std::none_of(std::begin(coords), std::end(coords),
                               [&](const auto &point){ auto pt = point + originPt;return this->outOfRange(pt.y()+1, pt.x());})&&
               std::all_of(std::begin(coords), std::end(coords),
                               [&](const auto &point){ auto pt = point + originPt;return this->getState(pt.y()+1, pt.x()) != Filled;});
    }
    //
    // cells operations
    Q_INVOKABLE  void fillPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        for ( auto &point: coords ) {
            auto pt = point + originPt;

            if (outOfRange(pt.y(), pt.x()))
                continue;

            m_board[pt.y()][pt.x()] = Occupied;
        }
    }

    Q_INVOKABLE  void clearPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        for ( auto &point: coords ) {
            auto pt = point + originPt;

            if (outOfRange(pt.y(), pt.x()))
                continue;

            m_board[pt.y()][pt.x()] = Empty;
        }
    }

    Q_INVOKABLE  void resetAll() { for ( auto &row: m_board ) std::fill(std::begin(row), std::end(row), Empty); }
    //
    bool outRight(int row, int col) const {
        if((size_t) col >= size().width())
            return true;

        return false;
    }
    bool outLeft(int row, int col) const {
        if((size_t) col < 0)
            return true;

        return false;
    }
    bool outTop(int row, int col) const {
        if((size_t) row <0)
            return true;

        return false;
    }
    bool outBottom(int row, int col) const {
        if((size_t) row >= size().height())
            return true;

        return false;
    }
    bool outOfRange(int row, int col) const {
        if(outBottom(row, col) || outTop(row, col) || outLeft(row, col) || outRight(row, col))
            return true;

        return false;
    }
    Q_INVOKABLE State getState(int row, int col) const {
        if (outOfRange(row, col)) return Undefined;

        return State(m_board[row][col].toInt());
    }
    Q_INVOKABLE void setState(int row, int col, State state) {

        if(outOfRange(row, col) || state==Undefined || getState(row, col) == state)
            return;

        m_board[row][col] = state;
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const {
        return m_board.size();
    }
    Q_INVOKABLE QVariantList getRow(int row) const {
        return m_board.at(row);
    }
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const {
        if(!index.isValid())
            return QVariant();

        if(role != Qt::DisplayRole)
            return QVariant();

        if((size_t) index.row() >= m_board.size() || index.row() < 0)
            return QVariant();

        return QVariant::fromValue(m_board.at(index.row()));
    }

signals:
    //    dataChanged();

private:
    QList<QVariantList> m_board;
};
#endif // GAMEBOARD_H
