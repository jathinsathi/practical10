#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    int src, dest, weight;
    
    // Comparator operator to sort edges by weight
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set (Union-Find) structure for cycle detection
struct DisjointSet {
    vector<int> parent, rank;

    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        // Initially, every node is its own parent
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Find the representative/ultimate parent of the set containing u (with path compression)
    int findParent(int u) {
        if (u == parent[u])
            return u;
        return parent[u] = findParent(parent[u]); // Path compression
    }

    // Union two sets by rank
    void unionByRank(int u, int v) {
        int rootU = findParent(u);
        int rootV = findParent(v);

        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

// Function to calculate MST using Kruskal's algorithm
void kruskalsMST(int V, vector<Edge>& edges) {
    // Step 1: Sort all the edges in non-decreasing order of their weight
    sort(edges.begin(), edges.end());

    DisjointSet ds(V);
    vector<Edge> mst; // Stores the final MST edges
    int mstWeight = 0;

    // Step 2: Iterate through all sorted edges
    for (const auto& edge : edges) {
        int rootSrc = ds.findParent(edge.src);
        int rootDest = ds.findParent(edge.dest);

        // If including this edge doesn't cause a cycle, add it to the MST
        if (rootSrc != rootDest) {
            mst.push_back(edge);
            mstWeight += edge.weight;
            ds.unionByRank(rootSrc, rootDest);
        }

        // Optimization: Stop if we have gathered V-1 edges
        if (mst.size() == V - 1) break;
    }

    // Print the results
    cout << "Edges in the Minimum Spanning Tree:\n";
    for (const auto& edge : mst) {
        cout << edge.src << " -- " << edge.dest << " == " << edge.weight << "\n";
    }
    cout << "Total weight of MST: " << mstWeight << "\n";
}

int main() {
    int V = 4; // Number of vertices
    int E = 5; // Number of edges
    vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    kruskalsMST(V, edges);

    return 0;
}
