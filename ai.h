#pragma once
#ifndef AI_H
#define AI_H
#include "board.h"
#include <QDebug>
#include <time.h>
//#define corner 0x8100000000000081
//#define side 0x3C0081818181003C
//#define adj 0x42C300000000C342
//#define INT_MAX 2147483647
typedef unsigned long long ull;

struct ttt{
    ull pos;
    int index, score, after;
    bool cuted = false;
};
bool cmp(ttt a, ttt b);
class ai_player {
public:
    int my_color, start_time, step, level = 3;
    int depth_cnt[30];
    bool end, time_out = false;
    bool no_avai = false;
    ttt s[30];
    ai_player() {
        memset(depth_cnt, 0, sizeof(depth_cnt));
        //        step = 0;
    }
    int depth = 7;
    bool pass;
    int negamax(chess_board &b, int h, int alpha, int beta) {
        depth_cnt[h]++;
        if (h > 5 and clock() - start_time > 980) {
            time_out = true;
            return 0;
        }
        if (h == depth) {
            int temp = b.compute_score(b.side, step + h, level);
            return temp;
        }
        ull avai = b.find_available(), pos;
        ull black = b.board[0], white = b.board[1];
        if (!avai) {
            if(no_avai){
                no_avai = false;
                return (b.cnt(b.side) - b.cnt(!b.side)) * 1000;
            }
            no_avai = true;
            b.skip_round();
            int value;
            //            if(end){
            value = -negamax(b, h, -beta, -alpha);
            //            }else{
            //                value = -negamax(b, h + 1, -beta, -alpha);
            //            }
            b.skip_round();
            return value;
        }
        no_avai = false;
        for (pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)) {
            int temp;
            b.play(pos);
            temp = -negamax(b, h + 1, -beta, -alpha);
            b.back(black, white);
            if(time_out){
                return 0;
            }
            if (temp > alpha) {
                alpha = temp;
                if (temp >= beta) {
                    //                return beta;
                    break;
                }
            }
        }
        return alpha;
    }
    //    ull poses[30];
    int cnt = 0;
    int convert(ull pos){
        int index = __builtin_clzll(pos);
        return index;
    }
    ull end_game(chess_board &b) {
        ull pos, available;
        int max_score = -1e9 - 1;
        start_time = clock();
        ull black = b.board[0], white = b.board[1], res;
        depth = __builtin_popcountll(~(black | white));
        available = b.find_available();
        if (!available) {
            return 0;
        }
        for (pos = available & (~available + 1), res = pos; available; available ^= pos, pos = available &
                                                                                               (-available)) {
            b.play(pos);
            //            int temp = -negamax(b, 1, -INT_MAX, INT_MAX);
            int temp = -negamax(b, 1, -1e9, -max_score + 1);
            b.back(black, white);
            int index = convert(pos);
            s[cnt].index = index, s[cnt].score = temp;
            if (temp > max_score) {
                max_score = temp, res = pos;
            }
            cnt++;
        }
        return res;
    }
    int cut_cnt = 0;
    ull iterative_deepening(chess_board &b, bool is_end) {
        time_out = false;
        ull black = b.board[0], white = b.board[1], res;
        int max_depth = __builtin_popcountll(~(black | white));
        if(max_depth <= 9){
            return end_game(b);
        }
        ull pos, available;
        start_time = clock();
        available = b.find_available();
        if (!available) {
            return 0;
        }
        int start_depth = 6;
        depth = 6;
        int max_score = -1e9 - 1, min_score = 1e9;
        //        if(is_end){
        //            start_depth = min(8, max_depth);
        //        }
        cnt = 0;
        int new_max, new_min;
        for (pos = available & (~available + 1), res = pos; available; available ^= pos, pos = available &
                                                                                               (-available)) {
            b.play(pos);
            int temp = -negamax(b, 1, -INT_MAX, INT_MAX);
            b.back(black, white);
            int index = convert(pos);
            s[cnt].index = index;
            s[cnt].score = temp;
            s[cnt].pos = pos;
            s[cnt].cuted = false;
            if (temp > max_score) {
                max_score = temp, res = pos;
            }
            cnt++;
        }
        if(level == 2){
            return res;
        }
        std::sort(s, s + cnt, cmp);
        ull temp_res;
        for(depth = 7; depth <= max_depth; depth++) {
            pos = s[0].pos;
            temp_res = pos;
            b.play(pos);
            max_score = -negamax(b, 1, -1e9, 1e9);
            b.back(black, white);
            s[0].score = max_score;
            for(int i = 1; i < cnt; i++){
                pos = s[i].pos;
                b.play(pos);
                int temp;
                //                temp = -negamax(b, 1, -INT_MAX, INT_MAX);
                temp = -negamax(b, 1, -max_score - 1, -max_score);
                if(temp > max_score){
                    //                    temp = -negamax(b, 1, -1e9, 1e9);
                    temp = -negamax(b, 1, -1e9, -max_score);
                }
                b.back(black, white);
                if(time_out){
                    break;
                }
                s[i].score = temp;
                if(temp > max_score){
                    max_score = temp, temp_res = pos;
                }
            }
            if(!time_out){
                res = temp_res;
            }else{
                break;
            }
            if(clock() - start_time >= 300){
                break;
            }
        }
        return res;
    }
};


#endif // AI_H
