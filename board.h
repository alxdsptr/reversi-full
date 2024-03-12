#ifndef BOARD_H
#define BOARD_H
#include <cstring>
#include <fstream>
#include <iostream>
#include <QObject>
#include <bit>
#include <vector>
#define CORNER 0x8100000000000081
#define SIDE_ 0x3C0081818181003C
#define SEC 0x3C424242423C00
#define LEFT_MASK 0xFEFEFEFEFEFEFEFE
#define RIGHT_MASK 0x7F7F7F7F7F7F7F7F
#define MID_MASK 0x7E7E7E7E7E7E7E7E
#define EDGE_MASK 0xFF818181818181FF
//#define adj 0x42C300000000C342
        /*
         * 以右边为例，第一行会把黑子右边紧挨着的白子变成1
         * 第二行会把右边紧挨着的白子右边紧挨着的白子变成1
         * 第三行将adjacent中左边一个是白子的白子变成1 以便后续操作
         * 第四行将flip右移2 然后&adj 此时为1的位置左边1个一定是白子，左边两个也一定是1，说明能够一直向左连续直到黑子 此时能覆盖黑子右边4个
         * 第五行重复第四行的操作，并且能覆盖6个 而连续6个子就是能被翻转的上限了
         */
#define FIND_HELPER(orien, d, opp) \
            flip = (me orien d) & opp; \
            flip |= (flip orien d) & opp; \
            adjacent = opp & (opp orien d); \
            flip |= (flip orien (d+d)) & adjacent; \
            flip |= (flip orien (d+d)) & adjacent; \
            moves |= flip orien d
#define PLAY_HELPER(orien, d, opp) \
            temp = (pos orien d) & opp; \
            temp |= (temp orien d) & opp; \
            adjacent = opp & (opp orien d); \
            temp |= (temp orien (d+d)) & adjacent; \
            temp |= (temp orien (d+d)) & adjacent; \
            if(temp orien d & me) flip |= temp


typedef unsigned long long ull;

class chess_board : public QObject
{
    Q_OBJECT
signals:
    void draw();

public:
    ull board[2]; //0是黑 1是bai
    int side = 0;
    int time_cnt[12];
    void clear_time(){
        memset(time_cnt, 0, sizeof(time_cnt));
    }
    chess_board(){
        board[0] = 0x810000000;
        board[1] = 0x1008000000;
    }
    void clear(){
        board[0] = 0x810000000;
        board[1] = 0x1008000000;
        side = 0;
    }
//    chess_board(ifstream &fin){
//    }
    ull find_available(){
        ull me = board[side], opp = board[!side];
        ull flip, adjacent, inner, moves = 0; // adjacent存储了相邻位置有棋子的位置
        inner = opp & 0x7E7E7E7E7E7E7E7E; // inner存储了中间6列的棋子 因为在计算左右的flip的时候，显然最边上的不可能被翻转，所以这些位置一定是false 如果这些位置是true的话，因为最后计算可能的move是通过将flip左移或者右移，那么就会跑到上一行去，这是不可接受的

        FIND_HELPER(>>, 1, inner); FIND_HELPER(<<, 1, inner); //右， 左
        FIND_HELPER(>>, 8, opp); FIND_HELPER(<<, 8, opp); //下， 上
        FIND_HELPER(>>, 7, inner); FIND_HELPER(<<, 7, inner);// 左下 右上
        FIND_HELPER(>>, 9, inner); FIND_HELPER(<<, 9, inner);// 右下 左上

        return moves & ~(me | opp); //已经有子的地方不能落子
    }
    ull find_available(int color){
        ull me = board[color], opp = board[!color];
        ull flip, adjacent, inner, moves = 0; // adjacent存储了相邻位置有棋子的位置
        inner = opp & MID_MASK; // inner存储了中间6列的棋子 因为在计算左右的flip的时候，显然最边上的不可能被翻转，所以这些位置一定是false 如果这些位置是true的话，因为最后计算可能的move是通过将flip左移或者右移，那么就会跑到上一行去，这是不可接受的

        FIND_HELPER(>>, 1, inner); FIND_HELPER(<<, 1, inner); //右， 左
        FIND_HELPER(>>, 8, opp); FIND_HELPER(<<, 8, opp); //下， 上
        FIND_HELPER(>>, 7, inner); FIND_HELPER(<<, 7, inner);// 左下 右上
        FIND_HELPER(>>, 9, inner); FIND_HELPER(<<, 9, inner);// 右下 左上

        return moves & ~(me | opp); //已经有子的地方不能落子
    }
    void play(ull pos){
        ull me = board[side], opp = board[!side];
        ull flip = 0, adjacent, inner, temp;
        inner = opp & MID_MASK;
        PLAY_HELPER(>>, 1, inner); PLAY_HELPER(<<, 1, inner); //右， 左
        PLAY_HELPER(>>, 8, opp); PLAY_HELPER(<<, 8, opp); //下， 上
        PLAY_HELPER(>>, 7, inner); PLAY_HELPER(<<, 7, inner);// 左下 右上
        PLAY_HELPER(>>, 9, inner); PLAY_HELPER(<<, 9, inner);// 右下 左上
        board[side] ^= pos;
        board[side] ^= flip, board[!side] ^= flip;
        side = !side;
    }
    int potential_mobility(int color){
        ull me = board[color], opo = board[!color], left, right;
        ull empty = ~(me | opo), res;
        left = opo & LEFT_MASK;
        right = opo & RIGHT_MASK;
        res = (left >> 1) & empty;
        res |= (right << 1) & empty;
        res |= (opo >> 8) & empty;
        res |= (opo << 8) & empty;
        res |= (right >> 7) & empty;
        res |= (right << 9) & empty;
        res |= (left << 7) & empty;
        res |= (left >> 9) & empty;
        int a = __builtin_popcountll(res);
        left = me & LEFT_MASK;
        right = me & RIGHT_MASK;
        res = (left >> 1) & empty;
        res |= (right << 1) & empty;
        res |= (me >> 8) & empty;
        res |= (me << 8) & empty;
        res |= (right >> 7) & empty;
        res |= (right << 9) & empty;
        res |= (left << 7) & empty;
        res |= (left >> 9) & empty;
        int b = __builtin_popcountll(res);
        return a - b;
        //        if(a + b  == 0) return 0;
        //        return 100 * (a - b) / (a + b);
        //        return 100 * (a - b);
    }
    int semi_stable_cnt(int color){
        ull me = board[color], opp = board[!color];
        ull edge = me & EDGE_MASK;
        //        if(!me){
        //            return 0;
        //        }
        ull left = opp & LEFT_MASK;
        ull right = opp & LEFT_MASK;
        ull ss = ((left >> 1) & edge & (right <<1)) | ((opp >> 8) & edge & (opp << 8));
        ss |= (opp >> 8) & me & (opp << 8) & (left >> 1) & (right << 1);
        return __builtin_popcountll(ss);
    }
#define STABLE_HELPER(orien, d, me) \
    stable |= (stable orien d) & me; \
        adjacent = me & (me orien d); \
        stable |= (stable orien (d+d)) & adjacent; \
        stable |= (stable orien (d+d)) & adjacent; \
        stable |= (stable orien (d+d)) & adjacent

#define STABLE_HELPER2(orien, d, me, s) \
            temp |= (temp orien d) & me; \
        adjacent = me & (me orien d); \
        temp |= (temp orien (d+d)) & adjacent; \
        temp |= (temp orien (d+d)) & adjacent; \
        temp |= (temp orien d) & me; \
        s |= temp

#define LEFT_EDGE_F 0xFE808080808080FE
#define RIGHT_EDGE_F 0x7F0101010101017F
#define LEFT_EDGE 0x8080808080808080
#define RIGHT_EDGE 0x101010101010101
#define INNER_MASK 0x7E7E7E7E7E7E00
        int stable_cnt(int color){
        ull cur = board[color];
        ull cor = cur & CORNER;
        if(!cor){
            return 0;
        }
        ull temp, adjacent, stable;
        stable = cor;
        if(cor & 0x8100000000000000){
            STABLE_HELPER(>>, 8, cur);
        }
        if(cor & 0x81){
            STABLE_HELPER(<<, 8, cur);
        }
        temp = stable & LEFT_EDGE;
        STABLE_HELPER2(>>, 1, cur, stable);
        temp = stable & RIGHT_EDGE;
        STABLE_HELPER2(<<, 1, cur, stable);
        ull t1 = stable & INNER_MASK, t2 = 0;
        stable = stable & EDGE_MASK;
        temp = stable;
        STABLE_HELPER2(>>, 8, cur, t2);
        temp = stable;
        STABLE_HELPER2(<<, 8, cur, t2);
        t2 = t1 & t2;
        stable |= t2;
        return __builtin_popcountll(stable);
        /*
        if(temp){//左边两个
            STABLE_HELPER(>>, 1, cur);
        }
        temp = cor & 0x100000000000001;
        if(temp){//右边两个
            STABLE_HELPER(<<, 1, cur);
        }
        temp = stable;
        STABLE_HELPER(>>, 8, cur);
        STABLE_HELPER(<<, 8, cur);
        t1 = stable;
        stable = stable & EDGE_MASK;
        return __builtin_popcountll(stable);*/
    }
    void back(ull b, ull w){
        board[0] = b, board[1] = w;
        side = !side;
//        emit draw();
    }
    int contain_disc(int x, int y){
        x--, y--;
        ull mask = 1ull << (63 - x * 8 - y);
        if(board[0] & mask){
            return 0;
        }else if(board[1] & mask){
            return 1;
        }else{
            return -1;
        }
    }
    void skip_round(){
        side = !side;
    }
#define CORNER_HELPER(cur) \
    cor = cur & CORNER;\
        left = cur & LEFT_MASK; \
        right = cur & RIGHT_MASK; \
        ss = ((left >> 1) & (~cur) & (right << 1)) | ((cur >> 8) & (~cur) & (cur << 8)); \
        left = cor & LEFT_MASK, right = cor & RIGHT_MASK; \
        insert = ((left >> 1) & ss) | ((right << 1) & ss) | ((cor >> 8) & ss) | ((cor << 8) & ss)


          int compute_corner(int color, bool end){
        ull cur = board[color], opp = board[!color], empty = ~(cur | opp);
        ull cor, ss;
        ull left, right, adj, diag, insert;
        int res = 0;
        CORNER_HELPER(cur);
        res += __builtin_popcountll(cor) * 20;
        res -= __builtin_popcountll(insert & opp) * 12 + __builtin_popcountll(insert & empty) * 6;
        CORNER_HELPER(opp);
        res -= __builtin_popcountll(cor) * 20;
        res += __builtin_popcountll(insert & cur) * 12 + __builtin_popcountll(insert & empty) * 6;
        cor = empty & CORNER;
        left = cor & LEFT_MASK;
        right = cor & RIGHT_MASK;
        adj = cor >> 8 | cor << 8;
        adj |= left >> 1 | right << 1;
        diag = left << 7 | left >> 9 | right >> 7 | right << 9;
        if(end){
            res -= __builtin_popcountll(adj & cur) * 10 + __builtin_popcountll(diag & cur) * 10;
            res += __builtin_popcountll(adj & opp) * 10 + __builtin_popcountll(diag & opp) * 10;
        }else{
            res -= __builtin_popcountll(adj & cur) * 2 + __builtin_popcountll(diag & cur) * 10;
            res += __builtin_popcountll(adj & opp) * 2 + __builtin_popcountll(diag & opp) * 10;
        }
        return res;
    }
#define UNSTABLE_HELPER(d, me, opp, ava, us) \
          adj1 = opp & (opp >> d); \
              adj2 = opp & (opp << d); \
              flip = (me >> d) & opp; \
              flip |= (flip >> d) & opp; \
              flip |= (flip >> (d+d)) & adj1; \
              flip |= (flip >> (d+d)) & adj1; \
              temp = (flip >> d) & empty; \
              ava |= temp;         \
              flip = (temp << d) & opp; \
              flip |= (flip << d) & opp; \
              flip |= (flip << (d+d)) & adj2; \
              flip |= (flip << (d+d)) & adj2; \
              us |= flip;         \
              flip = (me << d) & opp; \
              flip |= (flip << d) & opp; \
              flip |= (flip << (d+d)) & adj2; \
              flip |= (flip << (d+d)) & adj2; \
              temp = (flip << d) & empty; \
              ava |= temp;         \
              flip = (temp >> d) & opp; \
              flip |= (flip >> d) & opp; \
              flip |= (flip >> (d+d)) & adj1; \
              flip |= (flip >> (d+d)) & adj1; \
              us |= flip

    int compute_score(int color, int step, int level){
        if(level == 1){
            int res = compute_score_helper(board[color]) - compute_score_helper(board[!color]);
            return res;
        }else{
            ull cur = board[color], opo = board[!color], empty = ~(cur | opo);
            if(__builtin_popcountll(empty) == 0){
                return  (__builtin_popcountll(cur) - __builtin_popcountll(opo)) * 1000;
            }
            int disc, mobility, a, b, potential_mob, cor, pos, stability;
            a = __builtin_popcountll(cur), b = __builtin_popcountll(opo);
            if(a == 0){
                return -1e9;
            }else if(b == 0){
                return 1e9;
            }
            disc= 100 * (a - b) / (a + b);
            ull inner = opo & MID_MASK, adj1, adj2, temp, flip;
            ull a_cur = 0, a_opo = 0, us_cur = 0, us_opo = 0;
            UNSTABLE_HELPER(1, cur, inner, a_cur, us_opo);
            UNSTABLE_HELPER(8, cur, opo, a_cur, us_opo);
            UNSTABLE_HELPER(7, cur, inner, a_cur, us_opo);
            UNSTABLE_HELPER(9, cur, inner, a_cur, us_opo);
            inner = cur & MID_MASK;
            UNSTABLE_HELPER(1, opo, inner, a_opo, us_cur);
            UNSTABLE_HELPER(8, opo, cur, a_opo, us_cur);
            UNSTABLE_HELPER(7, opo, inner, a_opo, us_cur);
            UNSTABLE_HELPER(9, opo, inner, a_opo, us_cur);
            a = __builtin_popcountll(a_cur), b = __builtin_popcountll(a_opo);
            //        mobility = compute_value(a, b);
            if(a + b == 0){
                return  (__builtin_popcountll(cur) - __builtin_popcountll(opo)) * 1000;
            }
            mobility = a - b;
            //        mobility = 100 * (a - b) / (a + b);
            potential_mob= potential_mobility(color);
            //        a = compute_corner(color), b = compute_corner(!color);
            //        cor = a - b;
            cor = 0;
            if(a_cur & CORNER){
                cor += 20;
            }
            if(a_opo & CORNER){
                cor -= 20;
            }
            //        compute_value(a, b, cor);
            a = __builtin_popcountll(cur & SIDE_) * 5 - __builtin_popcountll(cur & SEC);
            b = __builtin_popcountll(opo & SIDE_) * 5 - __builtin_popcountll(opo & SEC);
            pos = a - b;
            //        compute_value(a, b, pos);
            //        pos = compute_value(a, b);
            //        pos = a + b == 0 ? 0 :100 * (a - b) / (abs(a + b));
            a = 5 * stable_cnt(color) - __builtin_popcountll(us_cur) + 2 * semi_stable_cnt(color); //unstable_cnt(!color, a_opo);
            b = 5 * stable_cnt(!color) - __builtin_popcountll(us_opo) + 2 * semi_stable_cnt(!color); //unstable_cnt(color, a_cur);
            //        stability = compute_value(a, b);
            //        compute_value(a, b, stability);
            stability = a - b;
            //        stability = a + b == 0 ? 0 :100 * (a - b) / abs(a + b);
            int res;

            if(step < 30){
                cor += compute_corner(color, false);
                res = 40 * cor + mobility * 15 + potential_mob * 10 + stability * 25 + pos * 5;
                //            res = 10 * disc + 800 * cor + 78 * mobility + 74 * stability + 35 * potential_mob + 10 * pos;
            }else if(step < 60){
                cor += compute_corner(color, false);
                //            res = 50 * cor + mobility * 10 + potential_mob * 5 + stability * 15 + disc * 5 + pos * 10;
                res = 50 * cor + mobility * 10 + potential_mob * 10 + stability * 30 + pos * 5;
                //            res = 50 * cor + mobility * 15 + potential_mob * 10 + stability * 25 + pos * 5;
                //            res = 40 * cor + mobility * 20 + potential_mob * 10 + stability * 30 + pos * 5;
            }
            else{
                if(a_cur & CORNER){
                    cor += 20;
                }
                if(a_opo & CORNER){
                    cor -= 20;
                }
                cor += compute_corner(color, true);
                res = 10 * disc + 50 * stability + 10 * pos + 10 * cor;
            }
            //        if(res > 0){
            //            posi++;
            //        }else{
            //            nega++;
            //        }
            return res;
        }

    }
    int compute_score_helper(ull cur){
            int res = __builtin_popcountll(cur);
            res += __builtin_popcountll(cur & CORNER) * 1e6;
            res += __builtin_popcountll(cur & SIDE_) * 50;
            res -= __builtin_popcountll(cur & SEC) * 30;
            for(int i = 0; i < 4; i++){
                ull mask_c[4] = {0x8000000000000000, 0x100000000000000, 0x80, 0x1};
                ull mask_diag[4] = {0x40000000000000, 0x2000000000000, 0x4000, 0x200};
                ull mask_adj[4] = {0x4080000000000000, 0x201000000000000, 0x8040, 0x102};
                if(!(cur & mask_c[i])){
                    if(cur & mask_adj[i]){
                        res -= 100;
                    }else if(cur & mask_diag[i]){
                        res -= 10000;
                    }
                }
            }
        return res;
    }
    int cnt(int side){
        return __builtin_popcountll(board[side]);
    }
    int check_winner(){
        int black = cnt(0), white = cnt(1);
        return (black > white) ? 0 : 1;
    }
/*
    int compute_score(int color){
        vector<int> v{1, 8}, v2{2, 7};
        int me = 0, i , j;
        if(board[1][1] == color){
            me += 1e7;
        }else{
            if(board[2][2] == color){
                me -= 1e5;
            }
        }
        if(board[8][1] == color){
            me += 1e7;
        }else{
            if(board[7][2] == color){
                me -= 1e5;
            }
        }
        if(board[1][8] == color){
            me += 1e7;
        }else{
            if(board[2][7] == color){
                me -= 1e5;
            }
        }
        if(board[8][8] == color){
            me += 1e7;
        }else{
            if(board[7][7] == color){
                me -= 1e5;
            }
        }
        for(i = 1; i <= 8; i += 7){
            for(j = 3; j <= 6; j++){
                if(board[i][j] == color){
                    me += 100;
                }
            }
        }
        for(j = 1; j <= 8; j += 7){
            for(i = 3; i <= 6; i++){
                if(board[i][j] == color){
                    me += 100;
                }
            }
        }
        for(i = 1; i <= 8; i += 7){
            if(board[i][1] == color){
                for(j = 2; j <= 8 and board[i][j] == color; j++){
                    me += 100;
                }
            }
            for(j = 1; j <= 8; j++){
                if(board[i][j] == 0)
                    continue;
                if(board[i][j] == color){
                    if((i == 1 or i == 8) and (j == 1 or j == 8)){
                        me += 1e7;
                    }else if((i == 2 or i == 7) and (j == 2 or j == 7)){
                        me -= 1e7;
                    }
                }
            }
        }
        return me;
    }*/
};
#endif // BOARD_H
