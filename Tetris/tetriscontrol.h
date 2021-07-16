// Danie Walker
// Johnny Scholtz

#ifndef TETRISCONTROL_H
#define TETRISCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class TetrisControl : public QWidget
{
    Q_OBJECT
public:
    explicit TetrisControl(QWidget *parent = nullptr);



private:
    QPushButton *newgame;
    QPushButton *pause;
    QPushButton *back;
    QSpinBox *size;
    QLabel *tLabel;
    QLabel *highScoresLabel;
    QLabel *highScoresLabel1;
    QLabel *highScoresLabel2;
    QLabel *highScoresLabel3;
    QLabel *highScoresLabel4;
    QLabel *highScoresLabel5;

    QLabel *highScores1;
    QLabel *highScores2;
    QLabel *highScores3;
    QLabel *highScores4;
    QLabel *highScores5;

    QLabel *nextBlock;
    QLabel *nextHeader;
    QLabel *nextBlockP2;
    QLabel *nextHeaderP2;

    QLabel *mp;
    QLabel *mp1;
    QLabel *mpScore;

    bool paused = true;

public slots:
    void updateNextBlock(int);
    void updateMPScore(int,bool);
    void updateNextBlockP2(int);

private slots:
    void newclick();
    void pauseclick();
    void tSize();
    void backClick();

signals:
    void newGame(int);
    void pauseGame();
    void goBack();
};

#endif // TETRISCONTROL_H
