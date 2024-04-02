// #include <emscripten.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isEnd(int chessboard[2][7]);

// EMSCRIPTEN_KEEPALIVE
int mancalaBoard(int flag, int seq[], int size, int ans[])
{
    // int ans[15];
    memset(ans, 0, 15);

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
        int beginHole = seq[i] % 10 - 1;
        int beginPerson = seq[i] / 10 - 1;
        // 检查不符合规则操作

        // 不该是该选手行动
        if (tmpFlag != beginPerson)
        {
            printf("不该是该选手行动,tmpFlag=%d,beginPerson=%d\n", tmpFlag, beginPerson);
            int cnt_ans = 0;
            for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
            {
                ans[cnt_ans] = chessboard[0][cnt_chessBoard];
                cnt_ans++;
            }
            for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
            {
                ans[cnt_ans] = chessboard[1][cnt_chessBoard];
                cnt_ans++;
            }
            if (beginPerson == 0)
            {
                ans[14] = 200 + 2 * chessboard[0][6] - 48;
            }
            else
            {
                ans[14] = 200 - 2 * chessboard[1][6] + 48;
            }

            return 0;
        }
        // 游戏已结束
        if (isEnd(chessboard))
        {
            printf("游戏已结束\n");
            int cnt_ans = 0;
            for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
            {
                ans[cnt_ans] = chessboard[0][cnt_chessBoard];
                cnt_ans++;
            }
            for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
            {
                ans[cnt_ans] = chessboard[1][cnt_chessBoard];
                cnt_ans++;
            }
            if (beginPerson == 0)
            {
                ans[14] = 200 + 2 * chessboard[0][6] - 48;
            }
            else
            {
                ans[14] = 200 - 2 * chessboard[1][6] + 48;
            }

            return 0;
        }
        // 选择的地方无棋子
        if (chessboard[beginPerson][beginHole] == 0)
        {
            printf("选择的地方无棋子\n");
            int cnt_ans = 0;
            for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
            {
                ans[cnt_ans] = chessboard[0][cnt_chessBoard];
                cnt_ans++;
            }
            for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
            {
                ans[cnt_ans] = chessboard[1][cnt_chessBoard];
                cnt_ans++;
            }
            if (beginPerson == 0)
            {
                ans[14] = 200 + 2 * chessboard[0][6] - 48;
            }
            else
            {
                ans[14] = 200 - 2 * chessboard[1][6] + 48;
            }

            return 0;
        }

        printf("\n目前棋盘情况\n");
        for (int j = 6; j >= 0; j--)
        {
            printf("%d ", chessboard[1][j]);
        }
        printf("\n  ");
        for (int j = 0; j <= 6; j++)
        {
            printf("%d ", chessboard[0][j]);
        }
        printf("\n");

        printf("输入 %d\n", seq[i]);

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
            // printf("最后一个棋子不是我方计分洞，换人\n");
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

        printf("操作后棋盘情况\n");
        for (int j = 6; j >= 0; j--)
        {
            printf("%d ", chessboard[1][j]);
        }
        printf("\n  ");
        for (int j = 0; j <= 6; j++)
        {
            printf("%d ", chessboard[0][j]);
        }
        printf("\n");
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
        }
    }

    // 假如游戏结束
    if (isEnd(chessboard))
    {
        int cnt_ans = 0;
        for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
        {
            ans[cnt_ans] = chessboard[0][cnt_chessBoard];
            cnt_ans++;
        }
        for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
        {
            ans[cnt_ans] = chessboard[1][cnt_chessBoard];
            cnt_ans++;
        }
        ans[14] = 200 + chessboard[0][6] - chessboard[1][6];
        // 返回先手净胜棋数
        return 0;
    }
    // 假如游戏未结束
    else
    {
        int cnt_ans = 0;
        for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
        {
            ans[cnt_ans] = chessboard[0][cnt_chessBoard];
            cnt_ans++;
        }
        for (int cnt_chessBoard = 0; cnt_chessBoard < 7; cnt_chessBoard++)
        {
            ans[cnt_ans] = chessboard[1][cnt_chessBoard];
            cnt_ans++;
        }
        ans[14] = tmpFlag;

        return 0;
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

int main()
{
    int array[100] = {22, 11, 23, 16, 21, 12, 13, 21, 11, 22, 14, 25, 15, 22, 16, 22};
    int ans[15];
    mancalaBoard(2, array, 16, ans);
    for (int i = 0; i < 15; i++)
        printf("ans = %d ", ans[i]);
    printf("\n");
    return 0;
}