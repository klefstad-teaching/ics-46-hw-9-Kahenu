#include "dijkstras.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <graph_file>" << endl;
        return 1;
    }
    
    string filename = argv[1];
    Graph G;
    
    try {
        file_to_graph(filename, G);
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    
    int source = 0;
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    
    for (int dest = 0; dest < G.numVertices; ++dest) {
        if (distances[dest] != INF) {
            vector<int> path = extract_shortest_path(distances, previous, dest);
            print_path(path, distances[dest]);
        } else {
            cout << "No path from " << source << " to " << dest << endl;
        }
    }
    
    return 0;
}