#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distance(n, INF);
    vector<bool> visited(n, false);
    previous.assign(n, -1);
    
    struct Node {
        int vertex;
        int weight;
        
        Node(int v, int w) : vertex(v), weight(w) {}
        
        bool operator>(const Node& other) const {
            return weight > other.weight;
        }
    };
    
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push(Node(source, 0));
    distance[source] = 0;
    
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push(Node(v, distance[v]));
            }
        }
    }
    
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    
    if (destination < 0 || destination >= distances.size() || distances[destination] == INF) {
        return path;
    }
    
    stack<int> temp_path;
    for (int v = destination; v != -1 && v < distances.size(); v = previous[v]) {
        temp_path.push(v);
    }
    
    while (!temp_path.empty()) {
        path.push_back(temp_path.top());
        temp_path.pop();
    }
    
    return path;
}

void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << endl << "Total cost is " << total << endl;
        return;
    }
    
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        cout << " ";
    }
    cout << endl;
    cout << "Total cost is " << total << endl;
}