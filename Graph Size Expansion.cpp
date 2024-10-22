// Graph Size Expansion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Node.h"
#include <vector>
#include <cmath>
#include <fstream>

static int totalGraphs = 0;
static int connections = 0;
static int current = 0;
static int subgraphs = 0;
//number of nodes, connections, list of number of subgraphs for a connection
static std::map<int, std::map<int, std::vector<int>>> record;
static std::ofstream* outputFile;

void writeOutRecord() {
    std::ofstream out("record.csv");
    out << "number of nodes,connections,number of subgraphs\n";
    for (std::pair<int, std::map<int, std::vector<int>>> i : record) {
        for (std::pair<int, std::vector<int>> j : i.second) {
            for (int k : j.second) {
                out << i.first << "," << j.first << "," << k << "\n";
            }
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
    *outputFile << current << "," << connections << "," << subgraphs << "\n";
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
   // std::cout << "Forest:\n";
    while (head < size) {
        if (!checked[head]) {
            std::vector<int> tree = getConnections(checked, nodes[head]);
            //std::cout << "Connected Subgraph: ";
            //for (int n : tree) {
            //    std::cout << n << " ";
            //}
            //std::cout << "\n";
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
        //showConnections(nodes);
        //saveConfigurationToStats();
        outputStatPoint();
        return;
    }
    int headPermutationSize = pow(2, nodes.size() - 1 - index);
    //std::cout << "headPermutationSize " << headPermutationSize << "\n";
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
    std::ofstream out("record.csv");
    out << "number of nodes,connections,number of subgraphs\n";
    outputFile = &out;
    std::vector<Node*> nodes;
    int upto = 8;
    for (current = 0; current < upto; current++) {
        Node* newNode = new Node(current);
        nodes.push_back(newNode);
        std::map<int, std::vector<int>> emptyMap;
        record[current] = emptyMap;
    }
    permutateGraphs(nodes, 0);
    //writeOutRecord();
    out.close();
    time(&timestamp);
    std::cout << "end: " << ctime(&timestamp) << "\n";
}
