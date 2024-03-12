#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iostream>
#include <fstream>
#include <QObject>
#include <algorithm>
#include <QFIle>
#include <QString>
#include "board.h"
#include "ai.h"

class chess_board;
typedef unsigned long long ull;
using std::endl;

class game: public QObject{
    Q_OBJECT

signals:
    void draw_board();
    void skip_signal();
public slots:
public:
    ull history[120][2];
    int history_step[120];
    ull available;
    int step, start_color = 0, mode = 1, max_step = 0;
    chess_board b;
    ai_player ai;
    int convert(ull pos){
        int index = __builtin_clzll(pos);
        return index;
    }
    void save_history(int x){
        history[step][0] = b.board[0];
        history[step][1] = b.board[1];
        history_step[step] = x;
        if(step > max_step){
            max_step = step;
        }
        step++;
    }
    game(){
        step = 0;
        save_history(0);
    }
    void start(int s){
        start_color = s; //, my_color = !s;
        if(s == 1){
            available = b.find_available();
            ull pos = ai.iterative_deepening(b, false);// ai play
            b.play(pos);
            save_history(convert(pos));
            emit draw_board();
        }
        available = b.find_available();
    }
    void regret(){
        if(step == 1){
            return;
        }
        step--;
        b.board[0] = history[step - 1][0];
        b.board[1] = history[step - 1][1];
        b.skip_round();
        available = b.find_available();
    }
    void proceed(){
        if(step > max_step){
            return;
        }
        b.board[0] = history[step][0];
        b.board[1] = history[step][1];
        step++;
        b.skip_round();
        available = b.find_available();
    }
    void save(QString dir){
        std::ofstream fout(dir.toStdString());
        fout << step << ' ' << max_step << ' ' << b.side << ' ' << start_color << endl;
        fout << mode << ' ' << ai.level << endl;
        for(int i = 0; i <= max_step; i++){
            fout << history[i][0] << ' ' << history[i][1] << endl;
        }
        for(int i = 1; i < step; i++){
            if(history_step[step] == -1){
                fout << "--";
            }else{
                fout << (char)(history_step[i] % 8 + 'a') << history_step[i] / 8 + 1;
            }
        }
    }
    QString getHistory(){
        std::string x;
        for(int i = 1; i < step; i++){
            if(history_step[i] == -1){
                x += '-';
                x += '-';
            }else{
                x += (char)(history_step[i] % 8 + 'a');
                x += history_step[i] / 8 + '1';
            }
        }
        QString res = QString::fromStdString(x);
        return res;
    }
    int load(QString dir){
        int res = 0;
        std::ifstream fin(dir.toStdString());
        fin >> step >> max_step >> b.side >> start_color;
        fin >> mode >> ai.level;
        for(int i = 0; i <= max_step; i++){
            fin >> history[i][0] >> history[i][1];
        }
        if(fin.fail()){
            restart();
            return -2;
        }
        b.board[0] = history[step - 1][0], b.board[1] = history[step - 1][1];
        label: available = b.find_available();
        if(!available){
            emit skip_signal();
            b.skip_round();
            save_history(-1);
        }
        if(mode == 1 and b.side != start_color){
            ull pos = ai.iterative_deepening(b, false);
            b.play(pos);
            goto label;
        }
        emit draw_board();
        if(mode == 2){
            res = ai_play();
        }
        return res;
    }
    void load_from_string(QString his){
        int len = his.size(), i;
        std::string s = his.toStdString();
        step = 0, max_step = 0;
        b.clear();
//        start(0);
        save_history(0);
//        step = len / 2 + 1;
//        max_step = step;
//        ull pos;
        for(i = 0; i < len / 2; i++){
            if(s[i * 2] == '-'){
                b.skip_round();
                save_history(-1);
            }else{
                int index = (s[i * 2 + 1] - '1') * 8 + (s[i * 2] - 'a');
                b.play(1ll << (63 - index));
                save_history(index);
            }
        }
        label: available = b.find_available();
        if(!available){
            emit skip_signal();
            b.skip_round();
            save_history(-1);
        }
        if(mode == 1 and b.side != start_color){
            ull pos = ai.iterative_deepening(b, false);
            b.play(pos);
            goto label;
        }
        emit draw_board();
    }
    void restart(){
        b.clear();
        step = 0, max_step = 0;
        save_history(0);
        start(start_color);
    }
    int ai_play(){
        while(true){
            ull pos = ai.iterative_deepening(b, false);
            b.play(pos);
            save_history(convert(pos));
            emit draw_board();
            available = b.find_available();
            if(!available){
                b.skip_round();
                save_history(-1);
                available = b.find_available();
                if(!available){
                    return -1;
                }
            }
        }
    }
    int try_play_human(int x, int y){
        ull mask = 1ull << (63 - x * 8 - y);
        if(available & mask){
            b.play(mask);
            save_history(x * 8 + y);
            emit draw_board();
            available = b.find_available();
            if(!available){
                b.skip_round();
                save_history(-1);
                available = b.find_available();
                if(!available){
                    return -1;
                }else{
                    emit skip_signal();
                    return 1;
                }
            }else{
                return 1;
            }
        }else{
            return 0;
        }
    }
    int try_play(int x, int y){
        if(b.side != start_color){
            return 0;
        }
        ull mask = 1ull << (63 - x * 8 - y);
        if(available & mask){
            b.play(mask);
            save_history(x * 8 + y);
            emit draw_board();
            available = b.find_available();
            if(!available){
                b.skip_round(); //跳过ai 回合
                save_history(-1);
//                emit skip_signal();
                available = b.find_available();
                if(!available){
                    // 谁都不能下了
                    return -1;
                }else{
                    emit skip_signal();
                    return 1;
                }
            }else{
//                ai: ai.step = step;
                ai: ull pos = ai.iterative_deepening(b, false);// ai play
                b.play(pos);
                save_history(convert(pos));
                emit draw_board();
                available = b.find_available();
                if(!available){// 人下不了
                    b.skip_round();
                    save_history(-1);
                    available = b.find_available();
                    if(!available){
                        return -1;
                    }else{
                        emit skip_signal();
                        goto ai;
                    }
                }else{
                    return 1;
                }
            }
        }else{
            return 0;
        }
    }
    int try_(int x, int y){
        int res = 0;
        if(mode == 1){
            res = try_play(x, y);
        }else if(mode == 3){
            res = try_play_human(x, y);
        }
        return res;
    }
};

#endif // GAMEPLAY_H
