// Danie Walker
// Johnny Scholtz

#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QtMultimedia/QSound>

struct BoardItem
{
    QLabel *label;
    int colour;
};

struct ItemBlock
{
    int x;
    int y;
    int colour;
};

class TetrisGame : public QWidget
{
    Q_OBJECT
public:
    explicit TetrisGame(QWidget *parent = nullptr);
    TetrisGame(int, QWidget *parent = nullptr);
    ~TetrisGame();

public slots:
    void pauseGame();
    void timertick();
    void setMP(bool, bool = false, QString = "");

private:
    int boardX;
    int boardY;
    int nextBlock;
    bool isBomb;

    int timerCount;
    bool gameisover;

    bool isMP;
    QString mbID;
    bool isHost;
    QNetworkAccessManager *nm;
    QTimer *MPtimer;

    int score;

    BoardItem **board;
    QList<ItemBlock> item;
    QTimer *timer;
    bool mustspawn;
    bool lineIsFull;
    int lastspawn;
    bool hardDropped;
    int countHeight = 0;

    QSound *lineclearsound;
    QSound *gameoversound;
    QSound *harddropsound;
    QSound *rotatesound;
    QSound *explosionsound;

    void init();
    void spawnItem(int=0);
    void setBlockColour(QLabel*, int=0);
    void keyPressEvent(QKeyEvent*) override;
    void sendData();
    void explode();

private slots:
    void getScore(int);
    void getKey(QKeyEvent*);
    void netreply(QNetworkReply *r);
    void MPtimertick();


signals:
    void lineZapped(int);
    void blockPlaced(int);
    void hardDrop(int);
    void gameOver();
    void updateNextBlock(int);
    void newGame(int);
    void sendKey(QKeyEvent*);

};

#endif // TETRISGAME_H
