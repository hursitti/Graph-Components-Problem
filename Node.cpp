#include "Node.h"

void Node::connectEdge(Node* other) {
	//edgeMap[id][other->id] = true;
	//edgeMap[other->id][id] = true;
	connections[other->id] = other;
	other->connections[id] = this;
}
void Node::disconnectEdge(Node* other) {
	//edgeMap[id][other->id] = false;
	//edgeMap[other->id][id] = false;
	connections.erase(other->id);
	other->connections.erase(id);
}
bool Node::isConnected(Node* other) {
	return connections.find(other->id) != connections.end();
}
std::string Node::stringConnections() {
	std::string ret = std::to_string(id) + " is connected to";
	for (std::pair<int, Node*> p : connections) {
		ret = ret + " " + std::to_string(p.first);
	}
	return ret;
}
Node::Node(int _id) {
	id = _id;
	//edgeMap = _edgeMap;
}