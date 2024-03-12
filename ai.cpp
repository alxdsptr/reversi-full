#include "ai.h"

bool cmp(ttt a, ttt b){
    return a.score > b.score;
}
/*
int choose_helper(chess_board &b, int h, int alpha, int beta, bool is_max, int color){ // alpha是上一个最大层节点已经能获得的最大值，beta是上一个最小层节点已经能获得的最小值
//    qDebug ( "a");
    ull avai = b.find_available(), pos;
    ull black = b.board[0], white = b.board[1];
    int start = clock();
    if(h == 8){
        if(!avai){
            int temp = b.compute_score(color) - b.compute_score(!color);
            return temp - 100;
        }
        int score = 0;
        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            b.play(pos);
            int temp = b.compute_score(color) - b.compute_score(!color);
            if(temp > score){
                score = temp;
            }
            b.back(black, white);
        }
        return score;
    }
    if(!avai){
        b.skip_round();
        int temp = choose_helper(b, h + 1, -INT_MAX, INT_MAX, !is_max, color);
        b.skip_round();
        return temp;
    }
    if(is_max){
        int max_score = -INT_MAX;
        if(avai & CORNER)
            return 1e7;
        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            int temp;
            b.play(pos);
//        if(temp >= 1e7){
//            return temp;
//        }else if(temp <= -1e6){
//            continue;
//        }
            temp = choose_helper(b, h + 1, max_score, 0, false, color);
            b.back(black, white);
            if(temp >= beta){
                return temp;
            }
            if(temp > max_score){
                max_score = temp;
            }
        }
        return max_score;
    }else{
        int min_score = INT_MAX;
        if(avai & CORNER)
            return -1e7;
        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            int temp;
            b.play(pos);
//        if(temp >= 1e7){
//            return temp;
//        }else if(temp <= -1e6){
//            continue;
//        }
            temp = choose_helper(b, h + 1, 0, min_score, true, color);
            b.back(black, white);
            if(temp <= alpha){
                return temp;
            }
            if(temp < min_score){
                min_score = temp;
            }
        }
        return min_score;
    }
//    int end = clock();
//    b.time_cnt[h + 2] += end - start;
}

ull choose(chess_board &b, ull available){
    ull pos;
//    if(available & 0x8100000000000081){

//    }
    int max_score = -INT_MAX, color = b.side;
    ull black = b.board[0], white = b.board[1],res;
    for(pos = available & (~available + 1), res = pos; available; available ^= pos, pos = available &(-available)){
        if(pos & CORNER){
            return pos;
        }
        b.play(pos);
//        int start = clock();
        int temp = choose_helper(b, 1, max_score, 0, false, color);
//        int end = clock();
//        qDebug() << "step" << end - start;
        b.back(black, white);
        if(temp > max_score){
            max_score = temp, res = pos;
        }
    }
//    for(int i = 0; i <= 11; i++){
//        qDebug("%d: %d ", i, b.time_cnt[i]);
//    }
    return res;
}*/
/*
int choose_helper(chess_board &b, int h, int alpha, int beta, bool is_max){ // alpha是上一个最大层节点已经能获得的最大值，beta是上一个最小层节点已经能获得的最小值
    //    qDebug ( "a");
    ull avai = b.find_available(), pos;
    ull black = b.board[0], white = b.board[1];
    //    int start = clock();
    if(h == 7){
        if(!avai){
            int temp = b.compute_score(my_color) - b.compute_score(!my_color);
            return temp - 100;
        }
        //        int score = 0;
        int score = INT_MAX;

        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            b.play(pos);
            int temp = b.compute_score(my_color) - b.compute_score(!my_color);
            if(temp > score){
                score = temp;
            }
            b.back(black, white);
        }
        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            b.play(pos);
            int temp = b.compute_score(my_color) - b.compute_score(!my_color);
            if(temp < score){
                score = temp;
            }
            b.back(black, white);
        }
        return score;
    }
    if(!avai){
        b.skip_round();
        int temp = choose_helper(b, h + 1, -INT_MAX, INT_MAX, !is_max);
        b.skip_round();
        return temp;
    }
    if(is_max){
        int max_score = -INT_MAX;
        if(avai & CORNER)
            return 1e7;
        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            int temp;
            b.play(pos);
            //        if(temp >= 1e7){
            //            return temp;
            //        }else if(temp <= -1e6){
            //            continue;
            //        }
            temp = choose_helper(b, h + 1, max_score, 0, false);
            b.back(black, white);
            if(temp >= beta){
                return temp;
            }
            if(temp > max_score){
                max_score = temp;
            }
        }
        return max_score;
    }else{
        int min_score = INT_MAX;
        if(avai & CORNER)
            return -1e7;
        for(pos = avai & (-avai); avai; avai ^= pos, pos = avai & (-avai)){
            int temp;
            b.play(pos);
            //        if(temp >= 1e7){
            //            return temp;
            //        }else if(temp <= -1e6){
            //            continue;
            //        }
            temp = choose_helper(b, h + 1, 0, min_score, true);
            b.back(black, white);
            if(temp <= alpha){
                return temp;
            }
            if(temp < min_score){
                min_score = temp;
            }
        }
        return min_score;
    }
    //    int end = clock();
    //    b.time_cnt[h + 2] += end - start;
}

ull choose(chess_board &b, ull available){
    ull pos;
    int max_score = -INT_MAX;
    ull black = b.board[0], white = b.board[1],res;
    for(pos = available & (~available + 1), res = pos; available; available ^= pos, pos = available &(-available)){
        if(pos & CORNER){
            return pos;
        }
        b.play(pos);
        //        int start = clock();
        int temp = choose_helper(b, 1, max_score, 0, false);
        //        int end = clock();
        //        qDebug() << "step" << end - start;
        b.back(black, white);
        if(temp > max_score){
            max_score = temp, res = pos;
        }
    }
    //    for(int i = 0; i <= 11; i++){
    //        qDebug("%d: %d ", i, b.time_cnt[i]);
    //    }
    return res;
}
*/
