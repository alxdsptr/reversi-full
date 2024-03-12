#include "boardwidget.h"

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget{parent}
{
//    p = (MainWindow *)parent;
    gird.load("./sources/grid.png");
    black.load("./sources/black.png");
    white.load("./sources/white.png");
//    g.start(0);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
this->setMouseTracking(true);

}
void BoardWidget::mousePressEvent(QMouseEvent* e) {
    // 这里写处理鼠标事件用的代码...
    int x = e->pos().x(), y = e->pos().y();
    int i, j;
    //    qDebug() << x << ' ' << y;

        i = x / 50, j = y / 50;
        int res = 0;
        process_click(i, j);
}
void BoardWidget::process_click(int i, int j){
            int res = 0;
        res = g.try_(j, i);
            //        repaint();
        if(res == -1){
            emit show_win_sig();
                //            Sleep(100000);
        }
}
void BoardWidget::paintEvent(QPaintEvent *){
        //QPainter painter(this);
        QPainter thin(this);
        // 这里写绘图用的代码...
        int i, j;
        QPoint pos;
        //QPen pen;
        QPen thiner;
        thiner.setWidth(3);
        thiner.setColor(QColor(50, 50, 50, 80));
        thin.setPen(thiner);
        //pen.setWidth(10);
        //painter.setPen(pen);
        /*
        for(i = 0; i <= 8; i++){
            int y = (i + 1) * 100;
            int x = (i + 2) * 100;
            painter.drawLine(QPoint(200, y), QPoint(1000, y));
            painter.drawLine(QPoint(x, 100), QPoint(x, 900));
        }*/
        int x, y;
        unsigned long long avai = g.b.find_available();
        for(i = 0; i < 8; i++){
            y = i * 50 + 2;
            for(j = 0; j < 8; j++){
                x = (j) * 50 + 2;
                unsigned long long mask = 1ll << (63 - 8 * (i) - j);
                thin.drawPixmap(x, y, 46, 46, gird);
                //            int temp = g.b.contain_disc(i, j);
                if(mask & g.b.board[0]){
                    thin.drawPixmap(x + 3, y + 3, 40, 40, black);
                }else if(mask & g.b.board[1]){
                    thin.drawPixmap(x + 3, y + 3, 40, 40, white);
                }else if(show_poss and mask & avai){
                    thin.drawEllipse(x + 10, y + 10, 28, 28);
                }
            }
        }
}
