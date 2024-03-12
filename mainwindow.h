#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextEdit>
#include <QCOlor>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QPen>
#include <QPalette>
#include <QPixmap>
#include <QFileDialog>
#include <QPoint>
#include <QMessageBox>
#include "boardwidget.h"
#include <time.h>
#include <windows.h>
//#include <windows.h>
//#include "boardwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void closeEvent(QCloseEvent*) override;
public slots:
    void paint_board(){
        repaint();
    }
    void regret(){
        hide_win();
        bw->regret();
        repaint();
    }
    void restart_game(){
        int res = bw->restart_game();
        hide_win();
        if(res == -1){
            show_win();
        }
        repaint();
    }
    void save_game(){
        QString suffix = "txt";
        QString dir = QFileDialog::getSaveFileName(this,
                                                           "select path",
                                                            ".",
                                             "*.txt*", &suffix);
        bw->save_game(dir);
    }
    void save_to_clip();
    void close_clip();
    void load_from_clip();
    void close_and_load();
//    void close_clip();
    void load_game(){
        QString dir = QFileDialog::getOpenFileName(this, "select file", ".", "*.txt*");
        int res = bw->g.load(dir);
        if(res == -1){
            show_win();
        }else if(res == -2){
            QMessageBox::warning(this, "错误", "存档文件格式错误！", QMessageBox::Ok);
        }
    }
    void setLevel1(){
        bw->setLevel1();
    }
    void setLevel2(){
        bw->setLevel2();
    }
    void setLevel3(){
        bw->setLevel3();
    }
    void enable_poss(bool x){
        bw->enable_poss(x);
        repaint();
    }
    void setMode2(){
        hide_win();
        int res = bw->setMode2();
        repaint();
        if(res == -1){
            show_win();
        }
    }
    void setMode3(){
        hide_win();
        bw->setMode3();
        repaint();
    }
    void setMode1_black(){
        hide_win();
        bw->restart_game(0);
        repaint();
    }
    void setMode1_white(){
        hide_win();
        bw->restart_game(1);
        repaint();
    }
    void show_win_slot(){
        show_win();
    }
    void skip_round(){
        skip.show();
        repaint();
        Sleep(1000);
        skip.hide();
        repaint();
    }
    void start_game();
public:
//    bool show_poss = true;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    game g;
    void process_click(int i, int j);
    void proceed(){
        bw->g.proceed();
    }

private:
    Ui::MainWindow *ui;
    QPushButton regret_button, proceed_button, exit_button;
    QPushButton restart, finished, load;
    QTextEdit clipboard;
    QLabel black_cnt, white_cnt;
    QLabel black_win, white_win;
    QLabel skip, back_image;
    BoardWidget *bw;
    QPalette palette;
    QPixmap bgImage;

    void hide_win(){
        black_win.hide();
        white_win.hide();
    }
    void show_win();
    int color;
};
#endif // MAINWINDOW_H
