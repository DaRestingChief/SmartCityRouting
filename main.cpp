
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
struct Stop
{
    StopID id;
    string name;
    Point loc;
};

struct Edge
{
    StopID to;
    double weight;
};

struct Graph
{
    vector<Stop> stops;
    unordered_map<string, StopID> nameToId;
    vector<vector<Edge>> adj;

    StopID add_stop(const string &name, double x = 0.0, double y = 0.0)
    {
        string tn = trim(name);
        if (nameToId.count(tn)) return nameToId[tn];
        StopID id = stops.size();
        stops.push_back({id, tn, {x, y}});
        nameToId[tn] = id;
        adj.emplace_back();
        logger.log("Added stop: " + tn);
        return id;
    }

    bool has_stop(const string &name)
    {
        return nameToId.count(trim(name));
    }

    StopID get_id(const string &name)
    {
        string tn = trim(name);
        if (!nameToId.count(tn)) return -1;
        return nameToId[tn];
    }

    string get_name(StopID id)
    {
        if (id < 0 || id >= (StopID)stops.size()) return "";
        return stops[id].name;
    }

    void add_edge(const string &a, const string &b, double w, bool bidir = true)
    {
        StopID u = add_stop(a);
        StopID v = add_stop(b);
        adj[u].push_back({v, w});
        if (bidir) adj[v].push_back({u, w});
        logger.log("Added edge: " + a + " <-> " + b);
    }

    vector<pair<StopID, double>> neighbors(StopID u)
    {
        vector<pair<StopID, double>> out;
        for (auto &e : adj[u]) out.push_back({e.to, e.weight});
        return out;
    }

    size_t size()
    {
        return stops.size();
    }

    bool save_to(const string &sf, const string &ef)
    {
        ofstream s(sf), e(ef);
        if (!s || !e) return false;
        for (auto &st : stops) s << st.id << "\t" << st.name << "\t" << st.loc.x << "\t" << st.loc.y << "\n";
        for (size_t i = 0; i < adj.size(); ++i)
        {
            for (auto &x : adj[i]) e << i << "\t" << x.to << "\t" << x.weight << "\n";
        }
        logger.log("Saved graph data.");
        return true;
    }

    bool load_from(const string &sf, const string &ef)
    {
        ifstream s(sf), e(ef);
        if (!s || !e) return false;
        stops.clear(); nameToId.clear(); adj.clear();

        string line;
        while (getline(s, line))
        {
            if (trim(line).empty()) continue;
            stringstream ss(line);
            int id; string name; double x, y;
            ss >> id >> name >> x >> y;
            if ((int)stops.size() <= id) stops.resize(id + 1);
            stops[id] = {id, name, {x, y}};
            nameToId[name] = id;
            adj.resize(stops.size());
        }
        while (getline(e, line))
        {
            if (trim(line).empty()) continue;
            stringstream ss(line);
            int u, v; double w;
            ss >> u >> v >> w;
            adj[u].push_back({v, w});
        }
        logger.log("Loaded graph data.");
        return true;
    }
};

