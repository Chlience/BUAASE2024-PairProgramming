#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int mancalaResult(int begin, int seq[], int size);
int test_normal(int cnt);

int q_begin[100], q_seq[100][100], q_size[100], q_result[100];

void test_print(int cnt)
{
	printf("assert.strictEqual(mancalaResult(");
	printf("%d, ", q_begin[cnt]);
	for (int i = 0; i < q_size[cnt]; ++i)
	{
		if (i == 0)
		{
			printf("[");
		}
		else
		{
			printf(", ");
		}
		printf("%d", q_seq[cnt][i]);
	}
	printf("], %d), %d);\n", q_size[cnt], q_result[cnt]);
}

int main()
{
	srand(time(NULL));
	for (int i = 0; i < 10; ++i)
	{
		test_normal(i);
		q_result[i] = mancalaResult(q_begin[i], q_seq[i], q_size[i]);
		test_print(i);
	}
	return 0;
}

int isEnd(int num[][7])
{
	int flag[2] = {1, 1};
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j <= 5; ++j)
		{
			if (num[i][j] > 0)
			{
				flag[i] = 0;
				break;
			}
		}
	}
	return flag[0] || flag[1];
}

int mancalaResult(int begin, int seq[], int size)
{
	int num[2][7];
	for (int i = 0; i < 6; ++i)
	{
		num[0][i] = 4;
		num[1][i] = 4;
	}
	num[0][6] = 0;
	num[1][6] = 0;

	int player = begin - 1;
	for (int i = 0; i < size; ++i)
	{
		if (isEnd(num))
		{
			// test
			return 30000 + i;
		}

		int curPlayer = seq[i] / 10 - 1;
		int curHole = seq[i] % 10 - 1;

		if (curPlayer != player)
		{
			// test
			return 30000 + i;
		}

		if (num[curPlayer][curHole] == 0)
		{
			// test
			return 30000 + i;
		}

		int curNum = num[curPlayer][curHole];
		num[curPlayer][curHole] = 0;
		// Hole 编号为 0-5 为正常的 Hole，6 为 Mancala
		while (curNum > 0)
		{
			++curHole;

			if (curHole < 6)
			{
				num[curPlayer][curHole]++;
				curNum--;
			}
			else if (curHole == 6)
			{
				if (curPlayer == player)
				{
					num[curPlayer][6]++;
					curNum--;
				}
				else
				{
					//
				}
			}
			else
			{
				curPlayer ^= 1;
				curHole = -1;
			}
		}

		// 观察结束情况
		if (curHole == 6)
		{
			// player = player;
		}
		else if (curPlayer == player && num[curPlayer][curHole] == 1 && num[curPlayer ^ 1][5 - curHole] > 0)
		{
			// test
			num[curPlayer][6] += num[curPlayer][curHole] + num[curPlayer ^ 1][5 - curHole];
			num[curPlayer][curHole] = 0;
			num[curPlayer ^ 1][5 - curHole] = 0;
			player ^= 1;
		}
		else
		{
			player ^= 1;
		}
	}

	// 双方棋子加入其计分洞
	for (int i = 0; i < 6; i++)
	{
		num[0][6] += num[0][i];
		num[1][6] += num[1][i];
		num[0][i] = 0;
		num[1][i] = 0;
	}

	if (isEnd(num))
	{
		return 15000 + num[begin - 1][6] - num[(begin - 1) ^ 1][6];
	}
	else
	{
		return 20000 + num[begin - 1][6];
	}
}

int getHole(int player, int num[][7])
{
	int stack[6], top = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (num[player][i] > 0)
		{
			stack[top++] = i;
		}
	}
	int p = rand() % top;
	return stack[p];
}

int step(int player, int hole, int num[][7])
{
	int curNum = num[player][hole];
	num[player][hole] = 0;
	int curPlayer = player;
	int curHole = hole;
	while (curNum > 0)
	{
		++curHole;

		if (curHole < 6)
		{
			num[curPlayer][curHole]++;
			curNum--;
		}
		else if (curHole == 6)
		{
			if (curPlayer == player)
			{
				num[curPlayer][6]++;
				curNum--;
			}
			else
			{
			}
		}
		else
		{
			curPlayer ^= 1;
			curHole = -1;
		}
	}

	// 观察结束情况
	if (curHole == 6)
	{
	}
	else if (curPlayer == player && num[curPlayer][curHole] == 1 && num[curPlayer ^ 1][5 - curHole] > 0)
	{
		num[curPlayer][6] += num[curPlayer][curHole] + num[curPlayer ^ 1][5 - curHole];
		num[curPlayer][curHole] = 0;
		num[curPlayer ^ 1][5 - curHole] = 0;
		player ^= 1;
	}
	else
	{
		player ^= 1;
	}
	return player;
}

int test_normal(int cnt)
{
	int player = rand() % 2;
	int num[2][7];
	for (int i = 0; i < 6; ++i)
	{
		num[0][i] = 4;
		num[1][i] = 4;
	}
	num[0][6] = 0;
	num[1][6] = 0;
	int seq[100];
	int size = 0;
	q_begin[cnt] = player + 1;
	while (!isEnd(num))
	{
		int hole = getHole(player, num);
		q_seq[cnt][q_size[cnt]++] = (player + 1) * 10 + hole + 1;
		player = step(player, hole, num);
	}
	return 0;
}
