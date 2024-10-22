#include <iostream>

#include "Node.h"
#include <vector>
#include <cmath>
#include <fstream>

static int totalGraphs = 0;
static int connections = 0;
static int current = 0;
static int subgraphs = 0;

static int upto = 7;
static int maxConnections;

//number of nodes, connections, list of number of subgraphs for a connection
static std::map<int, std::map<int, std::vector<int>>> record;
//[subgraphs][connections]
static std::vector<std::vector<int>> table;
static std::ofstream* outputFile;

void setupTable() {
    maxConnections = (upto * (upto - 1) / 2) + 1;
    for (int i = 0; i < upto; i++) {
        table.push_back(std::vector<int>(maxConnections));
        for (int j = 0; j < maxConnections; j++) {
            table[i][j] = 0;
        }
    }
}

void writeOutRecord() {
    std::ofstream out("record.csv");
    out << "connections";
    for (int i = 0; i < upto; i++) {
        out << "," << (i + 1) << " component(s)";
    }
    for (int j = 0; j < maxConnections; j++) {
        out << "\n" << j;
        for (int i = 0; i < upto; i++) {
            out << "," << table[i][j];
        }
    }
    out.close();
}

void saveConfigurationToStats() {
    if (record[current].find(connections) == record[current].end()) {
        std::vector<int> emptyList;
        record[current][connections] = emptyList;
    }
    record[current][connections].push_back(subgraphs);
}
void outputStatPoint() {
    table[subgraphs - 1][connections]++;
}

std::vector<int> getConnections(bool* checked, Node* head) {
    std::vector<int> ret;
    ret.push_back(head->id);
    checked[head->id] = true;
    for (std::pair<int, Node*> neighbor : head->connections) {
        if (!checked[neighbor.first]) {
            std::vector<int> app = getConnections(checked, neighbor.second);
            ret.insert(ret.end(), app.begin(), app.end());
        }
    }
    return ret;
}

void dfsDisplay(std::vector<Node*> nodes) {
    int head = 0;
    subgraphs = 0;
    int size = nodes.size();
    bool* checked = new bool[size];
    for (int i = 0; i < size; i++) {
        checked[i] = false;
    }
    while (head < size) {
        if (!checked[head]) {
            std::vector<int> tree = getConnections(checked, nodes[head]);
            subgraphs++;
        }
        head++;
    }
    delete[]checked;
}

void showConnections(std::vector<Node*> nodes) {
    std::cout << "Connections:\n";
    for (Node* n : nodes) {
        std::cout << n->stringConnections() << "\n";
    }
}

void permutateGraphs(std::vector<Node*> nodes, int index) {
    if (index + 1 == nodes.size()) {
        totalGraphs++;
        dfsDisplay(nodes);
        outputStatPoint();
        return;
    }
    int headPermutationSize = pow(2, nodes.size() - 1 - index);
    std::vector<Node*> connectingNodes(nodes.begin() + index + 1, nodes.end());
    std::vector<int> toggleOnCount;
    int exponent = 1;
    for (int power = 1; power < nodes.size() - index; power++) {
        toggleOnCount.push_back(exponent);
        exponent *= 2;
    }
    for (int i = 1; i <= headPermutationSize; i++) {
        for (int j = 0; j < toggleOnCount.size(); j++) {
            if (i % toggleOnCount[j] == 0) {
                if (nodes[index]->isConnected(connectingNodes[j])) {
                    nodes[index]->disconnectEdge(connectingNodes[j]);
                    connections--;
                } else {
                    nodes[index]->connectEdge(connectingNodes[j]);
                    connections++;
                }
            }
        }
        permutateGraphs(nodes, index + 1);
    }
}

int main() {
    time_t timestamp;
    time(&timestamp);
    std::cout << "start: " << ctime(&timestamp) << "\n";
    std::vector<Node*> nodes;
    for (current = 0; current < upto; current++) {
        Node* newNode = new Node(current);
        nodes.push_back(newNode);
        std::map<int, std::vector<int>> emptyMap;
        record[current] = emptyMap;
    }
    setupTable();
    permutateGraphs(nodes, 0);
    writeOutRecord();
    time(&timestamp);
    std::cout << "end: " << ctime(&timestamp) << "\n";
}
