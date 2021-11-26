#include <stack>
#include <vector>
#include <chrono>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <iostream>

class Graph : public std::unordered_map<int, std::unordered_set<int>> {

public:

    int v, e;

    explicit Graph(int nv=0, int ne=0) {
        v = nv;
        e = ne;
    }

    bool in_graph(int node) const {

        return (this->find(node) != this->end());
    }

};

Graph generate_graph(int v, int e, std::mt19937 &gen) {

    std::unordered_set<int> sample;

    int var, N = v*v;
    for (int r = N - e; r < N; ++r) {
        var = std::uniform_int_distribution<>(0, r)(gen);
        if (!sample.insert(var).second)
            sample.insert(r);
    }

    Graph g(v, e);

    int u, w;
    for(int n: sample) {
        u = n / v;
        w = n % v;
        if (g.in_graph(u))
            g[u].insert(w);
        else {
            std::unordered_set<int> successors({w});
            g[u] = successors;
        }
    }
    return g;
}

class SCC {

public:

    Graph *graph;

    explicit SCC(Graph &g) : low_links(g.v, -1), index(g.v, -1), on_stack(g.v, false) {
        graph = &g;
    }

    std::vector<std::vector<int>> get_strongly_connected_components() {
        int node;
        for (const auto& element: *graph) {
            node = element.first;
            if (low_links[node] == -1)
                search_component(node);
        }
        return strongly_connected_components;
    }

private:

    int cnt = 0;
    std::stack<int> stack;
    std::vector<bool> on_stack;
    std::vector<int> low_links;
    std::vector<int> index;
    std::vector<std::vector<int>> strongly_connected_components;

    void search_component(int node) {

        index[node] = cnt;
        low_links[node] = cnt;
        cnt++;
        stack.push(node);
        on_stack[node] = true;

        std::unordered_set<int> successors;
        if (graph->in_graph(node))
            successors = (*graph)[node];

        for (int successor : successors) {
            if (low_links[successor] == -1) {
                search_component(successor);
                low_links[node] = std::min(low_links[node], low_links[successor]);
            }
            else if (on_stack[successor])
                low_links[node] = std::min(low_links[node], index[successor]);
        }

        if (low_links[node] == index[node]) {

            std::vector<int> connected_component;
            int successor;
            while (true) {
                successor = stack.top();
                stack.pop();
                on_stack[successor] = false;
                connected_component.push_back(successor);
                if (successor == node)
                    break;
            }
            strongly_connected_components.push_back(connected_component);
        }
    }
};

int main() {

    int NUMBER_OF_TESTS, MAX_VERTICES;

    std::cout << "Input number of tests: ";
    std::cin >> NUMBER_OF_TESTS;
    std::cout << "\nInput maximum number of vertices: ";
    std::cin >> MAX_VERTICES;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::ofstream out;
    out.open("../data.txt");

    int v, e;
    long long time;

    for (int k = 0; k < NUMBER_OF_TESTS; ++k) {
        v = std::uniform_int_distribution<>(2, MAX_VERTICES)(gen);
        e = std::uniform_int_distribution<>(v, v*(v-1))(gen);
        Graph G = generate_graph(v, e, gen);
        auto start = std::chrono::steady_clock::now();
        std::vector<std::vector<int>> scc = SCC(G).get_strongly_connected_components();
        auto end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        out << G.v+G.e << " " << time << "\n";
    }
    out.close();

    return 0;
}
