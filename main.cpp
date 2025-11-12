
#include <bits/stdc++.h>
using namespace std;

// to store stops 
typedef int StopID; 
struct Point 
{
    double x, y;
};
double euclidean(const Point &a, const Point &b)
{
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
string trim(const string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// to log messages
struct Logger
{
    deque<string> q; 
    size_t maxlen = 1000;
    void log(const string &s)
    {
        q.push_back(s);
        if (q.size() > maxlen) q.pop_front();
    }
    void print_recent(size_t n = 20)
    {
        size_t m = std::min(n, q.size());
        cout << "---- Recent logs (last " << m << ") ----\n";
        size_t start = q.size() > n ? q.size() - n : 0;
        for (size_t i = start; i < q.size(); ++i) cout << q[i] << "\n";
        cout << "--------------------------------------\n";
    }
} logger;

// for autocomplete of stop names
struct TrieNode
{
    bool end;
    unordered_map<char, TrieNode*> nxt;
    TrieNode() : end(false) {}
    ~TrieNode()
    {
        for (auto &p : nxt) delete p.second;
    }
};
struct Trie
{
    TrieNode *root;
    Trie() { root = new TrieNode(); }
    ~Trie() { delete root; }

    void insert(const string &s)
    {
        TrieNode *cur = root;
        for (size_t i = 0; i < s.size(); ++i)
        {
            char ch = s[i];
            if (!cur->nxt.count(ch)) cur->nxt[ch] = new TrieNode();
            cur = cur->nxt[ch];
        }
        cur->end = true;
    }

    void collect(TrieNode *node, const string &prefix, vector<string> &out)
    {
        if (!node) return;
        if (node->end) out.push_back(prefix);
        for (auto &p : node->nxt) collect(p.second, prefix + p.first, out);
    }

    vector<string> suggest(const string &prefix)
    {
        TrieNode *cur = root;
        for (size_t i = 0; i < prefix.size(); ++i)
        {
            char c = prefix[i];
            if (!cur->nxt.count(c)) return vector<string>();
            cur = cur->nxt[c];
        }
        vector<string> out;
        collect(cur, prefix, out);
        return out;
    }
};
