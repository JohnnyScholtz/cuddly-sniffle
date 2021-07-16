// Danie Walker 26846179
// Johnny Scholtz 31614795
// REII 313 Practical

#include "tetrisgame.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QTextStream>
#include <iostream>
#include <QGraphicsItem>

TetrisGame::TetrisGame(QWidget *parent) : QWidget(parent) // Default constructor
{
    boardX = 10;
    boardY = 20;
    init();
}

TetrisGame::TetrisGame(int v, QWidget *parent) : QWidget(parent) // Constructor with specified size
{
    boardX = v;
    boardY = v*2;
    init();
}

TetrisGame::~TetrisGame() // Destructor
{
    releaseKeyboard();
}

void TetrisGame::init() // Initialize game
{
    // Initialize flags and variables
    mustspawn = true;
    gameisover = false;
    lastspawn = 0;
    isMP = false;
    hardDropped = false;

    // Create board
    board = new BoardItem*[boardX];
    for (int x = 0; x < boardX; ++x)
        board[x] = new BoardItem[boardY];

    // Create layout
    QGridLayout *layout = new QGridLayout;

    // Initialize and display board
    for (int x = 0; x < boardX; ++x)
    {
        for (int y = 0; y < boardY; ++y)
        {
            board[x][y].colour = 0;
            QLabel *l = new QLabel();
            setBlockColour(l);
            l->resize(30,30);
            layout->addWidget(l, y, x);
            board[x][y].label = l;
        }
    }

    setLayout(layout);

    // Start timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timertick()));
    timer->start(1000);
    timerCount = 1000; // Default interval for timer is 1 second

    // Load game sounds
    lineclearsound = new QSound(":/sounds/Tetris_line_clear.wav",this);
    gameoversound = new QSound(":/sounds/Tetris_gameover.wav",this);
    harddropsound = new QSound(":/sounds/Tetris_harddrop.wav",this);
    rotatesound = new QSound(":/sounds/Tetris_rotate.wav",this);
    explosionsound = new QSound(":/sounds/Tetris_explosion.wav",this);

    nextBlock = QRandomGenerator::global()->bounded(1,8); // Initialize first block to drop
}

void TetrisGame::pauseGame() // Pause game - stop timer
{
    if (timer->isActive())
    {
        timer->stop();
    }
    else
    {
        timer->start();
    }
}

void TetrisGame::setBlockColour(QLabel *l, int c) // Function to display blocks
{
    QString hw = "height=\"" + QString::number(l->height()) + "\" width=\"" + QString::number(l->width()) + "\"";
    switch (c) // Switch case to load the corresponding image for each block
    {
    case 0:
        l->setStyleSheet("background-color: black; border: 1px solid grey;");
        l->setText("");
        break;
    case 1:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/S.png\"" + hw +"></div>");
        l->setStyleSheet("background-color: green; border: none;");
        break;
    case 2:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/I.png\"" + hw +"></div>");
        l->setStyleSheet("background-color: cyan; border: none;");
        break;
    case 3:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/SQ.png\"" + hw +"></div>");
        l->setStyleSheet("background-color: yellow; border: none;");
        break;
    case 4:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/L.png\"" + hw +"></div>");
        l->setStyleSheet("background-color: orange; border: none;");
        break;
    case 5:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/Z.png\"" + hw +"></div>");
        l->setStyleSheet("background-color: red; border: none;");
        break;
    case 6:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/M_L.jpg\"" + hw +"></div>");
        l->setStyleSheet("background-color: blue; border: none;");
        break;
    case 7:
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/T.jpg\"" + hw +"></div>");
        l->setStyleSheet("background-color: magenta; border: none;");
        break;
    case 8:
        hw = "height=\"" + QString::number(l->height()-8) + "\" width=\"" + QString::number(l->width()-8) + "\"";
        l->setText("<div align=\"centre\"><img src=\":/colourBlocks/bomb2.png\"" + hw +"></div>");
        l->setStyleSheet("background-color: black; border: 1px solid grey;");
        break;
    default:
        l->setStyleSheet("background-color: white; border: none;");
    }
}

void TetrisGame::spawnItem(int i) // Spawn an item
{
    if (i == 0) // If no item is specified
    {
        i = nextBlock; // The item to spawn is loaded from the next item
        int bChance = 10; // 1 in (100/bchance) chance to spawn a bomb -> 10% chance
        int spawnBomb = QRandomGenerator::global()->bounded(1,(100/bChance+1)); // Random if the item is a bomb
        if (spawnBomb == 100/bChance)
            nextBlock = 8;
        else
            nextBlock = QRandomGenerator::global()->bounded(1,8); // If it is not a bomb, choose a random block
        updateNextBlock(nextBlock);

        if (i==8) // Flag to indicate if the current item is a block
            isBomb = true;
        else
            isBomb = false;
    }

    switch (i) // Case statement to create item
    {
    case 1:             // S-Shape
        ItemBlock a;
        a.x = 4;
        a.y = 0;
        a.colour = i;
        item.append(a);
        a.y = 1;
        item.append(a);
        a.x = 5;
        item.append(a);
        a.y = 2;
        item.append(a);
        break;
    case 2:             //Line-Shape
        ItemBlock b;
        b.x = 3;
        b.y = 0;
        b.colour = i;
        item.append(b);
        b.x = 4;
        item.append(b);
        b.x = 5;
        item.append(b);
        b.x = 6;
        item.append(b);
        break;
    case 3:             //Square-Shape
        ItemBlock c;
        c.x = 4;
        c.y = 0;
        c.colour = i;
        item.append(c);
        c.y = 1;
        item.append(c);
        c.x = 5;
        c.y = 0;
        item.append(c);
        c.y = 1;
        item.append(c);
        break;
    case 4:             //L-Shape
        ItemBlock d;
        d.x = 4;
        d.y = 0;
        d.colour = i;
        item.append(d);
        d.x = 5;
        item.append(d);
        d.y = 1;
        item.append(d);
        d.y = 2;
        item.append(d);
        break;
    case 5:             //Z-Shape
        ItemBlock e;
        e.x = 5;
        e.y = 0;
        e.colour = i;
        item.append(e);
        e.y = 1;
        item.append(e);
        e.x = 4;
        item.append(e);
        e.y = 2;
        item.append(e);
        break;
    case 6:             //Mirrored L-Shape
        ItemBlock f;
        f.x = 5;
        f.y = 0;
        f.colour = i;
        item.append(f);
        f.x = 4;
        item.append(f);
        f.y = 1;
        item.append(f);
        f.y = 2;
        item.append(f);
        break;
    case 7:             // T-Shape
        ItemBlock g;
        g.x = 3;
        g.y = 0;
        g.colour = i;
        item.append(g);
        g.x = 4;
        item.append(g);
        g.x = 5;
        item.append(g);
        g.x = 4;
        g.y = 1;
        item.append(g);
        break;
     case 8:            // Bomb
        ItemBlock h;
        h.x = 5;
        h.y = 0;
        h.colour = i;
        item.append(h);
        break;
    }

    // Check if the item can spawn
    bool canplace = true;
    for (auto i : item)
        if (board[i.x][i.y].colour != 0)
        {
            canplace = false;
            break;
        }

    if (canplace) // If the item can spawn, create it
    {
        for (auto i : item)
            setBlockColour(board[i.x][i.y].label, i.colour);
        lastspawn = i;
    }
    else // If the item cant spawn, end the game
    {
        gameoversound->play();
        gameisover = true;
    }

}

void TetrisGame::timertick() // Timer tick
{
    if (gameisover) // If the game is over
    {
        gameOver(); // Send to the tetrisstats class

        if (isMP) // If playing online multiplayer, send to the opponent that the game is over
        {
                    QByteArray data;
                    QTextStream strm(&data);
                    strm << "stop "<<score<<" ";
                    strm.flush();

                    QNetworkRequest r(QUrl("http://196.252.74.10/REII313/mailbox/mb.php"));
                    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
                    QByteArray d("msgid=");
                    d.append(mbID);
                    d.append("&msg=");
                    d.append(QUrl::toPercentEncoding(data));
                    nm->post(r, d);
        }
        return; // Do not do any other actions if the game is over
    }

    if (isMP) // If playing multiplayer, send the boardsate
        sendData();

    // Increase the speed at which items drop
    if (timerCount>330 && !hardDropped)  // If the max speed has not been reached and the item is not being hard dropped
    {
        --timerCount; // Decrease the timerinterval
        if (timerCount%10==0)
            timer->setInterval(timerCount);
    }

    // Check if the current item can drop (move down)
    bool candrop = true;
    for (auto i : item)
    {
        if ((i.y >= boardY-1) || board[i.x][i.y+1].colour)
        {
            candrop = false;
            break;
        }
    }

    // If it can drop, move it down
    if (candrop)
    {
        if (hardDropped) // Count the distance an item is harddropped
        {
            ++countHeight;
        }
        for (auto i : item)
            setBlockColour(board[i.x][i.y].label); // Clear the blocks at which the item was
        for (auto &i : item)
        {
            ++i.y;
            setBlockColour(board[i.x][i.y].label, i.colour); // Move it down
        }
    }
    else
    {
        for (auto i : item)
        {
            board[i.x][i.y].colour = i.colour;
        }

        blockPlaced(lastspawn); // Send to tetrisstats to add to score
        hardDrop(countHeight); // Increase score for a harddrop

        // Reset variables
        countHeight = 0;
        hardDropped = false;

        timer->setInterval(timerCount); // Reset the timerinterval after a harddrop


        int count = 0;

        if (!isBomb) // If a block is falling
        {
            item.clear(); // Clear the current item if it hits the bottom

            // Check if a full line exists
            for(int i = boardY-1; i >= 0; --i)
            {
                lineIsFull = true;
                for (int j = 0; j < boardX; ++j)
                {
                    if(!board[j][i].colour)
                    {
                        lineIsFull = false;
                        break;
                    }
                }

                // If a full line exits, move blocks down
                if(lineIsFull)
                {
                    ++count; // Count number of full lines
                    lineclearsound->play();
                    for (int j = 0; j < boardX; ++j)
                    {
                        setBlockColour(board[j][i].label, 0);
                        board[j][i].colour=0;
                    }

                    for (int a=i; a>=0; --a)
                    {
                        for (int j = 0; j < boardX; ++j)
                        {
                            if(board[j][a].colour!=0)
                            {
                                ItemBlock g;
                                g.x=j;
                                g.y=a;
                                g.colour=board[j][a].colour;
                                item.append(g);
                            }
                        }
                    }

                    for (auto it : item)
                    {
                        setBlockColour(board[it.x][it.y].label);
                        board[it.x][it.y].colour=0;
                    }
                    for (auto &it : item)
                    {
                        ++it.y;
                        setBlockColour(board[it.x][it.y].label, it.colour);
                        board[it.x][it.y].colour = it.colour;
                    }
                    item.clear();
                    i++;
                }
            }
            if (count != 0)
            {
                lineZapped(count); // If lines were cleared, increase the score
            }
        }
        else
        {
            explosionsound->play(); // If a bomb was dropped, play a sound and clear blocks around it
            explode();
        }

        mustspawn = true; // Let a new block spawn
    }

    if (mustspawn) // Spawn new block if needed
    {
        spawnItem();
        mustspawn = false;
    }
}

void TetrisGame::keyPressEvent(QKeyEvent *e) // Keyboard events
{
    bool canmove = true;

    if (e->key() == Qt::Key_Left) // Move left
    {
        if (gameisover) // Do not move if the game is over
                return;

        // Check if item can move
        for (auto i : item)
        {
            if (i.x == boardX-1)
                canmove = true;
            if (i.x == 0)
            {
                canmove = false;
                break;
            }
            if (board[i.x-1][i.y].colour)
            {
                canmove = false;
                break;
            }
        }

        // Move item if appropriate
        if (canmove)
        {
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                --i.x;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }

    else if (e->key() == Qt::Key_Right) // Move right
    {
        if (gameisover)
                return;

        for (auto i : item)
        {
            if(i.x == 0)
                canmove = true;
            if (i.x == boardX-1)
            {
                canmove = false;
                break;
            }
            if (board[i.x+1][i.y].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                ++i.x;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }

    else if (e->key() == Qt::Key_Down) // Soft drop
    {
        if (gameisover)
                return;

        for (auto i : item)
        {
            if (i.y == boardY-1)
            {
                canmove = false;
                break;
            }
            if (board[i.x][i.y+1].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                ++i.y;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }

    else if (e->key() == Qt::Key_Space) // Hard drop
    {
        if (gameisover)
                return;

        for (auto i : item)
        {
            if (i.y == boardY-1)
            {
                canmove = false;
                break;
            }
            if (board[i.x][i.y+1].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            hardDropped = true;
            harddropsound->play();
            timer->setInterval(10); // Reduce timer interval to drop the item quickly
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                ++i.y;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }


    else if (e->key() == Qt::Key_Up) // Rotate anti clockwise
    {
        if (gameisover)
                return;

        int xmin = item[0].x;
        int ymin = item[0].y;

        for (auto i : item) // Find top left corner
        {
            if (i.x<xmin)
                xmin=i.x;
            if (i.y<ymin)
                ymin=i.y;
        }

        bool canrotate = true;

        int N = 3;
        if (item[0].colour==2) // Line's longest side is 4
            N = 4;
        if (item[0].colour==3) // Square does not rotate
            canrotate = false;

        // Check if can rotate
        if ((xmin >= boardX-(N-1))||(ymin >= boardY-(N-2)))
            canrotate = false;
        else
        for (auto i : item)
        {
            if (board[xmin+(N-1)][i.y].colour)
            {
                canrotate = false;
                break;
            }
            if (board[i.x][ymin+(N-1)].colour)
            {
                canrotate = false;
                break;
            }
        }

        // Rotate
        if (canrotate)
        {

            rotatesound->play();
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);

            int arrCompact[N][N];

            for (int i=0;i<N;i++)
                for (int j=0; j<N;j++)
                    arrCompact[i][j]=0;

            for (auto i : item) // Create array at origin
                arrCompact[i.x-xmin][i.y-ymin]=1;

            for (int i=0;i<N/2;i++) // Rotate at origin
                for (int j=i; j<N-1-i;j++)
                {
                    int temp = arrCompact[i][j];
                    arrCompact[i][j]=arrCompact[N-1-j][i];
                    arrCompact[N-1-j][i]=arrCompact[N-1-i][N-1-j];
                    arrCompact[N-1-i][N-1-j]=arrCompact[j][N-1-i];
                    arrCompact[j][N-1-i]=temp;
                }

            // Translate to original position
            bool cont;
            for (auto &i : item)
            {
                cont = false;
                for (int a=0;a<N;a++)
                {

                    for (int j=0; j<N;j++)
                    {
                        if (arrCompact[a][j]==1)
                        {
                            cont = true;
                            i.x=a+xmin;
                            i.y=j+ymin;
                            arrCompact[a][j]=0;
                            break;
                        }
                        cont = false;
                    }
                    if (cont)
                        break;
                }
            }
            bool changey = true;

            int dy = N;
            for (auto i : item)
            {
                if (i.y-ymin<dy)
                    dy=i.y-ymin;
                if (i.y==ymin)
                {
                    changey = false;
                    break;
                }
            }

            for (auto &i : item)
            {
                if (changey)
                    i.y=i.y-dy;
                setBlockColour(board[i.x][i.y].label, i.colour);

            }
        }
    }

    else if (e->key() == Qt::Key_Z) // Rotate clockwise
    {
        if (gameisover)
                return;

        int xmin = item[0].x;
        int ymin = item[0].y;
        int xmax = item[0].x;
        int ymax = item[0].y;

        for (auto i : item) // Find top left corner
        {
            if (i.x<xmin)
                xmin=i.x;
            if (i.y<ymin)
                ymin=i.y;
            if (i.x>xmax)
                xmax=i.x;
            if (i.y>ymax)
                ymax=i.y;
        }

        bool canrotate = true;

        int N=3;
        if (item[0].colour==2)
            N = 4;

        // Check if can rotate
        if (item[0].colour==3)
            canrotate = false;
        if (xmin >= boardX-(N-1)||(ymin >= boardY-(N-2)))
            canrotate = false;
        else
        for (auto i : item)
        {
            if (board[xmin+(N-1)][i.y].colour)
             {
                 canrotate = false;
                 break;
             }
             if (board[i.x][ymin+(N-1)].colour)
             {
                 canrotate = false;
                 break;
             }
        }

        // Rotate
        if (canrotate)
        {
            rotatesound->play();
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);

            int arrCompact[N][N];



            for (int i=0;i<N;i++)
                for (int j=0; j<N;j++)
                    arrCompact[i][j]=0;
            for (auto i : item)
                arrCompact[i.x-xmin][i.y-ymin]=1;
            for (int i=0;i<N/2;i++)
                for (int j=i; j<N-1-i;j++)
                {
                    int temp = arrCompact[i][j];
                    arrCompact[i][j]=arrCompact[j][N-i-1];
                    arrCompact[j][N-i-1]=arrCompact[N-1-i][N-1-j];
                    arrCompact[N-1-i][N-1-j]=arrCompact[N-j-1][i];
                    arrCompact[N-1-j][i]=temp;
                }

            bool cont;
            for (auto &i : item)
            {
                cont = false;
                for (int a=0;a<N;a++)
                {
                    for (int j=0; j<N;j++)
                    {
                        if (arrCompact[a][j]==1)
                        {
                            cont = true;
                            i.x=a+xmin;
                            i.y=j+ymin;
                            arrCompact[a][j]=0;
                            break;
                        }
                        cont = false;
                    }
                    if (cont)
                        break;
                }
             }
             bool changex = true;
             int dx = N;

             for (auto i : item)
             {
                 if (i.x-xmin<dx)
                     dx=i.x-xmin;
                 if (i.x==xmin)
                 {
                     changex = false;
                     break;
                 }
             }

             for (auto &i : item)
             {
                 if (changex)
                    i.x=i.x-dx;
                setBlockColour(board[i.x][i.y].label, i.colour);
             }
        }
    }
    else if (e->key() == Qt::Key_B && !isMP) // Cheat to spawn a bomb
    {
        if (gameisover)
                return;

        nextBlock = 8;
        updateNextBlock(nextBlock);
    }
    else
        sendKey(e); // Signal to send keys during multiplayer

}

void TetrisGame::setMP(bool mp, bool host, QString ID) // Start online multiplayer
{
    // Initialize variables
    isMP = mp;
    isHost = host;
    mbID = ID;

    mbID.truncate(7); // Max mailbox lenght is 8

    if (isMP)
    {
        timer->stop(); // Stop timer so both players start at the same time
        nm = new QNetworkAccessManager(this);

        if (isHost) // Add "h" for host and "c" for client
        {
            mbID.append("h");           
        }
        else
            mbID.append("c");

        // Send handshake to start simultaniously
        QByteArray data;
        QTextStream strm(&data);
        strm << "start ";
        strm.flush();

        QNetworkRequest r(QUrl("http://196.252.74.10/REII313/mailbox/mb.php"));
        r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QByteArray d("msgid=");
        d.append(mbID);
        d.append("&msg=");
        d.append(QUrl::toPercentEncoding(data));
        nm->post(r, d);

        // Timer to read if opponent is ready
        MPtimer = new QTimer(this);
        connect(MPtimer, SIGNAL(timeout()), this, SLOT(MPtimertick()));
        MPtimer->start(1000);

    }


}


void TetrisGame::sendData() // Send boardstate
{
    QByteArray data;
    QTextStream strm(&data);
    strm << "board " << boardX << " " << boardY << " ";
    for (int x = 0; x < boardX; ++x) // For-loops to send colour of the lable at each coordinate
    {
        for (int y = 0; y < boardY; ++y)
        {
            strm << x << " " << y << " " << board[x][y].colour << " " << score << " ";
        }
    }

    strm.flush();

    // Send data
    QNetworkRequest r(QUrl("http://196.252.74.10/REII313/mailbox/mb.php"));
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray d("msgid=");
    d.append(mbID);
    d.append("&msg=");
    d.append(QUrl::toPercentEncoding(data));
    nm->post(r, d);
}

void TetrisGame::MPtimertick() // Timertick to read if opponent is ready
{
    QString myurl("http://196.252.74.10/REII313/mailbox/mb.php?msgid=");
    QString repID = mbID;

    repID.truncate(7);
    if (isHost)
        repID.append("c"); // Read from client box if host
    else repID.append("h"); // Read from host box if client

    myurl.append(repID);
    QNetworkRequest r(myurl);
    connect(nm, SIGNAL(finished(QNetworkReply*)), this, SLOT(netreply(QNetworkReply*)));
    nm->get(r);
}

void TetrisGame::netreply(QNetworkReply *r) // Read data
{
    QByteArray data = r->readAll();
    QTextStream strm(data);
    QByteArray heading;
    strm >> heading;
    if(heading == "start") // If opponent is ready to start
    {
        MPtimer->stop(); // Stop this cycle
        timer->start(); // Start the game timer
    }
}

void TetrisGame::explode() // Bomb explosion
{
    for (int i = item[0].x-1;i<=item[0].x+1;i++)
        for (int j = item[0].y-1;j<=item[0].y+1;j++)
            if (i>=0 && i<boardX && j >=0 && j<boardY)
            {
                setBlockColour(board[i][j].label, 0); // Clear a 3x3 radius around the landing point
                board[i][j].colour=0;
            }
    item.clear(); // Clear bomb
}

void TetrisGame::getScore(int s)
{
    score = s; // Get score from tetrisstats to send for online multiplayer
}

// Read keys during local multiplayer
void TetrisGame::getKey(QKeyEvent* e)
{
    if (gameisover) // Do nothing if the game is over
            return;
    bool canmove = true;

    if (e->key() == Qt::Key_A) // Move left
    {

        for (auto i : item)
        {
            if (i.x == boardX-1)
                canmove = true;
            if (i.x == 0)
            {
                canmove = false;
                break;
            }
            if (board[i.x-1][i.y].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                --i.x;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }

    else if (e->key() == Qt::Key_D) // Move right
    {

        for (auto i : item)
        {
            if(i.x == 0)
                canmove = true;
            if (i.x == boardX-1)
            {
                canmove = false;
                break;
            }
            if (board[i.x+1][i.y].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                ++i.x;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }

    else if (e->key() == Qt::Key_S) // Soft drop
    {

        for (auto i : item)
        {
            if (i.y == boardY-1)
            {
                canmove = false;
                break;
            }
            if (board[i.x][i.y+1].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                ++i.y;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }

    else if (e->key() == Qt::Key_Shift) // Harddrop
    {

        for (auto i : item)
        {
            if (i.y == boardY-1)
            {
                canmove = false;
                break;
            }
            if (board[i.x][i.y+1].colour)
            {
                canmove = false;
                break;
            }
        }
        if (canmove)
        {
            hardDropped = true;
            harddropsound->play();
            timer->setInterval(10);
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);
            for (auto &i : item)
            {
                ++i.y;
                setBlockColour(board[i.x][i.y].label, i.colour);
            }
        }
    }


    else if (e->key() == Qt::Key_W) // Rotate anti-clockwise
    {

        int xmin = item[0].x;
        int ymin = item[0].y;

        for (auto i : item)
        {
            if (i.x<xmin)
                xmin=i.x;
            if (i.y<ymin)
                ymin=i.y;
        }

        bool canrotate = true;

        int N = 3;
        if (item[0].colour==2)
            N = 4;
        if (item[0].colour==3)
            canrotate = false;

        if ((xmin >= boardX-(N-1))||(ymin >= boardY-(N-2)))
            canrotate = false;
        else
        for (auto i : item)
        {
            if (board[xmin+(N-1)][i.y].colour)
            {
                canrotate = false;
                break;
            }
            if (board[i.x][ymin+(N-1)].colour)
            {
                canrotate = false;
                break;
            }
        }

        if (canrotate)
        {
            rotatesound->play();
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);            

            int arrCompact[N][N];

            for (int i=0;i<N;i++)
                for (int j=0; j<N;j++)
                    arrCompact[i][j]=0;

            for (auto i : item)
                arrCompact[i.x-xmin][i.y-ymin]=1;

            for (int i=0;i<N/2;i++)
                for (int j=i; j<N-1-i;j++)
                {
                    int temp = arrCompact[i][j];
                    arrCompact[i][j]=arrCompact[N-1-j][i];
                    arrCompact[N-1-j][i]=arrCompact[N-1-i][N-1-j];
                    arrCompact[N-1-i][N-1-j]=arrCompact[j][N-1-i];
                    arrCompact[j][N-1-i]=temp;
                }

            bool cont;
            for (auto &i : item)
            {
                cont = false;
                for (int a=0;a<N;a++)
                {

                    for (int j=0; j<N;j++)
                    {
                        if (arrCompact[a][j]==1)
                        {
                            cont = true;
                            i.x=a+xmin;
                            i.y=j+ymin;
                            arrCompact[a][j]=0;
                            break;
                        }
                        cont = false;
                    }
                    if (cont)
                        break;
                }
            }
            bool changey = true;

            int dy = N;
            for (auto i : item)
            {
                if (i.y-ymin<dy)
                    dy=i.y-ymin;
                if (i.y==ymin)
                {
                    changey = false;
                    break;
                }
            }

            for (auto &i : item)
            {
                if (changey)
                    i.y=i.y-dy;
                setBlockColour(board[i.x][i.y].label, i.colour);

            }
        }
    }

    else if (e->key() == Qt::Key_Q) // Rotate clockwise
    {

        int xmin = item[0].x;
        int ymin = item[0].y;
        int xmax = item[0].x;
        int ymax = item[0].y;

        for (auto i : item)
        {
            if (i.x<xmin)
                xmin=i.x;
            if (i.y<ymin)
                ymin=i.y;
            if (i.x>xmax)
                xmax=i.x;
            if (i.y>ymax)
                ymax=i.y;
        }

        bool canrotate = true;

        int N=3;
        if (item[0].colour==2)
            N = 4;
        if (item[0].colour==3)
            canrotate = false;
        if (xmin >= boardX-(N-1)||(ymin >= boardY-(N-2)))
            canrotate = false;
        else
        for (auto i : item)
        {
            if (board[xmin+(N-1)][i.y].colour)
             {
                 canrotate = false;
                 break;
             }
             if (board[i.x][ymin+(N-1)].colour)
             {
                 canrotate = false;
                 break;
             }
        }

        if (canrotate)
        {
            rotatesound->play();
            for (auto i : item)
                setBlockColour(board[i.x][i.y].label);

            int arrCompact[N][N];

            for (int i=0;i<N;i++)
                for (int j=0; j<N;j++)
                    arrCompact[i][j]=0;
            for (auto i : item)
                arrCompact[i.x-xmin][i.y-ymin]=1;
            for (int i=0;i<N/2;i++)
                for (int j=i; j<N-1-i;j++)
                {
                    int temp = arrCompact[i][j];
                    arrCompact[i][j]=arrCompact[j][N-i-1];
                    arrCompact[j][N-i-1]=arrCompact[N-1-i][N-1-j];
                    arrCompact[N-1-i][N-1-j]=arrCompact[N-j-1][i];
                    arrCompact[N-1-j][i]=temp;
                }

            bool cont;
            for (auto &i : item)
            {
                cont = false;
                for (int a=0;a<N;a++)
                {
                    for (int j=0; j<N;j++)
                    {
                        if (arrCompact[a][j]==1)
                        {
                            cont = true;
                            i.x=a+xmin;
                            i.y=j+ymin;
                            arrCompact[a][j]=0;
                            break;
                        }
                        cont = false;
                    }
                    if (cont)
                        break;
                }
             }
             bool changex = true;
             int dx = N;

             for (auto i : item)
             {
                 if (i.x-xmin<dx)
                     dx=i.x-xmin;
                 if (i.x==xmin)
                 {
                     changex = false;
                     break;
                 }
             }

             for (auto &i : item)
             {
                 if (changex)
                    i.x=i.x-dx;
                setBlockColour(board[i.x][i.y].label, i.colour);
             }
        }
    }
    else if (e->key() == Qt::Key_F) // Cheat to spawn a bomb
    {
        nextBlock = 8;
        updateNextBlock(nextBlock);
    }
}
