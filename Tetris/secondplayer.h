// Danie Walker 26846179
// Johnny Scholtz 31614795
// REII 313 Practical

#ifndef SECONDPLAYER_H
#define SECONDPLAYER_H


#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QString>
#include <QNetworkReply>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>

struct secBoardItem
{
    QLabel *label;
    int colour;
};

class secondplayer: public QWidget
{
    Q_OBJECT
public:
    explicit secondplayer(QWidget *parent = nullptr);
    void init(int, QString, bool);
public slots:


    void timertick();

private:
    int boardX, boardY;
    secBoardItem **board;
    QString mbID;
    QTimer *timer;
    QNetworkAccessManager *nm;

private slots:
    void netreply(QNetworkReply*);

signals:
    void updateMPScore(int,bool);
};

#endif // SECONDPLAYER_H
