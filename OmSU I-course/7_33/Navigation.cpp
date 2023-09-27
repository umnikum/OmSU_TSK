#include <sstream>
#include "Navigation.h"

Connection::Connection(const int &nodeA, const int &nodeB, const double &length){
	this->nodeA = nodeA;
	this->nodeB = nodeB;
	this->length = length;
}

Connection::Connection(const Connection &connection){
	this->nodeA = connection.nodeA;
	this->nodeB = connection.nodeB;
	this->length = connection.length;
}

bool Connection::isConnected(const int &nodeIndex)const{return (nodeA==nodeIndex)||(nodeB==nodeIndex);}

const std::string Connection::toString()const{
	std::stringstream ss;
	ss << nodeA << " <-(" << length << ")-> " << nodeB;
	return ss.str();
}

NavigationNode::NavigationNode(const int &index){
	this->index = index;
	connections = std::map<int, Connection>{};
}

NavigationNode::NavigationNode(const NavigationNode &node){
	this->index = node.index;
	connections = std::map<int, Connection>(node.connections);
}

bool NavigationNode::hasConnection(const NavigationNode &node)const{return connections.count(node.index) > 0;}

const std::string NavigationNode::toString()const{
	std::stringstream ss;
	ss << index << " |--> ";
	for(std::pair<int, Connection> pair:connections)
		ss << pair.first << ',';
	ss.seekp(-1, std::ios_base::end);
	ss << ' ';
	return ss.str();
}

Route::Route(){this->nodes = std::list<NavigationNode>{};}
Route::Route(const NavigationNode &start){
	this->nodes = std::list<NavigationNode>{};
	this->nodes.push_back(start);
}

bool Route::isEmpty()const{return !(nodes.size()>0);}
bool Route::wasVisited(const NavigationNode &node)const{
	for(NavigationNode candidate:this->nodes)
		if(candidate.index == node.index)
			return true;
	return false;
}

void Route::visit(const NavigationNode &node){this->nodes.push_back(node);}
void Route::return_(){this->nodes.pop_back();}

const double Route::length()const{
	double length = 0.0;
	int prevIndex = (*this->nodes.begin()).index;
	for(const auto node:this->nodes){
		if(node.index != prevIndex)
			length += node.connections.at(prevIndex).length;
		prevIndex = node.index;
	}
	return length;
}

bool Route::compare(const Route &betterRoute)const{return length() > betterRoute.length();}

const std::string Route::toString()const{
	std::stringstream ss;
	int prevIndex = (*this->nodes.begin()).index;
	ss << prevIndex;
	for(const auto node:this->nodes){
		if(node.index != prevIndex)
			ss << " -(" << node.connections.at(prevIndex).length << ")-> " << node.index;
		prevIndex = node.index;
	}
	return ss.str();
}

NavigationMap::NavigationMap(const int &count){
	this->nodes = std::map<int, NavigationNode>{};
	for(int i=0; i<count; ++i)
		nodes.emplace(i, NavigationNode(i));
}

void NavigationMap::setConnection(const Connection &connection){
	nodes.at(connection.nodeA).connections.emplace(connection.nodeB, connection);
	nodes.at(connection.nodeB).connections.emplace(connection.nodeA, connection);
}

const Route NavigationMap::searchAllConnections(const NavigationNode &finishNode, Route &route, const int &depth)const{
	NavigationNode &lastNode = route.nodes.back();
	Route minRoute{};
	if(lastNode.index == finishNode.index){
		minRoute.nodes.push_front(lastNode);
	}else if(depth > 0){
		for(auto pair:lastNode.connections){
			const NavigationNode &currentNode = this->nodes.at(pair.first);
			if(!route.wasVisited(currentNode)){
				route.visit(currentNode);
				Route search = searchAllConnections(finishNode, route, depth-1);
				if(!search.isEmpty()){
					search.nodes.push_front(lastNode);
					if(((!minRoute.isEmpty())&&(minRoute.compare(search)))||(minRoute.isEmpty())){
						minRoute = search;
					}
				}
				route.return_();
			}
		}
	}
	return minRoute;
}

const Route NavigationMap::operator()(const int &nodeA, const int &nodeB, const int &depth)const{
	Route route(this->nodes.at(nodeA));
	return searchAllConnections(this->nodes.at(nodeB), route, depth);
}

const std::string NavigationMap::toString()const{
	std::stringstream ss;
	for(std::pair<int, NavigationNode> pair:this->nodes)
		ss << pair.second.toString() << std::endl;
	return ss.str();
}
