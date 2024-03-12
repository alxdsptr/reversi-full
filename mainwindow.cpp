#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::start_game(){
    bw->g.start(0);
    bw->show();
    black_cnt.show();
    white_cnt.show();
    regret_button.show();
    restart.show();
    proceed_button.show();
    ui->start_button->hide();
    ui->reversi->hide();
    ui->label_1->show();
    ui->label_2->show();
    exit_button.show();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    this->setObjectName("dialog");
//    this->setStyleSheet("#dialog{background-image: url(:/images/images/background.jpg)");
    this->setFixedSize(800, 500);
    setMouseTracking(false);
    bw = new BoardWidget(this);
//    setCentralWikkdget(bw);
    bw->move(50, 60);
    bw->setFixedSize(400, 400);
//    bw->show();
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QString styleSheet = "QMainWindow#MainWindow {"
                         "   background-image: url(:/images/images/background.jpg);"
                         "   background-repeat: no-repeat;"
                         "   background-position: center;"
                         //"   background-size: cover;"  // 让背景图片充满整个窗口
                         "}";

    this->setStyleSheet(styleSheet);
    ui->setupUi(this);
    regret_button.setText("上一步");
    regret_button.setParent(this);
    regret_button.resize(80, 40);
    proceed_button.setText("下一步");
    proceed_button.setParent(this);
    proceed_button.resize(80, 40);
    black_cnt.setParent(this);
    black_cnt.resize(100, 70);
    white_cnt.setParent(this);
    white_cnt.resize(100, 70);
    skip.setParent(this);
    skip.resize(300, 100);
    skip.move(450, 160);
    black_win.setParent(this);
    black_win.resize(200, 100);
    black_win.move(520, 160);
    white_win.setParent(this);
    white_win.resize(200, 100);
    white_win.move(520, 160);
    QFont font = QFont();
    font.setBold(true);
    font.setPointSize(30);
    black_cnt.setFont(font);
    white_cnt.setFont(font);
    palette.setColor(QPalette::WindowText,Qt::white);
    black_cnt.setPalette(palette);
    white_cnt.setPalette(palette);
    black_win.setPalette(palette);
    skip.setPalette(palette);
    white_win.setPalette(palette);
    QFont win_font = QFont();
    win_font.setBold(true);
    win_font.setPointSize(50);
    black_win.setFont(win_font);
    white_win.setFont(win_font);
    black_win.setText("黑棋胜");
    white_win.setText("白棋胜");
    skip.setFont(win_font);
    skip.setText("跳过回合！");
    skip.hide();
    black_win.hide();
    white_win.hide();
    restart.setText("重新开始");
    restart.setParent(this);
    restart.resize(100, 50);
    restart.move(480, 30);
    exit_button.setText("退出程序");
    exit_button.setParent(this);
    exit_button.resize(100, 50);
    exit_button.move(630, 30);
    exit_button.hide();
    proceed_button.move(620, 330);
    regret_button.move(520, 330);
    black_cnt.move(550, 400);
    white_cnt.move(650, 400);
//    load.setFixedSize(200, 30);
    this->setWindowTitle("Reversi");
//    connect(&load, &QPushButton::clicked, this, &MainWindow::close);
//    connect(&load, &QPushButton::clicked, this, &MainWindow::update);
//    connect(&b, &chess_board::draw_board, this, &QWidget::repaint);
//    connect(&g.b, &chess_board::draw, this, &MainWindow::paint_board);
    connect(&regret_button, &QPushButton::clicked, this, &MainWindow::regret);
    connect(&proceed_button, &QPushButton::clicked, this, &MainWindow::proceed);
    connect(&restart, &QPushButton::clicked, this, &MainWindow::restart_game);
    connect(&bw->g, &game::draw_board, this, &MainWindow::paint_board);
    connect(bw, &BoardWidget::show_win_sig, this, &MainWindow::show_win_slot);
    connect(&bw->g, &game::skip_signal, this, &MainWindow::skip_round);
    connect(&finished, &QPushButton::clicked, this, &MainWindow::close_clip);
    connect(&load, &QPushButton::clicked, this, &MainWindow::close_and_load);
    connect(&exit_button, &QPushButton::clicked, this, &MainWindow::close);
    black_cnt.hide();
    white_cnt.hide();
    regret_button.hide();
    proceed_button.hide();
    restart.hide();
    ui->label_1->hide();
    ui->label_2->hide();
    clipboard.setParent(this);
    back_image.setParent(this);
    back_image.resize(800, 480);
    clipboard.resize(440, 300);
    clipboard.move(170, 80);
    clipboard.hide();
    bgImage.load("./sources/small_bg.jpg");
    back_image.setPixmap(bgImage);
    back_image.hide();
    back_image.raise();
    clipboard.raise();
    finished.setParent(this);
    finished.move(350, 410);
    finished.resize(100, 40);
    finished.setText("完成");
    finished.hide();
    load.setParent(this);
    load.move(350, 410);
    load.resize(100, 40);
    load.setText("完成");
    load.hide();

//    BoardWidget bb;
//    bb.show();
//    this->hide();
//    for(int i = 0, color = 1; ; i++, color = 3 - color){

}
void MainWindow::save_to_clip(){
    back_image.show();
    clipboard.show();
    QString content = bw->g.getHistory();
    clipboard.setText(content);
    finished.show();
    repaint();
}
void MainWindow::load_from_clip(){
    back_image.show();
    clipboard.show();
    clipboard.setText("");
    load.show();
    repaint();
}
void MainWindow::close_clip(){
    finished.hide();
    back_image.hide();
    clipboard.hide();
}
void MainWindow::close_and_load(){
    load.hide();
    back_image.hide();
    clipboard.hide();
    QString res = clipboard.toPlainText();
    bw->g.load_from_string(res);
}
MainWindow::~MainWindow()
{
    delete ui;
}

int mouseX = 0;
int mouseY = 0;

void MainWindow::paintEvent(QPaintEvent*) {
    QString styleSheet = "QMainWindow#MainWindow {"
                         "   background-image: url(:/images/images/background.jpg);"
                         "   background-repeat: no-repeat;"
                         "   background-position: center;"
//                         "   background-size: cover;"  // 让背景图片充满整个窗口
                         "}";

    this->setStyleSheet(styleSheet);
    QPainter painter(this);
//    QPainter thin(this);
    // 这里写绘图用的代码...
//    int i, j;
//    QPoint pos;
//    QPen pen;
//    QPen thiner;
//    thiner.setWidth(5);
//    thiner.setColor(QColor(50, 50, 50, 80));
//    thin.setPen(thiner);
//    painter.setPen(pen);
    black_cnt.setText(QString::number(bw->g.b.cnt(0)));
    white_cnt.setText(QString::number(bw->g.b.cnt(1)));
    /*
    pen.setWidth(10);
    for(i = 0; i <= 8; i++){
        int y = (i + 1) * 100;
        int x = (i + 2) * 100;
        painter.drawLine(QPoint(200, y), QPoint(1000, y));
        painter.drawLine(QPoint(x, 100), QPoint(x, 900));
    }
    int x, y;
    unsigned long long avai = g.b.find_available();
    for(i = 1; i <= 8; i++){
        y = i * 100 + 5;
        for(j = 1; j <= 8; j++){
            x = (j + 1) * 100 + 5;
            unsigned long long mask = 1ll << (64 - 8 * (i - 1) - j);
            painter.drawPixmap(x, y, 90, 90, gird);
//            int temp = g.b.contain_disc(i, j);
            if(mask & g.b.board[0]){
                painter.drawPixmap(x + 10, y + 10, 70, 70, black);
            }else if(mask & g.b.board[1]){
                painter.drawPixmap(x + 10, y + 10, 70, 70, white);
            }else if(show_poss and mask & avai){
                thin.drawEllipse(x + 20, y + 20, 50, 50);
            }
        }
    }*/
}
void MainWindow::show_win(){
            int winner = bw->g.b.check_winner();
            if(winner == 0){
                black_win.show();
            }else{
                white_win.show();
            }
            repaint();
}


void MainWindow::mousePressEvent(QMouseEvent* event) {
    // 这里写处理鼠标事件用的代码...
            /*
    int x = e->pos().x(), y = e->pos().y();
    int i, j;
//    qDebug() << x << ' ' << y;
    if(x >= 200 and x <= 1000 and y >= 100 and y <= 900){
        i = (x - 200) / 100, j = (y - 100) / 100;
        int res = 0;

    }*/
//            e->ignore();
            QMouseEvent forwardedEvent(event->type(),
                                       bw->mapFromGlobal(event->globalPosition()),
                                       event->button(),
                                       event->buttons(),
                                       event->modifiers());
            QApplication::sendEvent(bw, &forwardedEvent);
}
void MainWindow::closeEvent( QCloseEvent * event )//关闭窗口会先处理该事件函数
{
    QMessageBox msgBox;
    msgBox.setText("是否希望存档？");
//    msgBox.setInformativeText("Do you want to save your changes?");
//    int ret = QMessageBox::question(this, "", "是否要存档？", "保存", "不保存", "取消", 0, 2);
    QString dir, suffix = ".txt";
//    switch (ret) {
//    case 0:
//        QMessageBox::information(this, "", "是");
//        dir = QFileDialog::getSaveFileName(this, "select path", ".", "*.txt*", &suffix);
//        bw->save_game(dir);
//        break;
//    case 1:
//        QMessageBox::information(this, "", "否");
//        break;
//    case 2:
//        QMessageBox::information(this, "", "取消");
//        event->ignore();
//        break;
//    default:
//        break;
//    }
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setButtonText(QMessageBox::Save, "保存");
    msgBox.setButtonText(QMessageBox::Discard, "不保存");
    msgBox.setButtonText(QMessageBox::Cancel, "取消");
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
        dir = QFileDialog::getSaveFileName(this, "select path", ".", "*.txt*", &suffix);
        bw->save_game(dir);
        break;
    case QMessageBox::Discard:
        break;
    case QMessageBox::Cancel:
        event->ignore();
        break;
    default:
        break;
    }

}
