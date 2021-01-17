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

    QSize size() const { return m_size; }
    void setSize(QSize size) {

        if (m_size != size) {
            //            qDebug() << "setSize" << size;
            m_size = size;
            // resize board and fill each cell Empty
            m_board.clear();
            auto rows = m_size.height();
            auto columns = m_size.width();
            m_board.reserve(rows);
            for (auto i = 0; i < rows; ++i) {
                QVector<QVariant> v(columns, Empty);
                m_board.push_back(v.toList());
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
    // piece movement detection
    Q_INVOKABLE bool canGoDown(int shape, const QPoint& originPt) const {
        TetrixPiece piece;
        piece.setShape(TetrixShape::Value(shape));
        if (piece.maxY() + originPt.y() < m_size.height() - 1)
            return true;
        return false;
    }
    //
    // cells operations
    Q_INVOKABLE  void fillPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        for ( auto &point: coords ) {
            auto pt = point + originPt;
            m_board[pt.y()][pt.x()] = Occupied;
        }
    }

    Q_INVOKABLE  void clearPiece(int shape, const QPoint& originPt){
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape];
        for ( auto &point: coords ) {
            auto pt = point + originPt;
            m_board[pt.y()][pt.x()] = Empty;
        }
    }

    Q_INVOKABLE  void resetAll() { for ( auto &row: m_board ) std::fill(std::begin(row), std::end(row), Empty); }
    //
    Q_INVOKABLE State getState(int row, int col) const {
        if((size_t) row >= m_board.size() || row < 0)
            return Empty;

        if((size_t) col >= m_board[0].size() || col < 0)
            return Empty;

        return State(m_board[row][col].toInt());
    }
    Q_INVOKABLE void setState(int row, int col, State state) {

        if(getState(row, col) == state)
            return;
        //        qDebug() << "setState" << row << col << state;
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
    QSize m_size;
};
#endif // GAMEBOARD_H
