#include <bits/stdc++.h>
using namespace std;
const int inf = 1e9 + 7;
double START_CLOCK;
double TL = 1.90;
double runtime()
{
    return (clock() - START_CLOCK) / CLOCKS_PER_SEC;
}

struct Solver
{
    int D;
    vector<int> c;
    vector<vector<int>> s;
    Solver(int D, vector<int> c, vector<vector<int>> s) : D(D), c(c), s(s) {}

    vector<int> solve(const int k)
    {
        vector<int> t(0);

        // i日からi + k日まで何もコンテストを開かなかったときのスコアが最も高いコンテストjを選ぶ。
        for (int i = 0; i < D; i++)
        {
            if (runtime() > TL)
            {
                break;
            }
            int max_score = -inf;
            int max_j = 0;
            for (int j = 0; j < 26; j++)
            {
                t.push_back(j);
                int tmp_score = evaluate(t, k);
                if (max_score < tmp_score)
                {
                    max_score = tmp_score;
                    max_j = j;
                }
                t.pop_back();
            }
            t.push_back(max_j);
        }
        return t;
    }

    // tの長さd(<=26)に合わせて計算する。
    // d日からd+k日まで何もコンテストを開かなかったときのスコアを返す。
    int evaluate(const vector<int> &t, const int k)
    {
        int score = 0;
        vector<int> last(26, 0);
        for (int i = 0; i < (int)t.size(); i++)
        {
            last[t[i]] = i + 1;
            score += s[i][t[i]];
            for (int j = 0; j < 26; j++)
            {
                score -= c[j] * (i + 1 - last[j]);
            }
        }
        for (int i = (int)t.size(); i < min((int)t.size() + k, D); i++)
        {
            for (int j = 0; j < 26; j++)
            {
                score -= c[j] * (i + 1 - last[j]);
            }
        }
        return score;
    }

    // tの長さ(<=26)に合わせて計算する。
    int calc_score(const vector<int> &t)
    {
        int score = 0;
        vector<int> last(26, 0);
        for (int i = 0; i < (int)t.size(); i++)
        {
            last[t[i]] = i + 1;
            score += s[i][t[i]];
            for (int j = 0; j < 26; j++)
            {
                score -= c[j] * (i + 1 - last[j]);
            }
        }
        return score;
    }
};

int main()
{
    START_CLOCK = clock();

    ifstream in("./input.in");
    cin.rdbuf(in.rdbuf());

    int D;
    cin >> D;
    vector<int> c(26);
    for (int i = 0; i < 26; i++)
    {
        cin >> c[i];
    }
    vector<vector<int>> s(D, vector<int>(26));
    for (int i = 0; i < D; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            cin >> s[i][j];
        }
    }

    Solver solver(D, c, s);

    int max_score = -inf;
    vector<int> best_t;
    // kを時間のあるかぎり探索
    for (int k = 0; k <= 26; k++)
    {
        if (runtime() > TL)
        {
            break;
        }
        vector<int> tmp_t = solver.solve(k);
        int tmp_score = solver.calc_score(tmp_t);
        // 更新
        if (max_score < tmp_score)
        {
            max_score = tmp_score;
            swap(best_t, tmp_t);
        }
    }
    cerr << "Score = " << solver.calc_score(best_t) << endl;
    cerr << "time = " << runtime() << endl;

    for (int i = 0; i < D; i++)
    {
        cout << best_t[i] + 1 << endl;
    }
}