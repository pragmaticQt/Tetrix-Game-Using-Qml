#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QAbstractItemModel>
#include <QSize>
#include <QDebug>
#include <vector>
#include <algorithm>

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
            qDebug() << "setSize" << size;
            m_size = size;
            // resize board and fill each cell Empty
            m_board.clear();
            m_board.resize(m_size.height());
            for (auto & cell: m_board) {
                cell.resize(m_size.width());
                std::fill(begin(cell), end(cell), Empty);
            }
            qDebug() << "m_board" << m_board;
        }
    }

    enum State
    {
        Empty = 0,
        Occupied
    };
    Q_ENUM(State)


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

        return qVariantFromValue(getState(index.row(), index.column()));
    }

signals:

private:
    std::vector<std::vector<State>> m_board;
    QSize m_size;
};

#endif // GAMEBOARD_H
