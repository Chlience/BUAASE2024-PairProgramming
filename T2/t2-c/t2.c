#include <emscripten.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isEnd(int chessboard[2][7]);

EMSCRIPTEN_KEEPALIVE
int mancalaResult(int flag, int seq[], int size)
{
    int chessboard[2][7]; // 0-5代表己方棋盘，6是计分板
    // 初始化
    memset(chessboard, 0, sizeof(chessboard));
    for (int j = 0; j < 6; j++)
    {
        chessboard[0][j] = 4;
        chessboard[1][j] = 4;
    }
    chessboard[0][6] = 0;
    chessboard[1][6] = 0;
    flag -= 1;

    int tmpFlag = flag;
    // 运行
    for (int i = 0; i < size; i++)
    {
        int beginHole = seq[i] % 10;
        int beginPerson = seq[i] / 10 - 1;

        // 检查不符合规则操作

        // 不该是该选手行动
        if (tmpFlag != beginPerson)
        {
            // return tmpFlag;
            return 30000 + i;
        }
        // 游戏已结束
        if (isEnd(chessboard))
        {
            return 30000 + i;
        }

        // 分配
        int numChess = chessboard[beginPerson][beginHole];
        chessboard[beginPerson][beginHole] = 0;

        int cnt = 0; // 分了的棋子数目
        int tmpPerson = beginPerson;
        int tmpHole = beginHole;

        while (cnt < numChess)
        {
            tmpHole++;
            // 如果不在计分牌内
            if (tmpHole < 6)
            {
                chessboard[tmpPerson][tmpHole]++;
                cnt++;
            }
            // 如果在计分牌内，假如是自己，则放进计分板，不是自己则跳过。
            else if (tmpHole == 6)
            {
                if (tmpPerson == beginPerson)
                {
                    chessboard[tmpPerson][tmpHole]++;
                    cnt++;
                }
            }
            // 超出棋盘数目
            else
            {
                tmpPerson ^= 1;
                tmpHole = -1;
            }
        }
        // 讨论最后一个棋子的情况。
        // 假如是我方计分洞（只可能是我方）
        if (tmpHole == 6)
        {
            // printf("最后一个棋子是我方计分洞\n");
            continue;
        }
        else
        {
            // printf("最后一个棋子不是我方计分洞\n");
            tmpFlag ^= 1;
            // 是己方无棋子的棋洞且对方棋洞有棋子
            if (tmpPerson == beginPerson && chessboard[tmpPerson][tmpHole] == 1 && chessboard[tmpPerson ^ 1][5 - tmpHole] != 0)
            {
                chessboard[beginPerson][6] += chessboard[tmpPerson][tmpHole];
                chessboard[beginPerson][6] += chessboard[tmpPerson ^ 1][5 - tmpHole];
                chessboard[tmpPerson][tmpHole] = 0;
                chessboard[tmpPerson ^ 1][5 - tmpHole] = 0;
            }
            // 其他情况无操作
        }
    }

    // 假如游戏结束
    if (isEnd(chessboard))
    {
        // 双方棋子加入其计分洞
        for (int i = 0; i < 6; i++)
        {
            chessboard[0][6] += chessboard[0][i];
            chessboard[1][6] += chessboard[1][i];
            chessboard[0][i] = 0;
            chessboard[1][i] = 0;
        }
        // 返回先手净胜棋数
        return 15000 + chessboard[flag][6] - chessboard[flag ^ 1][6];
    }
    // 假如游戏未结束
    else
    {
        // printf("tmpFlag=%d ", tmpFlag);
        return 20000 + chessboard[flag][6];
    }
}

bool isEnd(int chessboard[2][7])
{
    int flag = 0;
    for (int i = 0; i < 6; i++)
    {
        if (chessboard[0][i] != 0)
        {
            flag = 1;
        }
    }
    if (flag == 0)
    {
        return true;
    }
    flag = 0;
    for (int i = 0; i < 6; i++)
    {
        if (chessboard[1][i] != 0)
        {
            flag = 1;
        }
    }
    if (flag == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// int main()
// {
//     int array[1] = {14};
//     printf("ans = %d\n", mancalaResult(1, array, 1));
//     return 0;
// }