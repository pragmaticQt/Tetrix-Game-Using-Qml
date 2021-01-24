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
        Any = -2,
        Undefined = -1, // out of range
        Empty = 0,      // inrange,
        Occupied,       // piece is drawn here temperarily
        Filled,         // piece can't move across it
        Ghost
    };
    Q_ENUM(Value)
};

class GameBoardListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size CONSTANT)
    Q_PROPERTY(QPoint startPoint READ startPoint CONSTANT)
    Q_PROPERTY(QPoint tetrimino READ tetrimino NOTIFY tetriminoChanged)
    Q_PROPERTY(int shape READ shape NOTIFY shapeChanged)
    Q_PROPERTY(int nextShape READ nextShape NOTIFY nextShapeChanged)

public:
    GameBoardListModel(QObject *parent = nullptr):
        QAbstractListModel(parent)
    {
        m_board.reserve(20);
        for (auto i = 0; i < 20; ++i) {
            QVector<QVariant> v(10, Cell::Empty);
            m_board.push_back(v.toList());
        }
        setTetrimino(m_startPoint);
    }

    Q_INVOKABLE void start() {
        resetAll();
        m_piece.setCenter(m_startPoint);
        m_piece.setShape(TetrixPiece::getRandomShape());
        fillPiece();
        setNextShape();
    }
    //
    Q_INVOKABLE  void rotateTetrimino() {
        if (canRotate()) {
            clearPiece();
            m_piece.rotate();
            setShape(m_piece.shape());
            fillPiece();
        }
    }
    // tetrimino operations
    Q_INVOKABLE  void goLeft(){
        if (canGoLeft()) {
            clearPiece();
            setTetrimino({m_piece.center().x()-1, m_piece.center().y()});
            fillPiece();
        }
    }
    Q_INVOKABLE  void goRight(){
        if (canGoRight()) {
            clearPiece();
            setTetrimino({m_piece.center().x()+1, m_piece.center().y()});
            fillPiece();
        }
    }
    Q_INVOKABLE  void softDrop(){
        if (canGoDown()) {
            clearPiece();
            setTetrimino({m_piece.center().x(), m_piece.center().y() + 1});
            fillPiece();
        }
        else
            landPiece();
    }
    // drop tetrimino directly to its final position
    Q_INVOKABLE  void hardDrop(){
        auto distance = canGoDownMost();
        if (distance) {
            clearPiece();
            setTetrimino({m_piece.center().x(), m_piece.center().y() + distance});
            fillPiece();
            landPiece();
        }
    }
    Q_INVOKABLE int/*Cell::Value*/ getState(const QPoint& pt) const {

        if (outOfRange(pt)) return Cell::Undefined;

        return Cell::Value(m_board[pt.y()][pt.x()].toInt());
    }
    //
    int shape() const { return m_piece.shape(); }
private:
    void setShape(int shape) {
        if (shape != this->shape()) {
            m_piece.setShape((TetrixShape::Value)shape);
            emit shapeChanged(this->shape());
        }
    }
public:
    int nextShape() const { return m_nextPiece; }
    void setNextShape() {
        m_nextPiece = TetrixPiece::getRandomShape();
        emit nextShapeChanged(nextShape());
    }

    QSize size() const { return QSize(m_board.size() > 0 ? m_board[0].size() : 0, m_board.size()); }

    QPoint startPoint() const { return m_startPoint; }
    const QPoint& tetrimino() const { return m_piece.center(); }
    void setTetrimino(const QPoint& point) {
        if (point != tetrimino()) {
            clearPiece();

            m_piece.setCenter(point);
            emit tetriminoChanged(tetrimino());

            fillPiece();
        }
    }

    bool canRotate() const {

        auto nextShape = TetrixPiece::nextShapeIfRotated((TetrixShape::Value)shape());
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)nextShape];

        return std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange(pt);}) &&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return getState(pt)==Cell::Filled;});
    }
    // never go up
    bool canGoUp() const {
        return false;
    }
    bool canGoLeft() const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];

        return std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange(pt);})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange({pt.x()-1, pt.y()});})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->getState({pt.x()-1, pt.y()}) == Cell::Filled;});
    }
    bool canGoRight() const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];

        return std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange(pt);})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange({pt.x()+1, pt.y()});})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->getState({pt.x()+1, pt.y()}) == Cell::Filled;});
    }
    // piece movement detection
    int canGoDownMost() const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];
        QPoint startPt = m_piece.center();

        while (std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + startPt;return this->outOfRange(pt);})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + startPt;return this->outOfRange(QPoint(pt.x(), pt.y()+1));})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + startPt;return this->getState(QPoint(pt.x(), pt.y()+1)) == Cell::Filled;}))
            startPt.ry() += 1;
        return startPt.y() - m_piece.center().y();
    }
    bool canGoDown() const {

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];

        return std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange(pt);})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->outOfRange(QPoint(pt.x(), pt.y()+1));})&&
               std::none_of(std::cbegin(coords), std::cend(coords),
                            [&](const auto &point){ auto pt = point + m_piece.center();return this->getState(QPoint(pt.x(), pt.y()+1)) == Cell::Filled;});
    }

    void landPiece(){
        replacePiece(Cell::Any, Cell::Filled);
        emit pieceLanded();

        // only current piece area needs detection
        QVariantList linesFilled;

        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];
        for ( const auto &point: coords ) {
            QPoint pt = point + m_piece.center();
            if (!linesFilled.contains(pt.y())) {
                auto b = std::all_of(std::cbegin(m_board[pt.y()]), std::cend(m_board[pt.y()]),
                                     [&](const auto&cell) {return cell==Cell::Filled;});
                if (b) {
                    linesFilled.push_back(pt.y());
                    resetRow(m_board[pt.y()]);
                }
            }
        }

        m_piece.setShape(TetrixShape::NoShape);

        // remove these black lines upward
        if (linesFilled.size()) {
            std::stable_partition(std::begin(m_board), std::end(m_board), [](const auto& row) {
                return std::all_of(std::cbegin(row), std::cend(row), [&](const auto&cell) {return cell==Cell::Empty;});
            });
            emit linesCleared(linesFilled);
        }

        // check if game over
        if (getState(m_startPoint) == Cell::Filled) {
            emit gameOver();
            return;
        }

        // start over
        m_piece.setShape((TetrixShape::Value)nextShape());
        setNextShape();
        m_piece.setCenter(m_startPoint);
    }

private:
    void replaceGhost(Cell::Value oldValue, Cell::Value newValue) {
        auto distance = canGoDownMost();
        if (distance > 1) {
            const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];
            for ( const auto &point: coords ) {
                QPoint pt = point + m_piece.center();
                pt.ry() += distance;

                if (outOfRange(pt))
                    continue;

                if (getState(pt) == oldValue) setState(pt, newValue);
            }
        }
    }

    void replacePiece(Cell::Value oldValue, Cell::Value newValue) {
        const auto& coords = TetrixPiece::CoordinatesTable[(TetrixShape::Value)shape()];
        for ( const auto &point: coords ) {
            QPoint pt = point + m_piece.center();

            if (outOfRange(pt))
                continue;

            if (getState(pt) == oldValue || Cell::Any == oldValue) setState(pt, newValue);
        }
    }
public:
    void fillPiece(){
        if (shape() != TetrixShape::NoShape) {
            replacePiece(Cell::Empty, Cell::Occupied);
            replaceGhost(Cell::Empty, Cell::Ghost);
        }
    }
    void clearPiece(){
        replaceGhost(Cell::Ghost, Cell::Empty);
        replacePiece(Cell::Occupied, Cell::Empty);
    }

    void resetAll() {
        for ( auto &row: m_board ) resetRow(row);
    }

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

    void setState(const QPoint& pt, /*Cell::Value*/int state) {

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
    void linesCleared(QVariantList lines);
    void gameOver();
    void pieceLanded();

    void tetriminoChanged(QPoint tetriminoPoint);
    void shapeChanged(int tetriminoShape);
    void nextShapeChanged(int nextShape);


private:
    QList<QVariantList> m_board;

    const QPoint m_startPoint {5, 1};

    TetrixPiece m_piece;
    int m_nextPiece {0};
};
#endif // GAMEBOARD_H
