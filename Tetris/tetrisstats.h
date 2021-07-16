// Danie Walker
// Johnny Scholtz

#ifndef TETRISSTATS_H
#define TETRISSTATS_H

#include <QWidget>
#include <QLabel>

struct ScoreItem
{
    int count;
    QLabel *label;
};

class TetrisStats : public QWidget
{
    Q_OBJECT
public:
    explicit TetrisStats(QWidget *parent = nullptr);
    void reset();

public slots:
    void lineZapped(int);
    void blockPlaced(int);
    void hardDrop(int);
    void gameOver();
    void updateMPScore(int,bool=false);

private:
    bool newHS;
    ScoreItem scores[9];
    int score;
    int opponentScore;
    bool opponentLost;
    QLabel *scoreValueLabel;
    int sum;
    QLabel *sumLabel;
    QLabel *playingLabel;

signals:
    void getScore(int);
    void setP2Score(int,bool = false);
};

#endif // TETRISSTATS_H
