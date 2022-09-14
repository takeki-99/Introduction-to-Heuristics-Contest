#include <bits/stdc++.h>
using namespace std;
const int inf = 1e9 + 7;
double START_CLOCK;
double TimeLimit = 1.90;
double getRuntime()
{
    return (clock() - START_CLOCK) / CLOCKS_PER_SEC;
}
double rangeRand(double FLOAT_MIN = 0.0, double FLOAT_MAX = 1.0)
{
    return FLOAT_MIN + ((double)rand() / RAND_MAX) * (FLOAT_MAX - FLOAT_MIN);
}

struct Solver
{
    // for Annealing
    const double T_h = 2e3;
    const double T_l = 6e2;

    // Input
    const int D;
    const vector<int> c;
    const vector<vector<int>> s;

    Solver(int D, const vector<int> &c, const vector<vector<int>> &s) : D(D), c(c), s(s) {}

    struct State
    {
        vector<int> t;
        int score;
        // 各コンテスト毎の開催日
        // 1-index
        vector<vector<int>> ds_1;
        State(const Solver &solver, const vector<int> &t) : t(t)
        {
            ds_1.resize(26);
            for (int i = 0; i < solver.D; i++)
            {
                ds_1[t[i]].push_back(i + 1);
            }
            score = solver.calc_score(t);
        }

        int gap_cost(int d) const
        {
            return d * (d - 1) / 2;
        }

        // d_0日目(0-index)をcontest new_iに変更する。
        void change(const Solver &solver, int d_0, int new_i)
        {
            int old_i = t[d_0];

            // old_iのgap_costに関する処理
            auto old_i_it = find(ds_1[old_i].begin(), ds_1[old_i].end(), d_0 + 1);
            int prev = (old_i_it == ds_1[old_i].begin() ? 0 : old_i_it[-1]);
            int next = ((old_i_it + 1) == ds_1[old_i].end() ? solver.D + 1 : old_i_it[1]);
            ds_1[old_i].erase(old_i_it);
            score -= (gap_cost(next - prev) - gap_cost(next - (d_0 + 1)) - gap_cost(d_0 + 1 - prev)) * solver.c[old_i];

            // new_iのgap_costに関する処理
            auto new_i_it = lower_bound(ds_1[new_i].begin(), ds_1[new_i].end(), d_0 + 1);
            prev = (new_i_it == ds_1[new_i].begin() ? 0 : new_i_it[-1]);
            next = ((new_i_it) == ds_1[new_i].end() ? solver.D + 1 : *new_i_it);
            ds_1[new_i].insert(new_i_it, d_0 + 1);
            score += (gap_cost(next - prev) - gap_cost(next - (d_0 + 1)) - gap_cost(d_0 + 1 - prev)) * solver.c[new_i];

            // sに関する処理
            score += solver.s[d_0][new_i] - solver.s[d_0][old_i];
            t[d_0] = new_i;
        }
    };

    vector<int> solve()
    {
        srand(0);

        vector<int> ret(D);

        // random solution
        for (int i = 0; i < D; i++)
        {
            ret[i] = rand() % 26;
        }

        State state(*this, ret);

        int best_score = state.score;
        vector<int> best_t = state.t;

        int loop_count = 0;
        double T; // temperature for annealing

        while (true)
        {
            if (loop_count % 100 == 0)
            {
                double T_ratio = getRuntime() / TimeLimit;
                if (T_ratio >= 1.0)
                {
                    break;
                }
                //　指数スケジューリングで温度Tを決める
                T = pow(T_h, 1.0 - T_ratio) * pow(T_l, T_ratio);
            }

            int old_score = state.score;
            if (rand() % 2)
            {
                // d日目をqに変更する
                int d = rand() % D;
                int q = rand() % 26;
                int old_q = state.t[d];
                state.change(*this, d, q);

                if ((old_score > state.score) &&
                    (rangeRand() > exp((double)(state.score - old_score) / T)))
                {
                    // reset the change
                    state.change(*this, d, old_q);
                }
            }
            else
            {
                // d1日目とd2日目を交換する
                int d1 = rand() % D;
                int d2 = min(d1 + 1 + rand() % 16, D - 1);
                int q1 = state.t[d1], q2 = state.t[d2];
                state.change(*this, d1, q2);
                state.change(*this, d2, q1);
                if ((old_score > state.score) &&
                    (rangeRand() > exp((double)(state.score - old_score) / T)))
                {
                    // reset the change
                    state.change(*this, d1, q1);
                    state.change(*this, d2, q2);
                }
            }

            // update best_score and best_t
            if (best_score < state.score)
            {
                best_score = state.score;
                best_t = state.t;
            }
            loop_count++;
        }

        return best_t;
    }

    // tの長さ(<=D)に合わせて計算する。
    int
    calc_score(const vector<int> &t) const
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
    cerr << "time = " << getRuntime() << endl;

    for (int i = 0; i < D; i++)
    {
        cout << t[i] + 1 << endl;
    }
}