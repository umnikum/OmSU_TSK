#ifndef STACK_H_
#define STACK_H_

#include <iostream>
#include <cassert>

template<class ValueType>
class Stack{
protected:
	size_t capasity, size{0};
	ValueType *nodes{nullptr};
public:
	Stack(const size_t &capasity=0):capasity{capasity}{
		nodes = new ValueType[capasity];}
	~Stack(){delete [] nodes;}
	bool empty()const{return size==0;}
	void clear(){size=0;}

	void push(const ValueType &node){
		assert(++size < capasity);
		nodes[size-1]=node;
	}

	ValueType& pop(){
		assert(size > 0);
		return nodes[size--];
	}
};

#endif /* STACK_H_ */
