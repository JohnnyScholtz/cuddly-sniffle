// Danie Walker 26846179
// Johnny Scholtz 31614795
// REII 313 Practical

#include "tetrisstats.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QMessageBox>
#include <QVector>
#include <QList>
using namespace std;

TetrisStats::TetrisStats(QWidget *parent) : QWidget(parent)
{
    // Initialize variables and flags
    opponentLost = false;
    opponentScore = 0;
    score = 0;
    sum = 0;
    newHS = true;

    // Create layout
    QGridLayout *layout = new QGridLayout;

    // Tetris logo
    QLabel *label = new QLabel();
    QString hw = "height=\"" + QString::number(60) + "\" width=\"" + QString::number(130) + "\"";
    label->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Tetris.png\"" + hw +"></div>");
    layout->addWidget(label, 0, 0, 1, 2);

    // Load block pictures for scoreboard
    for (int i = 1; i < 9; ++i)
    {
        scores[i].count = 0;
        QLabel *l = nullptr;
        QLabel *s = new QLabel("0");
        s->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");

        QString hw = "height=\"" + QString::number(40) + "\" width=\"" + QString::number(60) + "\"";


        switch(i)
        {
        case 1:
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/S.jpg\"" + hw +"></div>");
            break;
        case 2:
            hw = "height=\"" + QString::number(25) + "\" width=\"" + QString::number(80) + "\"";
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Line.jpg\"" + hw +"></div>");
            break;
        case 3:
            hw = "height=\"" + QString::number(40) + "\" width=\"" + QString::number(40) + "\"";
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/SQ.jpg\"" + hw +"></div>");
            break;
        case 4:
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/L.jpg\"" + hw +"></div>");
            break;
        case 5:
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Z.jpg\"" + hw +"></div>");
            break;
        case 6:
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/M_L.jpg\"" + hw +"></div>");
            break;
        case 7:
            l = new QLabel();
            l->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/T.jpg\"" + hw +"></div>");
            break;
        case 8:
            l = new QLabel();
            l->setText("Full lines");
            l->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
            break;
        }
        scores[i].label = s;
        layout->addWidget(l, i, 0);
        layout->addWidget(s, i, 1);
    }

    // Total blocks placed
    QLabel *totalBlocksLabel = new QLabel;
    totalBlocksLabel->setText("Total Blocks");
    totalBlocksLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    layout->addWidget(totalBlocksLabel, 9, 0);
    sumLabel = new QLabel;
    sumLabel->setText("0");
    sumLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");

    layout->addWidget(sumLabel, 9, 1);

    // Score label
    QLabel *scoreLabel = new QLabel;
    scoreLabel->setText("Score");
    scoreLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    layout->addWidget(scoreLabel, 10, 0);

    scoreValueLabel = new QLabel;
    scoreValueLabel->setText("0");
    scoreValueLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    layout->addWidget(scoreValueLabel, 10, 1);

    // Game state
    playingLabel = new QLabel;
    playingLabel->setText("Playing TETRIS");
    playingLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    layout->addWidget(playingLabel, 11, 0, 1, 2);

    // Create layout
    layout->setColumnMinimumWidth(0, 130);
    layout->setColumnMinimumWidth(1, 20);
    setFixedWidth(150);
    setLayout(layout);
}

// Increase score when a line is cleared
void TetrisStats::lineZapped(int n)
{
    for(int i = n; i > 0; --i) // For loop to count number of lines cleared simultaniously
    {
        ++scores[8].count;
        score = score + n*10; // More lines give more score
    }

    // Update labels
    scores[8].label->setText(QString::number(scores[8].count));
    scoreValueLabel->setText(QString::number(score));
}

// Add score for each block placed
void TetrisStats::blockPlaced(int b)
{
    switch(b)
    {
    case 1:
    case 5:
    case 7:
        score += 4; goto end; // Different blocks are worth a different amount of score
    case 2:
    case 3:
        score += 2; goto end;
    case 4:
    case 6:
        score += 3; goto end;
    }
    return;

    end:

    // Update labels
    ++scores[b].count;
    scores[b].label->setText(QString::number(scores[b].count));
    ++sum;
    sumLabel->setText(QString::number(sum));
    scoreValueLabel->setText(QString::number(score));
    getScore(score);
}

// Increase score if harddrop is used
void TetrisStats::hardDrop(int n)
{
    if (n % 2 != 0) // Make n an even number to accomodate the for loop
    {
        ++n;
    }
    for(int i = n/2; i > 0; --i) // For each 2 blocks harddropped, increaase the score
    {
        ++score;
    }
    scoreValueLabel->setText(QString::number(score)); // Update the label
}

// Get the opponent's score during multiplayer
void TetrisStats::updateMPScore(int os ,bool ol)
{
    opponentScore = os;
    opponentLost = ol; // Flag indicating that the opponent's game is over
}



void TetrisStats::gameOver()
{
    setP2Score(score,true); // Send score to opponent during multiplayer

    // Update the labels to indicate the winner during multiplayer
    if (opponentLost)
        if (score>opponentScore)
            playingLabel->setText("You won!");
        else
            playingLabel->setText("Your opponent won!");
    else
        playingLabel->setText("Game Over :("); // Gameover if singleplayer

    // Read highscores from file
    int list[] = {};
    QFile readfile("../saves/HighScores.txt");
    if (!readfile.open(QFile::ReadOnly | QFile::Text))
    {
    }

    QTextStream in(&readfile);
    QString line;
    int i = 0;

    while(!in.atEnd())
    {
        line = in.readLine();
        list[i] = line.toInt();
        ++i;
    }

    readfile.close();

    // If the current score is larger than the smallest highscore
    // Add it to the highscores
    if (newHS)
    {
        if (score > list[4])
        {
            list[4] = score;

            // Sort the highscores using bubblesort
            for(int i=0; i<5; i++)
            {
                for(int j=i+1; j<5; j++)
                {
                    if(list[i] < list[j])
                    {
                        int temp = list[j];
                        list[j] = list[i];
                        list[i] = temp;
                    }
                }
            }

            // Write the new list of highscores to the textfile
            QFile writefile("../saves/HighScores.txt");
            if (!writefile.open(QFile::WriteOnly | QFile::Text))
            {
            }

            QTextStream out(&writefile);
            i = 0;
            while (i < 5)
            {
                 QString s = QString::number(list[i]);

                 out << s << endl;
                 ++i;
            }
            writefile.close();
        }
    }

    newHS = false; // Prevents the same highscore from logging multiple times

}

// Reset the stats when a new game starts
void TetrisStats::reset()
{
    playingLabel->setText("Playing TETRIS");
    score = 0;
    scoreValueLabel->setText("0");
    sum = 0;
    sumLabel->setText("0");
    for (int i = 1; i < 9; ++i)
    {
        scores[i].count = 0;
        scores[i].label->setText("0");
    }
}
