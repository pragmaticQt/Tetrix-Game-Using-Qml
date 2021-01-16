#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tetrixpiece.h"
#include "gameboard.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    GameBoard board;
//    board.setSize(QSize(2, 3));

    qmlRegisterType<TetrixPiece>("io.qt.examples.Tetrix", 1,0, "TetrixPiece");
    qmlRegisterType<GameBoard>("io.qt.examples.Tetrix", 1,0, "GameBoard");

//    GameBoard gameBoard;
//    gameBoard.setState(0, 0, GameBoard::Occupied);
//    gameBoard.setState(0, 1, GameBoard::Occupied);
//    gameBoard.setState(1, 1, GameBoard::Occupied);
//    gameBoard.setState(1, 2, GameBoard::Occupied);

    QQmlApplicationEngine engine;

//    QQmlContext *ctxt = engine.rootContext();
//    ctxt->setContextProperty("myGameBoard", &gameBoard);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
