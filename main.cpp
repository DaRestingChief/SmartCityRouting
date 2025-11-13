#include <bits/stdc++.h>
using namespace std;

// Utility types and helper functions
typedef int StopID;
struct Point
{
    double x, y;
};
// Calculate distance between two points
double euclidean(const Point &a, const Point &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
// Remove whitespace from string
string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Simple logging
struct Logger
{
    deque<string> q;
    size_t maxlen = 1000;
    void log(const string &s)
    {
        q.push_back(s);
        if (q.size() > maxlen)
            q.pop_front();
    }
    void print_recent(size_t n = 20)
    {
        cout << "Recent logs (last " << n << ":" << endl;
        size_t start = 0;
        if (q.size() > n)
            start = q.size() - n;
        for (size_t i = start; i < q.size(); i++)
            cout << q[i] << endl;
    }
} logger;

// Trie for autocomplete
struct TrieNode
{
    bool end;
    unordered_map<char, TrieNode *> nxt;
    TrieNode() { end = false; }
};
struct Trie
{
    TrieNode *root;
    Trie() { root = new TrieNode(); }
    void insert(const string &s)
    {
        TrieNode *cur = root;
        for (int i = 0; i < s.length(); i++)
        {
            char ch = s[i];
            if (cur->nxt.find(ch) == cur->nxt.end())
                cur->nxt[ch] = new TrieNode();
            cur = cur->nxt[ch];
        }
        cur->end = true;
    }
    void collect(TrieNode *node, const string &prefix, vector<string> &out)
    {
        if (node == NULL)
            return;
        if (node->end)
            out.push_back(prefix);
        for (auto p : node->nxt)
            collect(p.second, prefix + p.first, out);
    }
    vector<string> suggest(const string &prefix)
    {
        TrieNode *cur = root;
        for (int i = 0; i < prefix.length(); i++)
        {
            char c = prefix[i];
            if (cur->nxt.find(c) == cur->nxt.end())
                return vector<string>();
            cur = cur->nxt[c];
        }
        vector<string> out;
        collect(cur, prefix, out);
        return out;
    }
};

// Stops and edges
struct Stop
{
    StopID id;
    string name;
    Point loc;
    Stop()
    {
        id = -1;
        name = "";
        loc.x = 0;
        loc.y = 0;
    }
    Stop(StopID id_, const string &name_, Point loc_)
    {
        id = id_;
        name = name_;
        loc = loc_;
    }
};
struct Edge
{
    StopID to;
    double weight;
    Edge()
    {
        to = 0;
        weight = 0;
    }
    Edge(StopID t, double w)
    {
        to = t;
        weight = w;
    }
};

struct Graph
{
    vector<Stop> stops;
    unordered_map<string, StopID> nameToId;
    vector<vector<Edge>> adj;

    Graph() {}

    StopID add_stop(const string &name, double x = 0.0, double y = 0.0)
    {
        string tn = trim(name);
        if (nameToId.count(tn))
            return nameToId[tn];
        StopID id = (StopID)stops.size();
        stops.push_back(Stop(id, tn, Point{x, y}));
        nameToId[tn] = id;
        adj.emplace_back();
        logger.log(string("Added stop: ") + tn + " (id=" + to_string(id) + ")");
        return id;
    }

    bool has_stop(const string &name) const
    {
        return nameToId.count(trim(name)) > 0;
    }

    StopID get_id(const string &name) const
    {
        auto t = nameToId.find(trim(name));
        if (t == nameToId.end())
            return -1;
        return t->second;
    }

    string get_name(StopID id) const
    {
        if (id < 0 || id >= (StopID)stops.size())
            return "";
        return stops[id].name;
    }

    Point get_loc(StopID id) const
    {
        if (id < 0 || id >= (StopID)stops.size())
            return Point{0, 0};
        return stops[id].loc;
    }

    void set_location(StopID id, double x, double y)
    {
        if (id < 0 || id >= (StopID)stops.size())
            return;
        stops[id].loc = Point{x, y};
    }

    void add_edge(const string &a, const string &b, double weight, bool bidir = true)
    {
        StopID u = add_stop(a);
        StopID v = add_stop(b);
        add_edge(u, v, weight, bidir);
    }

    void add_edge(StopID u, StopID v, double weight, bool bidir = true)
    {
        if (u < 0 || v < 0 || u >= (StopID)stops.size() || v >= (StopID)stops.size())
            return;
        adj[u].push_back(Edge(v, weight));
        if (bidir)
            adj[v].push_back(Edge(u, weight));
        logger.log(string("Added edge: ") + stops[u].name + " <-> " + stops[v].name + " (" + to_string(weight) + ")");
    }

    vector<pair<StopID, double>> neighbors(StopID u) const
    {
        vector<pair<StopID, double>> out;
        if (u < 0 || u >= (StopID)adj.size())
            return out;
        for (size_t i = 0; i < adj[u].size(); ++i)
            out.push_back(make_pair(adj[u][i].to, adj[u][i].weight));
        return out;
    }

    size_t size() const { return stops.size(); }

    // Persist graph to files (stops and edges)
    bool save_to(const string &stopsFile, const string &edgesFile) const
    {
        ofstream sf(stopsFile.c_str());
        ofstream ef(edgesFile.c_str());
        if (!sf || !ef)
            return false;
        for (size_t i = 0; i < stops.size(); ++i)
        {
            const Stop &s = stops[i];
            sf << s.id << "\t" << s.name << "\t" << s.loc.x << "\t" << s.loc.y << "\n";
        }
        for (size_t u = 0; u < adj.size(); ++u)
        {
            for (size_t j = 0; j < adj[u].size(); ++j)
            {
                const Edge &e = adj[u][j];
                ef << u << "\t" << e.to << "\t" << e.weight << "\n";
            }
        }
        sf.close();
        ef.close();
        logger.log(string("Saved graph to files: ") + stopsFile + " , " + edgesFile);
        return true;
    }

    bool load_from(const string &stopsFile, const string &edgesFile)
    {
        ifstream sf(stopsFile.c_str());
        ifstream ef(edgesFile.c_str());
        if (!sf || !ef)
            return false;
        stops.clear();
        nameToId.clear();
        adj.clear();

        string line;
        while (getline(sf, line))
        {
            if (trim(line).empty())
                continue;
            stringstream ss(line);
            int id;
            string name;
            double x, y;
            ss >> id;
            ss >> ws;

            // read remainder of line and split by tabs or spaces
            string rest;
            getline(ss, rest);

            if (rest.find('\t') != string::npos)
            {
                // Parse tab-separated format
                stringstream rr(rest);
                getline(rr, name, '\t');
                rr >> x >> y;
            }
            else
            {
                // Parse space-separated format as fallback
                stringstream rr(rest);
                rr >> name >> x >> y;
            }
            if ((int)stops.size() <= id)
                stops.resize(id + 1);
            stops[id] = Stop(id, name, Point{x, y});
            nameToId[name] = id;
            if (adj.size() <= (size_t)id)
                adj.resize(id + 1);
        }

        while (getline(ef, line))
        {
            if (trim(line).empty())
                continue;
            stringstream ss(line);
            int u, v;
            double w;
            ss >> u >> v >> w;
            if (u >= 0 && v >= 0)
            {
                if ((size_t)u >= adj.size() || (size_t)v >= adj.size())
                    continue;
                adj[u].push_back(Edge(v, w));
            }
        }
        logger.log(string("Loaded graph from files: ") + stopsFile + " , " + edgesFile);
        return true;
    }
};

// Dijkstra
pair<vector<double>, vector<int>> dijkstra(const Graph &g, StopID src)
{
    size_t n = g.size();
    const double INF = 1e18;
    vector<double> dist(n, INF);
    vector<int> prev(n, -1);
    typedef pair<double, StopID> PDI;
    priority_queue<PDI, vector<PDI>, greater<PDI>> pq;
    if (src < 0 || src >= (StopID)n)
        return make_pair(dist, prev);
    dist[src] = 0;
    pq.push(make_pair(0.0, src));
    while (!pq.empty())
    {
        PDI top = pq.top();
        pq.pop();
        double d = top.first;
        StopID u = top.second;
        if (d > dist[u])
            continue;
        vector<pair<StopID, double>> nbrs = g.neighbors(u);
        for (size_t i = 0; i < nbrs.size(); ++i)
        {
            StopID v = nbrs[i].first;
            double w = nbrs[i].second;
            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                prev[v] = (int)u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return make_pair(dist, prev);
}

// Build path from source to target using parent/predecessor array
vector<StopID> reconstruct_path(const vector<int> &prev, StopID target)
{
    vector<StopID> path;
    for (int at = (int)target; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}

// A* pathfinding result - includes success status, path, and estimated cost
struct AStarResult
{
    bool found;          // Whether a path was found
    vector<StopID> path; // Sequence of stops in the path
    double cost;         // Total travel time estimate
};
// A* pathfinding algorithm using geographic heuristic (Euclidean distance)
AStarResult astar(const Graph &g, StopID src, StopID dest, double avgSpeedKmPerHr = 40.0, double kmToMinFactor = 1.0)
{
    size_t n = g.size();
    if (src < 0 || dest < 0 || src >= (StopID)n || dest >= (StopID)n)
        return AStarResult{false, vector<StopID>(), 0.0};
    vector<double> gscore(n, 1e18), fscore(n, 1e18);
    vector<int> cameFrom(n, -1);
    // Heuristic: estimate remaining time based on straight-line distance
    auto heuristic = [&](StopID a, StopID b) -> double
    {
        Point pa = g.get_loc(a), pb = g.get_loc(b);
        double km = euclidean(pa, pb);
        double hours = (avgSpeedKmPerHr <= 0) ? 0 : (km / avgSpeedKmPerHr);
        return hours * 60.0; // Convert to minutes
    };
    typedef pair<double, StopID> P;
    priority_queue<P, vector<P>, greater<P>> openSet;
    gscore[src] = 0;
    fscore[src] = heuristic(src, dest);
    openSet.push(make_pair(fscore[src], src));
    while (!openSet.empty())
    {
        P top = openSet.top();
        openSet.pop();
        double curf = top.first;
        StopID cur = top.second;
        if (cur == dest)
        {
            vector<StopID> path = reconstruct_path(cameFrom, dest);
            return AStarResult{true, path, gscore[dest]};
        }
        vector<pair<StopID, double>> nbrs = g.neighbors(cur);
        for (size_t i = 0; i < nbrs.size(); ++i)
        {
            StopID v = nbrs[i].first;
            double w = nbrs[i].second;
            double tentative = gscore[cur] + w;
            if (tentative < gscore[v])
            {
                cameFrom[v] = (int)cur;
                gscore[v] = tentative;
                fscore[v] = tentative + heuristic(v, dest);
                openSet.push(make_pair(fscore[v], v));
            }
        }
    }
    return AStarResult{false, vector<StopID>(), 0.0};
}
// Prim's MST
pair<double, vector<pair<StopID, StopID>>> prim_mst(const Graph &g)
{
    size_t n = g.size();
    if (n == 0)
        return make_pair(0.0, vector<pair<StopID, StopID>>());
    vector<bool> inMST(n, false);
    vector<double> key(n, 1e18);
    vector<int> parent(n, -1);
    typedef pair<double, StopID> P;
    priority_queue<P, vector<P>, greater<P>> pq;
    key[0] = 0;
    pq.push(make_pair(0.0, 0));
    while (!pq.empty())
    {
        P top = pq.top();
        pq.pop();
        double k = top.first;
        StopID u = top.second;
        if (inMST[u])
            continue;
        inMST[u] = true;
        vector<pair<StopID, double>> nbrs = g.neighbors(u);
        for (size_t i = 0; i < nbrs.size(); ++i)
        {
            StopID v = nbrs[i].first;
            double w = nbrs[i].second;
            if (!inMST[v] && key[v] > w)
            {
                key[v] = w;
                parent[v] = (int)u;
                pq.push(make_pair(key[v], v));
            }
        }
    }
    double total = 0;
    vector<pair<StopID, StopID>> edges;
    for (size_t v = 1; v < n; ++v)
    {
        if (parent[v] != -1)
        {
            edges.push_back(make_pair((StopID)parent[v], (StopID)v));
            // find weight
            vector<pair<StopID, double>> nbrs = g.neighbors(parent[v]);
            for (size_t i = 0; i < nbrs.size(); ++i)
            {
                if (nbrs[i].first == (StopID)v)
                {
                    total += nbrs[i].second;
                    break;
                }
            }
        }
    }
    return make_pair(total, edges);
}

// Bus entity with route and position tracking
struct Bus
{
    string busId;
    vector<StopID> route; // Ordered sequence of stops
    int currentIndex;     // Current position in route
    double speed;         // Speed in km/h (informational)
    bool active;          // Whether bus is currently running
    Bus() : busId(""), currentIndex(0), speed(40.0), active(false) {}
    Bus(const string &id, const vector<StopID> &r, double sp = 40.0) : busId(id), route(r), currentIndex(0), speed(sp), active(true) {}
    StopID current_stop() const
    {
        if (route.empty())
            return -1;
        int idx = currentIndex;
        if (idx < 0)
            idx = 0;
        if (idx >= (int)route.size())
            idx = (int)route.size() - 1;
        return route[idx];
    }
    StopID next_stop() const
    {
        if (route.empty())
            return -1;
        if (currentIndex + 1 >= (int)route.size())
            return route.back();
        return route[currentIndex + 1];
    }
    bool move_next()
    {
        if (!active)
            return false;
        if (route.empty())
            return false;
        if (currentIndex + 1 < (int)route.size())
        {
            currentIndex++;
            return true;
        }
        else
        {
            active = false;
            return false;
        }
    }
    string info(const Graph &g) const
    {
        stringstream ss;
        ss << "[" << busId << "] ";
        ss << "At: ";
        StopID cs = current_stop();
        if (cs >= 0)
            ss << g.get_name(cs);
        else
            ss << "N/A";
        ss << " (idx " << currentIndex << "/" << (route.empty() ? 0 : route.size() - 1) << ")";
        ss << (active ? " RUNNING" : " STOPPED");
        return ss.str();
    }
    string serialize() const
    {
        stringstream ss;
        ss << busId << "|" << speed << "|" << currentIndex;
        for (size_t i = 0; i < route.size(); ++i)
            ss << "|" << route[i];
        return ss.str();
    }
    static Bus deserialize(const string &line)
    {
        vector<string> parts;
        string cur;
        stringstream ss(line);
        while (getline(ss, cur, '|'))
            parts.push_back(cur);
        Bus bus;
        if (parts.size() >= 3)
        {
            bus.busId = parts[0];
            bus.speed = atof(parts[1].c_str());
            bus.currentIndex = atoi(parts[2].c_str());
            for (size_t i = 3; i < parts.size(); ++i)
                bus.route.push_back((StopID)atoi(parts[i].c_str()));
            bus.active = true;
        }
        return bus;
    }
};