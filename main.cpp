

#include <bits/stdc++.h>
using namespace std;


typedef int StopID;
struct Point
{
    double x, y;
};
inline double euclidean(const Point &a, const Point &b)
{
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
inline string trim(const string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos)
        return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// ----------------------------- Logger (simple, thread-unsafe) -----------------------------
struct Logger
{
    deque<string> q; // history queue
    size_t maxlen = 1000;
    void log(const string &s)
    {
        q.push_back(s);
        if (q.size() > maxlen)
            q.pop_front();
    }
    void print_recent(size_t n = 20)
    {
        cout << "---- Recent logs (last " << (std::min)(n, q.size()) << ") ----\n";
        size_t start = q.size() > n ? q.size() - n : 0;
        for (size_t i = start; i < q.size(); ++i)
            cout << q[i] << "\n";
        cout << "--------------------------------------\n";
    }
} logger;
