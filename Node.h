#pragma once
#include <map>
#include <string>
class Node
{
public:
	int id;
	std::map<int, Node*> connections;
	void connectEdge(Node* other);
	void disconnectEdge(Node* other);
	bool isConnected(Node* other);
	std::string stringConnections();
	Node(int _id);
};

