#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distance(n, INF);
    vector<bool> visited(n, false);
    previous.resize(n, -1);
    
    distance[source] = 0;
    
    for (int i = 0; i < n; i++) {
        int u = -1;
        int min_dist = INF;
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && distance[v] < min_dist) {
                min_dist = distance[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        
        visited[u] = true;
        
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
            }
        }
    }
    
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    
    if (destination < 0 || destination >= distances.size() || distances[destination] == INF) 
        return path;
    
    int current = destination;
    while (current != -1) {
        path.push_back(current);
        current = previous[current];
    }
    
    reverse(path.begin(), path.end());
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