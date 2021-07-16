// Danie Walker
// Johnny Scholtz

#include "mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) // Constructor
{
    // Set icon
    setWindowIcon(QIcon(":/background/tetris.jpg"));

    // Create window
    window = new QWidget(this);
    window->setStyleSheet(QStringLiteral("border-image: url(:/background/backG.jpg);"));
    window->setFixedSize(500,500);

    // Play background music
    backgroundsound = new QSound(":/sounds/Tetris_sound.wav",this);
    backgroundsound->play();
    backgroundsound->setLoops(-1);

    // Create menu
    menu = new TetrisMenu(this);

    // Connect slots
    connect(menu, SIGNAL(singleplayer()), this, SLOT(singleplayer()));
    connect(menu, SIGNAL(multiplayer()), this, SLOT(multiplayer()));
    connect(menu, SIGNAL(goBack()), this, SLOT(goBack()));
    connect(menu, SIGNAL(multiplayerovernetwork(bool, QString)),this,SLOT(multiplayerovernetwork(bool, QString)));
    connect(menu, SIGNAL(Controls()), this, SLOT(Controls()));

    // Create layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(menu);
    setFixedSize(500,500);
    setLayout(layout);

    // Set flags for gamemodes
    SP = false;
    MP = false;
    MPON = false;
    controls = false;

}

MainWindow::~MainWindow()
{
}

void MainWindow::goBack() // Back button
{
    // Delete current widgets
    if(SP)
    {
        if (game != nullptr)
        {
            game->deleteLater();
        }
        control->deleteLater();
        stats->deleteLater();
        layout()->removeWidget(control);
        layout()->removeWidget(game);
        layout()->removeWidget(stats);
    }

    else if(MP)
    {
        if (game != nullptr)
        {
             game->deleteLater();
        }

        if (gameP2 != nullptr)
        {
             gameP2->deleteLater();
        }

        control->deleteLater();
        stats->deleteLater();
        //controlP2->deleteLater();
        statsP2->deleteLater();
        layout()->removeWidget(stats);
        layout()->removeWidget(game);
        layout()->removeWidget(control);
        layout()->removeWidget(gameP2);
        layout()->removeWidget(statsP2);
    }

    else if(MPON)
    {
        if (game != nullptr)
        {
             game->deleteLater();
             secondp->deleteLater();
        }
        control->deleteLater();
        stats->deleteLater();
        layout()->removeWidget(secondp);
        layout()->removeWidget(control);
        layout()->removeWidget(game);
        layout()->removeWidget(stats);
    }

    else if(controls)
    {
        back->deleteLater();
        layout()->removeWidget(back);
        layout()->removeWidget(window);
        setStyleSheet("background-color : none");
    }
    else menu -> deleteLater();

    delete layout();

    window->setStyleSheet(QStringLiteral("border-image: url(:/background/backG.jpg);"));
    window->setFixedSize(500,500);
    window->setGeometry(0,0,500,500);

    // Create menu
    menu = new TetrisMenu(this);

    // Connect slots
    connect(menu, SIGNAL(singleplayer()), this, SLOT(singleplayer()));
    connect(menu, SIGNAL(multiplayer()), this, SLOT(multiplayer()));
    connect(menu, SIGNAL(goBack()), this, SLOT(goBack()));
    connect(menu, SIGNAL(multiplayerovernetwork(bool, QString)),this,SLOT(multiplayerovernetwork(bool, QString)));
    connect(menu, SIGNAL(Controls()), this, SLOT(Controls()));

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(menu);
    setFixedSize(500,500);
    setGeometry(700,300,500,500);

    setLayout(layout);

    // Reset flags
    SP = false;
    MP = false;
    MPON = false;
    controls = false;
}

void MainWindow::newGame(int v) // Newgame button
{
    if (SP) // Create a new game of the specified size
    {
        if (game != nullptr)
        {
            game->deleteLater();
        }
        control->deleteLater();
        stats->reset();
        game = new TetrisGame(v, this);
        game->grabKeyboard();
        control = new TetrisControl(this);
    }

    if (MP)
    {
        if (game != nullptr)
        {
             game->deleteLater();
        }

        if (gameP2 != nullptr)
        {
             gameP2->deleteLater();
        }

        control->deleteLater();
        stats->reset();
        statsP2->reset();

        game = new TetrisGame(v, this);
        game->grabKeyboard();
        control = new TetrisControl(this);
        gameP2 = new TetrisGame(v, this);
    }

    if (SP)
    {
        // Connect single player slots
        connect(game, SIGNAL(lineZapped(int)), stats, SLOT(lineZapped(int)));
        connect(game, SIGNAL(blockPlaced(int)), stats, SLOT(blockPlaced(int)));
        connect(game, SIGNAL(hardDrop(int)), stats, SLOT(hardDrop(int)));
        connect(game, SIGNAL(gameOver()), stats, SLOT(gameOver()));
        connect(control, SIGNAL(pauseGame()), game, SLOT(pauseGame()));
        connect(control, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
        connect(control, SIGNAL(goBack()), this, SLOT(goBack()));
        connect(game, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlock(int)));
    }

    if (MP)
    {
        // Connect multilpayerslots
        connect(game, SIGNAL(lineZapped(int)), stats, SLOT(lineZapped(int)));
        connect(game, SIGNAL(sendKey(QKeyEvent*)), gameP2, SLOT(getKey(QKeyEvent*)));
        connect(game, SIGNAL(blockPlaced(int)), stats, SLOT(blockPlaced(int)));
        connect(game, SIGNAL(hardDrop(int)), stats, SLOT(hardDrop(int)));
        connect(game, SIGNAL(gameOver()), stats, SLOT(gameOver()));
        connect(control, SIGNAL(pauseGame()), game, SLOT(pauseGame()));
        connect(control, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
        connect(control, SIGNAL(goBack()), this, SLOT(goBack()));
        connect(game, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlock(int)));

        connect(gameP2, SIGNAL(lineZapped(int)), statsP2, SLOT(lineZapped(int)));
        connect(gameP2, SIGNAL(blockPlaced(int)), statsP2, SLOT(blockPlaced(int)));
        connect(gameP2, SIGNAL(hardDrop(int)), statsP2, SLOT(hardDrop(int)));
        connect(gameP2, SIGNAL(gameOver()), statsP2, SLOT(gameOver()));
        connect(control, SIGNAL(pauseGame()), gameP2, SLOT(pauseGame()));
        connect(gameP2, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlockP2(int)));
    }

    // Set layouts
    if (SP)
    {
        layout()->removeWidget(control);
        layout()->addWidget(game);
        layout()->addWidget(control);
    }
    if (MP)
    {
        layout()->removeWidget(control);
        layout()->removeWidget(game);
        layout()->removeWidget(stats);
        layout()->addWidget(gameP2);
        layout()->addWidget(control);
        layout()->addWidget(game);
        layout()->addWidget(stats);

    }
}

void MainWindow::singleplayer() // Singleplayer button
{
    SP = true;
    // Remove menu
    menu->deleteLater();
    layout()->removeWidget(menu);

    // Create widgets
    game = new TetrisGame(this);
    game->grabKeyboard(); // Game uses keyboard
    control = new TetrisControl(this);
    stats = new TetrisStats(this);
    stats->reset();

    // Connect slots
    connect(control, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
    connect(control, SIGNAL(goBack()), this, SLOT(goBack()));
    connect(this, SIGNAL(setMP(bool,bool,QString)), game, SLOT(setMP(bool,bool,QString)));
    setMP(false);
    connect(game, SIGNAL(lineZapped(int)), stats, SLOT(lineZapped(int)));
    connect(game, SIGNAL(blockPlaced(int)), stats, SLOT(blockPlaced(int)));
    connect(game, SIGNAL(hardDrop(int)), stats, SLOT(hardDrop(int)));
    connect(menu, SIGNAL(singleplayer()), this, SLOT(singleplayer()));
    connect(game, SIGNAL(gameOver()), stats, SLOT(gameOver()));
    connect(control, SIGNAL(pauseGame()), game, SLOT(pauseGame()));
    connect(game, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlock(int)));

    // Create layout
    layout()->addWidget(stats);
    layout()->addWidget(game);
    layout()->addWidget(control);

    window->setStyleSheet(QStringLiteral("border-image: url(:/background/backG4.jpg);"));
    window->setFixedSize(830,990);
    setFixedSize(830,990);
    setGeometry(500,37,850,990);
}

void MainWindow::multiplayer() // Local multiplayer
{
    MP = true;

    //Remove menu
    menu->deleteLater();
    layout()->removeWidget(menu);

    // Create widgets
    game = new TetrisGame(this);
    control = new TetrisControl(this);
    stats = new TetrisStats(this);
    stats->reset();
    gameP2 = new TetrisGame(this);
    game->grabKeyboard();
    statsP2 = new TetrisStats(this);

    // Connect slots
    connect(control, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
    connect(control, SIGNAL(goBack()), this, SLOT(goBack()));
    connect(this, SIGNAL(setMP(bool,bool,QString)), game, SLOT(setMP(bool,bool,QString)));
    setMP(false);
    connect(game, SIGNAL(lineZapped(int)), stats, SLOT(lineZapped(int)));
    connect(gameP2, SIGNAL(lineZapped(int)), statsP2, SLOT(lineZapped(int)));
    connect(game, SIGNAL(sendKey(QKeyEvent*)), gameP2, SLOT(getKey(QKeyEvent*)));
    connect(game, SIGNAL(blockPlaced(int)), stats, SLOT(blockPlaced(int)));
    connect(gameP2, SIGNAL(blockPlaced(int)), statsP2, SLOT(blockPlaced(int)));
    connect(game, SIGNAL(hardDrop(int)), stats, SLOT(hardDrop(int)));
    connect(gameP2, SIGNAL(hardDrop(int)), statsP2, SLOT(hardDrop(int)));
    connect(menu, SIGNAL(multiplayer()), this, SLOT(multiplayer()));
    connect(game, SIGNAL(gameOver()), stats, SLOT(gameOver()));
    connect(gameP2, SIGNAL(gameOver()), statsP2, SLOT(gameOver()));
    connect(control, SIGNAL(pauseGame()), game, SLOT(pauseGame()));
    connect(control, SIGNAL(pauseGame()), gameP2, SLOT(pauseGame()));
    connect(stats, SIGNAL(setP2Score(int,bool)), statsP2, SLOT(updateMPScore(int,bool)));
    connect(statsP2, SIGNAL(setP2Score(int,bool)), stats, SLOT(updateMPScore(int,bool)));
    connect(game, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlockP2(int)));
    connect(gameP2, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlock(int)));

    // Create layout
    layout()->addWidget(statsP2);
    layout()->addWidget(gameP2);
    layout()->addWidget(control);
    layout()->addWidget(game);
    layout()->addWidget(stats);

    window->setStyleSheet(QStringLiteral("border-image: url(:/background/backG4.jpg);"));
    window->setFixedSize(1600,990);
    setFixedSize(1600,990);
    setGeometry(180,37,1600,990);
}

void MainWindow::multiplayerovernetwork(bool isHost, QString mbID) // Online multiplayer
{
    MPON = true;

    // Remove menu
    menu->deleteLater();
    layout()->removeWidget(menu);

    // Create widgets
    game = new TetrisGame(this);
    control = new TetrisControl(this);
    stats = new TetrisStats(this);
    stats->reset();
    game->grabKeyboard();
    secondp = new secondplayer(this);

    // Connect slots
    connect(control, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
    connect(control, SIGNAL(goBack()), this, SLOT(goBack()));
    connect(stats, SIGNAL(getScore(int)), game, SLOT(getScore(int)));
    connect(game, SIGNAL(newGame(int)), this, SLOT(newGame(int)));
    connect(this, SIGNAL(setMP(bool,bool,QString)), game, SLOT(setMP(bool,bool,QString)));
    connect(secondp,SIGNAL(updateMPScore(int,bool)),control,SLOT(updateMPScore(int,bool)));
    connect(secondp,SIGNAL(updateMPScore(int,bool)),stats,SLOT(updateMPScore(int,bool)));
    setMP(true, isHost, mbID);
    connect(game, SIGNAL(lineZapped(int)), stats, SLOT(lineZapped(int)));
    connect(game, SIGNAL(blockPlaced(int)), stats, SLOT(blockPlaced(int)));
    connect(game, SIGNAL(hardDrop(int)), stats, SLOT(hardDrop(int)));
    connect(menu, SIGNAL(singleplayer()), this, SLOT(singleplayer()));
    connect(game, SIGNAL(gameOver()), stats, SLOT(gameOver()));
    connect(control, SIGNAL(pauseGame()), game, SLOT(pauseGame()));
    connect(game, SIGNAL(updateNextBlock(int)), control, SLOT(updateNextBlock(int)));

    // Create layout
    layout()->addWidget(stats);
    layout()->addWidget(game);
    layout()->addWidget(control);
    layout()->addWidget(secondp);

    window->setStyleSheet(QStringLiteral("border-image: url(:/background/backG5.jpg);"));
    window->setFixedSize(1300,990);
    setFixedSize(1300,990);
    setGeometry(300,37,1300,990);
    secondp->init(10,mbID,isHost); // test

}

void MainWindow::Controls() // Display controls
{
    controls = true;

    // Remove menu
    menu->deleteLater();
    layout()->removeWidget(menu);

    // Create back button
    back = new QPushButton();
    back->setText("Menu");
    back->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    back->setFixedSize(130,40);

    // Show image for controls
    window->setStyleSheet(QStringLiteral("border-image: url(:/background/controls.PNG);"));
    window->setFixedSize(900,600);

    // Set layout
    layout()->addWidget(back);
    layout()->addWidget(window);

    setStyleSheet("background-color : black");
    setFixedSize(1130,640);
    setGeometry(400,250,800,990);

    connect(back, SIGNAL(clicked()), this, SLOT(goBack())); // Connect signal for back button
}
