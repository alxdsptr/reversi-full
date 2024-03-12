#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QCOlor>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <time.h>
#include "gameplay.h"
#include <QString>

class BoardWidget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

public:
    explicit BoardWidget(QWidget *parent = nullptr);
//    game *g;
//    MainWindow *p;
public:
    void regret(){
        g.regret();
//        repaint();
    }
    int restart_game(){
        g.restart();
        int res = 0;
        if(g.mode == 2){
            res = g.ai_play();
        }
        repaint();
        return res;
    }
    void restart_game(int color){
        g.start_color = color;
        g.restart();
    }
    void save_game(QString dir){
        g.save(dir);
    }
    void setLevel1(){
        g.ai.level = 1;
    }
    void setLevel2(){
        g.ai.level = 2;
    }
    void setLevel3(){
        g.ai.level = 3;
    }
    int setMode2(){
        restart_game(0);
        g.mode = 2;
        repaint();
        return g.ai_play();
    }
    void setMode3(){
        restart_game(0);
        g.mode = 3;
        repaint();
    }
    void enable_poss(bool x){
        show_poss = x;
        repaint();
    }


signals:
    void show_win_sig();
private:
    QPixmap gird, black, white;
    void process_click(int x, int y);
    bool show_poss = true;
public:
    game g;
//    BoardWidget(MainWindow *parent){
//        p = parent;
//        gird.load("D:/examples/black and white chess/gui version/sources/grid.png");
//        black.load("D:/examples/black and white chess/gui version/sources/black.png");
//        white.load("D:/examples/black and white chess/gui version/sources/white.png");
//    }
};

#endif // BOARDWIDGET_H
