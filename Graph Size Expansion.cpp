#include <iostream>

#include "Node.h"
#include <vector>
#include <cmath>
static int totalGraphs = 0;

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
    int size = nodes.size();
    bool* checked = new bool[size];
    for (int i = 0; i < size; i++) {
        checked[i] = false;
    }
    std::cout << "Forest:\n";
    while (head < size) {
        if (!checked[head]) {
            std::vector<int> tree = getConnections(checked, nodes[head]);
            std::cout << "Connected Subgraph: ";
            for (int n : tree) {
                std::cout << n << " ";
            }
            std::cout << "\n";
        }
        head++;
    }
    
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
        showConnections(nodes);
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
                } else {
                    nodes[index]->connectEdge(connectingNodes[j]);
                }
            }
        }
        permutateGraphs(nodes, index + 1);
    }
}

int main() {
    std::vector<Node*> nodes;
    int upto = 5;
    for (int current = 0; current < upto; current++) {
        Node* newNode = new Node(current);
        nodes.push_back(newNode);
        
    }
    permutateGraphs(nodes, 0);
}
