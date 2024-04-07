// #include <emscripten.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int chessboard[2][7]; // 0-5代表己方棋盘，6是计分板
int dfs_choice;
int dfs_score;
int category_choice;

bool isBegin();
bool isEnd();
int dfs(int dep, int beginPerson);
int step(int, int, int[][7]);
int returnByStrategy(int beginPerson);

// EMSCRIPTEN_KEEPALIVE
int mancalaOperator(int flag, int status[]) {
    for (int j = 0; j < 7; j++) {
        chessboard[0][j] = status[j];
    }
    for (int j = 0; j < 7; j++) {
        chessboard[1][j] = status[j + 7];
    }

    dfs_choice = -1;
    dfs_score = dfs(0, flag - 1);
    printf("%d %d\n", dfs_choice, dfs_score);

    // category_choice = returnByStrategy(flag - 1);
    // printf("%d %d\n", category_choice, dfs_choice);
    return 10 * flag + dfs_choice + 1;
}

int dfs(int dep, int person) {
    if (isEnd()) {
        int ans = 0;
        for (int i = 0; i < 7; ++i) {
            ans += chessboard[person][i];
            ans -= chessboard[person ^ 1][i];
        }
        return ans;
    }
    int tmp_chessboard[2][7];
    memcpy(tmp_chessboard, chessboard, sizeof(chessboard));
    int best_score = -50;
    int best_choice = -1;
    for (int i = 0; i < 6; ++i) {
        if (chessboard[person][i] == 0) {
            continue;
        }
        int nxt_person = step(person, i, chessboard);
        int score = dfs(dep + 1, nxt_person);
        if (nxt_person != person) {
            score = -score;
        }
        if (score > best_score) {
            best_score = score;
            best_choice = i;
            if (dep == 0) {
                dfs_choice = i;
            }
        }
        memcpy(chessboard, tmp_chessboard, sizeof(chessboard));
    }
    return best_score;
}

int step(int player, int hole, int num[][7]) {
    int curNum = num[player][hole];
    num[player][hole] = 0;
    int curPlayer = player;
    int curHole = hole;
    while (curNum > 0) {
        ++curHole;

        if (curHole < 6) {
            ++num[curPlayer][curHole];
            --curNum;
        } else if (curHole == 6) {
            if (curPlayer == player) {
                ++num[curPlayer][6];
                --curNum;
            }
        } else {
            curPlayer ^= 1;
            curHole = -1;
        }
    }

    if (curHole == 6) {
        return player;
    }

    if (curPlayer == player && num[curPlayer][curHole] == 1 && num[curPlayer ^ 1][5 - curHole] > 0) {
        num[curPlayer][6] += num[curPlayer][curHole] + num[curPlayer ^ 1][5 - curHole];
        num[curPlayer][curHole] = 0;
        num[curPlayer ^ 1][5 - curHole] = 0;
    }
    player ^= 1;

    return player;
}

bool isEnd() {
    int flag = 0;
    for (int i = 0; i < 6; i++) {
        if (chessboard[0][i] != 0) {
            flag = 1;
        }
    }
    if (flag == 0) {
        return true;
    }
    flag = 0;
    for (int i = 0; i < 6; i++) {
        if (chessboard[1][i] != 0) {
            flag = 1;
        }
    }
    if (flag == 0) {
        return true;
    } else {
        return false;
    }
}

bool isBegin() {
    for (int i = 0; i < 6; ++ i) {
        if (chessboard[0][i] != 4) {
            return 0;
        }
        if (chessboard[1][i] != 4) {
            return 0;
        }
    }
    return 1;
}

int dfs_hole(int dep, int person) {
    int tmp_chessboard[2][7];
    memcpy(tmp_chessboard, chessboard, sizeof(chessboard));
    int ans = 0;
    for (int i = 0; i < 6; ++ i) {
        if (chessboard[person][i] == 0) {
            continue;
        }
        if (step(person, i, chessboard) == person) {
            int new_ans = dfs_hole(dep + 1, person) + 1;
            if (new_ans > ans) {
                ans = new_ans;
                if (dep == 0) {
                    category_choice = i;
                }
            }
        }
        memcpy(chessboard, tmp_chessboard, sizeof(chessboard));
    }
    return ans;
}

int returnByStrategy(int person) {
    if (isBegin()) {
        return 2;
    }
    category_choice = -1;
    int cnt = dfs_hole(0, person);
    if (category_choice != -1) {
        return category_choice;
    }
    do {
        category_choice = rand() % 6;
    } while(chessboard[person][category_choice] == 0);
    return category_choice;
}

int main() {
    int array[14] = {0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 4, 0, 1};
    mancalaOperator(1, array);
    return 0;
}