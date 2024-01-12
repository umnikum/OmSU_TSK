#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>

class Node{
public:
	size_t x, y;
	Node(const size_t &x=0, const size_t &y=0):x{x}, y{y}{}
	Node(const Node &node):x{node.x}, y{node.y}{}
	Node& operator=(const Node &node){
		x=node.x; y=node.y;
		return *this;
	}
	std::string str()const{
		std::stringstream ss;
		ss << "(" << x << ", " << y << ")";
		return ss.str();
	}
};

#endif /* NODE_H_ */
