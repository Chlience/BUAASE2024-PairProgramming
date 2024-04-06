#include <emscripten.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 记录了对于做决策的人的最优解，做决策的人行动，则是最大值，对方行动，则是最小值
int stack[1000];
// 记录最优解的选择是什么
int bset_action = 0;
int chessboard[2][7]; // 0-5代表己方棋盘，6是计分板
int depth = 0;
// 做决策的人
int person;

int dfs(int dep, int beginPerson);
bool isEnd();
int returnByStrategy(int beginPerson);

EMSCRIPTEN_KEEPALIVE
int mancalaOperator(int flag, int status[])
{
    person = flag - 1;

    memset(stack, 0, sizeof(stack));

    // 初始化
    memset(chessboard, 0, sizeof(chessboard));
    for (int j = 0; j < 7; j++)
    {
        chessboard[0][j] = status[j];
    }
    for (int j = 0; j < 7; j++)
    {
        chessboard[1][j] = status[j + 7];
    }
    int tmp = returnByStrategy(person);
    // if (tmp == -1)
    // {
    //     dfs(0, person);
    // }
    // else
    // {
    bset_action = tmp;
    // }
    return 10 * (person + 1) + bset_action + 1;
}

int dfs(int dep, int beginPerson)
{
    // printf("dep=%d\n", dep);
    int best_ans = 0;
    if (isEnd())
    {
        // printf("无棋子， 结束\n");
        int ans_0 = chessboard[0][6];
        int ans_1 = chessboard[1][6];
        for (int i = 0; i < 6; i++)
        {
            ans_0 += chessboard[0][i];
            ans_1 += chessboard[1][i];
        }
        // printf("best_ans =%d \n", ans_0 - ans_1);

        return ans_0 - ans_1;
    }
    if (dep == 5)
    {
        int ans_0 = chessboard[0][6];
        int ans_1 = chessboard[1][6];
        return ans_0 - ans_1;
    }

    if (beginPerson == person)
    {
        best_ans = -48;
    }
    else
    {
        best_ans = 48;
    }
    // for 循环
    for (int beginHole = 0; beginHole < 6; beginHole++)
    {
        int cnt = 0; // 分了的棋子数目
        int tmpPerson = beginPerson;
        int tmpHole = beginHole;
        int numChess = chessboard[tmpPerson][beginHole];
        int dfs_ans;
        // 假如棋盘没棋子，继续下一种可能
        if (numChess == 0)
        {
            continue;
        }
        // 存对方有棋子的棋洞
        int tmpChessInOpponent;
        int flag = 0;
        //  do choice

        // printf("dep=%d", dep);
        // printf("\n目前棋盘情况\n");
        // for (int j = 6; j >= 0; j--)
        // {
        //     printf("%d ", chessboard[1][j]);
        // }
        // printf("\n  ");
        // for (int j = 0; j <= 6; j++)
        // {
        //     printf("%d ", chessboard[0][j]);
        // }
        // printf("\n");

        chessboard[beginPerson][beginHole] = 0;

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

        // dfs (dep + 1)
        // 讨论最后一个棋子的情况。
        // 假如是我方计分洞（只可能是我方）
        if (tmpHole == 6)
        {
            // printf("最后一个棋子是我方计分洞\n");
            dfs_ans = dfs(dep + 1, beginPerson);
        }
        else
        {
            // 是己方无棋子的棋洞且对方棋洞有棋子
            if (tmpPerson == beginPerson && chessboard[tmpPerson][tmpHole] == 1 && chessboard[tmpPerson ^ 1][5 - tmpHole] != 0)
            {
                flag = 1;
                chessboard[beginPerson][6] += chessboard[tmpPerson][tmpHole];
                tmpChessInOpponent = chessboard[tmpPerson ^ 1][5 - tmpHole];
                chessboard[beginPerson][6] += tmpChessInOpponent;
                chessboard[tmpPerson][tmpHole] = 0;
                chessboard[tmpPerson ^ 1][5 - tmpHole] = 0;
            }
            // 其他情况无操作
            // printf("最后一个棋子不是我方计分洞，换人\n");

            dfs_ans = dfs(dep + 1, beginPerson ^ 1);
        }

        // back choice
        // 讨论最后一个棋子的情况。
        // 假如是我方计分洞（只可能是我方）
        if (tmpHole == 6)
        {
        }
        else
        {
            // 是己方无棋子的棋洞且对方棋洞有棋子
            if (flag == 1)
            {
                chessboard[beginPerson][6] -= 1;
                chessboard[tmpPerson][tmpHole] = 1;

                chessboard[beginPerson][6] -= tmpChessInOpponent;
                chessboard[tmpPerson ^ 1][5 - tmpHole] = tmpChessInOpponent;
            }
            // 其他情况无操作
        }

        cnt = 0;
        tmpHole++;
        while (cnt < numChess)
        {
            tmpHole--;
            // 如果不在计分牌内
            if (6 > tmpHole && tmpHole >= 0)
            {
                chessboard[tmpPerson][tmpHole]--;
                cnt++;
            }
            // 如果在计分牌内，假如是自己，则放进计分板，不是自己则跳过。
            else if (tmpHole == 6)
            {
                if (tmpPerson == beginPerson)
                {
                    chessboard[tmpPerson][tmpHole]--;
                    cnt++;
                }
            }
            // 超出棋盘数目
            else
            {
                tmpPerson ^= 1;
                tmpHole = 7;
            }
        }
        chessboard[beginPerson][beginHole] = numChess;
        // update best ans

        if (beginPerson == person)
        {
            if (dfs_ans > best_ans)
            {
                best_ans = dfs_ans;
                bset_action = beginHole;
            }
        }
        else
        {
            if (dfs_ans < best_ans)
            {
                best_ans = dfs_ans;
                bset_action = beginHole;
            }
        }
        // printf("dep=%d\n", dep);
        // printf("\n结束棋盘情况\n");
        // for (int j = 6; j >= 0; j--)
        // {
        //     printf("%d ", chessboard[1][j]);
        // }
        // printf("\n  ");
        // for (int j = 0; j <= 6; j++)
        // {
        //     printf("%d ", chessboard[0][j]);
        // }
        // printf("\n");
    }
    // printf("走完了该层的节点\n");
    return best_ans;
}

bool isEnd()
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

int returnByStrategy(int beginPerson)
{
    // 有3走3，没3走右
    if (chessboard[beginPerson][2] == 4)
    {
        return 2;
    }
    // 从右往左查看
    for (int i = 5; i >= 0; i--)
    {
        if (chessboard[beginPerson][i] == 6 - i && chessboard[beginPerson][i] != 0)
            return i;
    }
    // 清空最右
    if (chessboard[beginPerson][5] != 0)
    {
        return 5;
    }

    // 假如可以取对方的子
    for (int i = 0; i < 6; i++)
    {
        int tmp = chessboard[beginPerson][i];
        if (tmp != 0 && i + tmp >= 0 && i + tmp < 6 && chessboard[beginPerson][5 - (i + tmp)] >= 2)
        {
            return i;
        }
    }

    // 避免取子
    for (int i = 0; i < 6; i++)
    {
        if (chessboard[beginPerson ^ 1][5 - i] == 0 && chessboard[beginPerson][i] > 3)
        {
            return i;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        if (chessboard[beginPerson][i] != 0)
            return i;
    }
    // 返回一个最好的
    return -1;
}

// int main()
// {
//     int array[14] = {6, 0, 7, 7, 3, 1, 0, 0, 6, 6, 6, 5, 0, 1};

//     printf("ans = %d\n", mancalaOperator(1, array));
// }