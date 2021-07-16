// Danie Walker 26846179
// Johnny Scholtz 31614795
// REII 313 Practical

#include "tetrismenu.h"


TetrisMenu::TetrisMenu(QWidget *parent) : QWidget(parent)
{
    // Create layout
    layout = new QVBoxLayout;

    // Add menu buttons
    singlePlayer = new QPushButton("Singleplayer");
    singlePlayer->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    multiPlayer = new QPushButton("Local Multiplayer");
    multiPlayer->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    multiPlayerOverNetwork = new QPushButton("Online Multiplayer");
    multiPlayerOverNetwork->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    controls = new QPushButton("Controls");
    controls->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");

    // Set layout
    layout->addWidget(singlePlayer);
    layout->addWidget(multiPlayer);
    layout->addWidget(multiPlayerOverNetwork);
    layout->addWidget(controls);
    layout->addStretch(10);

    setFixedWidth(250);
    setLayout(layout);

    // Connect buttons to corresponding slots
    connect(singlePlayer, SIGNAL(clicked()), this, SLOT(singlePlayerClick()));
    connect(multiPlayer, SIGNAL(clicked()), this, SLOT(multiPlayerClick()));
    connect(multiPlayerOverNetwork, SIGNAL(clicked()), this, SLOT(multiPlayerOverNetworkClick()));
    connect(controls, SIGNAL(clicked()), this, SLOT(controlsClick()));
}

void TetrisMenu::singlePlayerClick() // Button to start singleplayer
{
    emit singleplayer();
}

void TetrisMenu::multiPlayerClick() // Button to start multiplayer
{
    emit multiplayer();
}

void TetrisMenu::multiPlayerOverNetworkClick() // Button to show online multiplayer menu
{

    // Hide previous menu's buttons
    controls->hide();
    multiPlayer->hide();
    multiPlayerOverNetwork->hide();
    singlePlayer->hide();
    delete layout;

    // Create new layout
    layout = new QVBoxLayout;

    // Checkbox to indicate host
    checkHost = new QCheckBox(this);
    checkHost->setText("Host:");
    checkHost->setCheckState(Qt::Unchecked);
    isHost = false;
    checkHost->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");

    // Create button to start game
    startMP = new QPushButton("Play");
    startMP->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    connect(startMP, SIGNAL(clicked()), this, SLOT(playMPClick()));

    // Textbox to enter the mailbox ID to use during online multiplayer
    enterID = new QLineEdit;
    enterID->setText("");
    enterID->setPlaceholderText("Enter Mailbox ID");
    enterID->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");

    // Create button to go back
    back = new QPushButton("Menu");
    back->setStyleSheet("background-color : white; font-size: 9pt; font-weight: bold");
    connect(back, SIGNAL(clicked()), this, SLOT(backClick()));

    // Set layout
    layout->addWidget(startMP);
    layout->addWidget(checkHost);
    layout->addWidget(enterID);
    layout->addWidget(back);
    layout->addStretch(10);
    setFixedWidth(220);
    setLayout(layout);
}

void TetrisMenu::controlsClick() // Button to show controls
{
    emit Controls();
}

void TetrisMenu::playMPClick() // Button to start online multiplayer
{
    if (checkHost->isChecked()) // Check if this instance of the game is the host
            isHost = true;
        else
            isHost = false;

    emit multiplayerovernetwork(isHost, enterID->text());
}

void TetrisMenu::backClick() // Button to go back to the menu from the online multiplayer menu
{
    emit goBack();
}
