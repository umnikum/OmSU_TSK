#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <list>
#include <map>
#include <string>

class Connection{
public:
	int nodeA;
	int nodeB;
	double length;
	Connection(const int &nodeA, const int &nodeB, const double &length=1.0);
	Connection(const Connection &connection);
	bool isConnected(const int &nodeIndex)const;
	const std::string toString()const;
};

class NavigationNode{
public:
	int index;
	std::map<int, Connection> connections;
	NavigationNode(const int &index);
	NavigationNode(const NavigationNode &node);
	bool hasConnection(const NavigationNode &node)const;
	const std::string toString()const;
};

class Route{
public:
	std::list<NavigationNode> nodes;
	Route();
	Route(const NavigationNode &start);
	bool isEmpty()const;
	bool wasVisited(const NavigationNode &node)const;
	void visit(const NavigationNode &node);
	void return_();
	const double length()const;
	bool compare(const Route &betterRoute)const;
	const std::string toString()const;
};

class NavigationMap{
private:
	const Route searchAllConnections(const NavigationNode &finishNode, Route &route, const int &depth)const;
public:
	std::map<int, NavigationNode> nodes;
	NavigationMap(const int &count);
	void setConnection(const Connection &connection);
	const Route operator()(const int &nodeA, const int &nodeB, const int &depth=1)const;
	const std::string toString()const;
};

#endif /* NAVIGATION_H_ */
