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

    const int D;
    const vector<int> c;
    const vector<vector<int>> s;
    Solver(int D, const vector<int> &c, const vector<vector<int>> &s) : D(D), c(c), s(s) {}

    struct State
    {
        vector<int> t;
        int score;
        // 各コンテスト毎の開催日
        vector<vector<int>> ds;
        State(const Solver &solver, const vector<int> &t) : t(t)
        {
            ds.resize(26);
            for (int i = 0; i < (int)t.size(); i++)
            {
                ds[t[i]].push_back(i);
            }
            score = solver.calc_score(t);
        }

        int cost(int d) const
        {
            return d * (d - 1) / 2;
        }

        // d日目をcontest new_iに変更する。
        void change(int d, int new_i)
        {
            t[d] = new_i;
        }
    };

    vector<int> solve()
    {
        // srand((unsigned)time(NULL));
        srand(0);

        vector<int> ret(D);

        // random solution
        for (int i = 0; i < D; i++)
        {
            ret[i] = rand() % 26;
        }

        int score = calc_score(ret);

        while (runtime() < TL)
        {
            if (rand() % 2)
            {
                int d = rand() % D;
                int q = rand() % 26;
                int old = ret[d];
                ret[d] = q;
                int tmp_score = calc_score(ret);
                if (score < tmp_score)
                {
                    score = tmp_score;
                }
                else
                {
                    ret[d] = old;
                }
            }
            else
            {
                int d1 = rand() % D;
                int d2 = min(d1 + 1 + rand() % 16, D - 1);
                swap(ret[d1], ret[d2]);
                int tmp_score = calc_score(ret);
                if (score < tmp_score)
                {
                    score = tmp_score;
                }
                else
                {
                    swap(ret[d1], ret[d2]);
                }
            }
        }

        return ret;
    }

    /*
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
    */

    // tの長さ(<=26)に合わせて計算する。
    int calc_score(const vector<int> &t) const
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

    vector<int> t = solver.solve();
    cerr << "Score = " << solver.calc_score(t) << endl;
    cerr << "time = " << runtime() << endl;

    for (int i = 0; i < D; i++)
    {
        cout << t[i] + 1 << endl;
    }
}