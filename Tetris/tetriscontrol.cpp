// Danie Walker
// Johnny Scholtz

#include "tetriscontrol.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QList>

TetrisControl::TetrisControl(QWidget *parent) : QWidget(parent)
{
    // Create layout
    QGridLayout *layout = new QGridLayout;

    // New game button
    newgame = new QPushButton("New game");
    newgame->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    pause = new QPushButton("Pause");
    pause->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");

    // Spinbox to set size of new game
    size = new QSpinBox;
    size->setRange(10,20);
    size->setValue(10);
    size->setStyleSheet("font-size: 8pt; font-weight: bold}");
    tLabel = new QLabel("Size:");
    tLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");

    // Labels to show highscores
    highScoresLabel = new QLabel("High Scores");
    highScoresLabel->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScoresLabel1 = new QLabel("1 - ");
    highScoresLabel1->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScoresLabel2 = new QLabel("2 - ");
    highScoresLabel2->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScoresLabel3 = new QLabel("3 - ");
    highScoresLabel3->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScoresLabel4 = new QLabel("4 - ");
    highScoresLabel4->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScoresLabel5 = new QLabel("5 - ");
    highScoresLabel5->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");

    // Label to show the next block to drop
    nextHeader = new QLabel("Next:");
    nextHeader->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    layout->addWidget(nextHeader,0,0,1,2);
    nextBlock = new QLabel;
    nextBlock->resize(130,130);
    layout->addWidget(nextBlock,1,0,1,2);

    layout->addWidget(newgame,2,0,1,2);
    layout->addWidget(pause,3,0,1,2);
    layout->addWidget(tLabel,4,0);
    layout->addWidget(size,4,1);

    layout->addWidget(highScoresLabel,5,0,1,2);
    layout->addWidget(highScoresLabel1,6,0);
    layout->addWidget(highScoresLabel2,7,0);
    layout->addWidget(highScoresLabel3,8,0);
    layout->addWidget(highScoresLabel4,9,0);
    layout->addWidget(highScoresLabel5,10,0);

    // Read highscores
    int list[] = {};
    QFile file("../saves/HighScores.txt");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
    }

    QTextStream in(&file);
    QString line;
    int i = 0;

    while(!in.atEnd())
    {
        line = in.readLine();
        list[i] = line.toInt();
        i++;

    }
    file.close();

    // Display highscores
    highScores1 = new QLabel(QString::number(list[0]));
    highScores1->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScores2 = new QLabel(QString::number(list[1]));
    highScores2->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScores3 = new QLabel(QString::number(list[2]));
    highScores3->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScores4 = new QLabel(QString::number(list[3]));
    highScores4->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    highScores5 = new QLabel(QString::number(list[4]));
    highScores5->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");

    layout->addWidget(highScores1,6,1);
    layout->addWidget(highScores2,7,1);
    layout->addWidget(highScores3,8,1);
    layout->addWidget(highScores4,9,1);
    layout->addWidget(highScores5,10,1);

    // Label to show score of opponent during online multiplayer
    mp = new QLabel("");
    mp1 = new QLabel("");
    mpScore = new QLabel("");

    layout->addWidget(mp,11,0,1,2);
    layout->addWidget(mp1,12,0);
    layout->addWidget(mpScore,12,1);

    // Backbutton
    back = new QPushButton("Menu");
    back->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");

    layout->addWidget(back,13,0,1,2);

    layout->setRowStretch(0|1|2|3|4|5|6|7|8|9|10|11|12, 1);

    // Label to show the next block to drop for player 2 during multiplayer
    nextHeaderP2 = new QLabel("");
    nextHeaderP2->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    layout->addWidget(nextHeaderP2,14,0,1,2);
    nextBlockP2 = new QLabel;
    nextBlockP2->resize(130,130);
    layout->addWidget(nextBlockP2,15,0,1,2);

    // Set layout
    setFixedWidth(130);
    setLayout(layout);

    // Connect slots
    connect(newgame, SIGNAL(clicked()), this, SLOT(newclick()));
    connect(pause, SIGNAL(clicked()), this, SIGNAL(pauseGame()));
    connect(pause, SIGNAL(clicked()), this, SLOT(pauseclick()));
    connect(back, SIGNAL(clicked()), this, SLOT(backClick()));
    connect(size, SIGNAL(valueChanged(int)), this, SLOT(tSize()));


}

void TetrisControl::newclick() // Start new game
{
    emit newGame(size->value());
}

void TetrisControl::tSize() // Label to indicate size
{
    tLabel->setText("Size:");
}

void TetrisControl::pauseclick() // Pause game
{
    if(paused)
    {
        pause->setText("Resume"); // Set text to "Resume" if paused
        paused = false;
    }
    else
    {
        pause->setText("Pause"); // Set text to "Pause" if playing
        paused = true;
    }
}

void TetrisControl::backClick() // Go back to menu
{
    emit goBack();
}

void TetrisControl::updateNextBlock(int blockNum) // Slot to show the next block
{
    QString hw = "height=\"" + QString::number(65) + "\" width=\"" + QString::number(98) + "\"";
    switch(blockNum) // Case statement to load picture of corresponding block
    {
    case 1:
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/S.jpg\"" + hw +"></div>"); break;
    case 2:
        hw = "height=\"" + QString::number(25) + "\" width=\"" + QString::number(100) + "\"";
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Line.jpg\"" + hw +"></div>"); break;
    case 3:
        hw = "height=\"" + QString::number(65) + "\" width=\"" + QString::number(65) + "\"";
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/SQ.jpg\"" + hw +"></div>"); break;
    case 4:
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/L.jpg\"" + hw +"></div>"); break;
    case 5:
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Z.jpg\"" + hw +"></div>"); break;
    case 6:
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/M_L.jpg\"" + hw +"></div>"); break;
    case 7:
        nextBlock->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/T.jpg\"" + hw +"></div>"); break;
    case 8:
        hw = "height=\"" + QString::number(80) + "\" width=\"" + QString::number(80) + "\"";
        nextBlock->setText("<div align=\"centre\"><img src=\":/colourBlocks/bomb2.png\"" + hw +"></div>"); break;
    }
}



void TetrisControl::updateMPScore(int score, bool haslost) // Update the opponent's score during online multiplayer
{
    // Hide unnescessary buttons during multiplayer
    newgame ->hide();
    size->hide();

    // Update score labels
    mp->setText("Second Player");
    mp->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    mp1->setText("Score");
    mp1->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
    mpScore->setText(QString::number(score));
    mpScore->setStyleSheet("QLabel {color : white; font-size: 9pt; font-weight: bold}");
}

void TetrisControl::updateNextBlockP2(int blockNum) // Show the next block for player 2
{
    // Update labels to indicate player 1 and player 2
    nextHeader->setText("Next - P1:");
    nextHeaderP2->setText("Next - P2:");

    QString hw = "height=\"" + QString::number(65) + "\" width=\"" + QString::number(98) + "\"";
    switch(blockNum) // Case statement to load picture of corresponding block
    {
    case 1:
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/S.jpg\"" + hw +"></div>"); break;
    case 2:
        hw = "height=\"" + QString::number(25) + "\" width=\"" + QString::number(100) + "\"";
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Line.jpg\"" + hw +"></div>"); break;
    case 3:
        hw = "height=\"" + QString::number(65) + "\" width=\"" + QString::number(65) + "\"";
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/SQ.jpg\"" + hw +"></div>"); break;
    case 4:
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/L.jpg\"" + hw +"></div>"); break;
    case 5:
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/Z.jpg\"" + hw +"></div>"); break;
    case 6:
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/M_L.jpg\"" + hw +"></div>"); break;
    case 7:
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/fullColourBlocks/T.jpg\"" + hw +"></div>"); break;
    case 8:
        hw = "height=\"" + QString::number(80) + "\" width=\"" + QString::number(80) + "\"";
        nextBlockP2->setText("<div align=\"centre\"><img src=\":/colourBlocks/bomb2.png\"" + hw +"></div>"); break;
    }
}

