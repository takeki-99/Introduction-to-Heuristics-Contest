#include <bits/stdc++.h>
using namespace std;
const int inf = 1e9 + 7;

struct Solver
{
    int D;
    vector<int> c;
    vector<vector<int>> s;
    Solver(int D, vector<int> c, vector<vector<int>> s) : D(D), c(c), s(s) {}

    vector<int> solve()
    {
        vector<int> t(0);

        //貪欲にスコアが高くなるものを選ぶ
        for (int i = 0; i < D; i++)
        {
            int max_score = -inf;
            int max_j = 0;
            for (int j = 0; j < 26; j++)
            {
                t.push_back(j);
                int tmp_score = calc_score(t);
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

    // ifstream in("./input.in");
    // cin.rdbuf(in.rdbuf());

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

    for (int i = 0; i < D; i++)
    {
        cout << t[i] + 1 << endl;
    }
}