// Danie Walker
// Johnny Scholtz

#include "secondplayer.h"
#include <iostream>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

secondplayer::secondplayer(QWidget *parent) : QWidget(parent)
{

}


void secondplayer::init(int X, QString ID, bool isHost)
{
    // Initialize variables
    updateMPScore(0,false); // Opponent's score is 0 at the start of the game
    boardX = X; // Set board size to specified size
    boardY = 2*boardX;

    // Create board
    board = new secBoardItem*[boardX];
    for (int x = 0; x < boardX; ++x)
        board[x] = new secBoardItem[boardY];

    // Create layout
    QGridLayout *layout = new QGridLayout;

    // Display board
    for (int x = 0; x < boardX; ++x)
    {
        for (int y = 0; y < boardY; ++y)
        {
            board[x][y].colour = 0;
            QLabel *l = new QLabel();
            //setBlockColour(l);
            l->setStyleSheet("background-color: black; border: 1px solid grey;");
            layout->addWidget(l, y, x);
            board[x][y].label = l;
        }
    }

    setLayout(layout);

    // Create timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timertick()));
    timer->start(1000);

    // Initialize network access manager and corresponding slot
    nm = new QNetworkAccessManager(this);
    connect(nm, SIGNAL(finished(QNetworkReply*)), this, SLOT(netreply(QNetworkReply*)));

    // Get mailbox name
    mbID = ID;
    mbID.truncate(7); // Truncate at 7 because the max mailbox lenght is 8
    if (isHost)
        mbID.append("c"); // Add "c" - read from client box if host
    else
        mbID.append("h"); // Add "h" - read from host box if client
}

void secondplayer::timertick() // Read from mailbox at each timer tick
{
    QString myurl("http://196.252.74.10/REII313/mailbox/mb.php?msgid=");
    myurl.append(mbID);
    QNetworkRequest r(myurl);
    nm->get(r);
}

void secondplayer::netreply(QNetworkReply *r)
{
    // Initialize data structires
    QByteArray data = r->readAll();
    QTextStream strm(data);
    QByteArray heading;
    strm >> heading; // Read heading

    if (heading == "board") // If heading indicates the recieved data is a boardstate
    {
        int bx, by; // Read the size of the board
        strm >> bx >> by;
        for (int x = 0; x < bx; ++x) // Read the boardstate
        {
            for (int y = 0; y < by; ++y)
            {
                int dx, dy, dv, score;
                strm >> dx >> dy >> dv >> score; // Read the score
                updateMPScore(score,false); // Display the score on the tetriscontrol class
                if ((dx >= 0) && (dx < boardX) && (dy >= 0) && (dy < boardY))
                {

                    board[x][y].colour = dv;
                    QString hw = "height=\"" + QString::number(board[x][y].label->height()) + "\" width=\"" + QString::number(board[x][y].label->width()) + "\"";

                    // Switch statement to display correct colours of opponent board
                    switch (board[x][y].colour){
                        case 0:
                            board[x][y].label->setText("");
                            board[x][y].label->setStyleSheet("background-color: black; border: 1px solid grey;");
                            break;
                        case 1:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/S.png\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: green; border: none;");
                            break;
                        case 2:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/I.png\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: cyan; border: none;");
                            break;
                        case 3:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/SQ.png\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: yellow; border: none;");
                            break;
                        case 4:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/L.png\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: orange; border: none;");
                            break;
                        case 5:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/Z.png\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: red; border: none;");
                            break;
                        case 6:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/M_L.jpg\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: blue; border: none;");
                            break;
                        case 7:
                            board[x][y].label->setText("<div align=\"centre\"><img src=\":/colourBlocks/T.jpg\"" + hw +"></div>");
                            board[x][y].label->setStyleSheet("background-color: magenta; border: none;");
                            break;
                        default:
                            board[x][y].label->setStyleSheet("background-color: black; border: none;");
                            break;
                        }

                    }
            }
        }
    }
    else if (heading == "stop") // If the heading indicates the game is over
    {
        int score;
        strm >> score; // Read the score
        updateMPScore(score,true); // Display the score on the tetriscontrol class
    }
}

