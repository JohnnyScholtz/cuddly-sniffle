// Danie Walker
// Johnny Scholtz

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "tetrisgame.h"
#include "tetriscontrol.h"
#include "tetrisstats.h"
#include "tetrismenu.h"
#include "secondplayer.h"
#include <QString>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    QWidget *window;
    TetrisGame *game;
    TetrisGame *gameP2;
    TetrisControl *control;
    TetrisControl *controlP2;
    TetrisStats *stats;
    TetrisStats *statsP2;
    TetrisMenu *menu;
    secondplayer *secondp;
    QTimer *timer;
    QPushButton *back;
    QSound *backgroundsound;
    bool SP = false;
    bool MP = false;
    bool MPON = false;
    bool controls = false;

    void init();

private slots:
    void newGame(int);
    void goBack();
    void singleplayer();
    void multiplayer();
    void multiplayerovernetwork(bool, QString);
    void Controls();

signals:
    void setMP(bool, bool = false, QString = "");
    void isMultiPlayer(bool);

};
#endif // MAINWINDOW_H
