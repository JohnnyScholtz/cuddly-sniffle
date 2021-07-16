// Danie Walker
// Johnny Scholtz

#ifndef TETRISMENU_H
#define TETRISMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QCheckBox>
//#include <string.h>
#include <QString>

class TetrisMenu : public QWidget
{
    Q_OBJECT
public:
    explicit TetrisMenu(QWidget *parent = nullptr);
private:
    QPushButton *singlePlayer;
    QPushButton *multiPlayer;
    QPushButton *multiPlayerOverNetwork;
    QPushButton *controls;
    QPushButton *startMP;
    QPushButton *back;
    QTimer *timer;
    QLineEdit *enterID;
    QCheckBox *checkHost ;
    QVBoxLayout *layout;
    bool isHost;

private slots:
    void singlePlayerClick();
    void multiPlayerClick();
    void multiPlayerOverNetworkClick();
    void controlsClick();
    void playMPClick();
    void backClick();


signals:
    void singleplayer();
    void multiplayer();
    void multiplayerovernetwork(bool, QString);
    void Controls();
    void goBack();
};

#endif // TETRISMENU_H
