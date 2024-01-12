#ifndef SURFACE_H_INCLUDED
#define SURFACE_H_INCLUDED

#include <vector>
#include <map>
#include <cassert>
#include <string>
#include "Node.h"

class Lattice{
    protected:
		std::vector<short> values;
    public:
		size_t size;
        Lattice(const size_t &linearSize=0, const short &defaultValue=1):
        		values(linearSize*linearSize, defaultValue), size{linearSize}{}
        void reset(const short &default_value=1){
        	for(short &value:values)
        		value=default_value;
        }
        const short& operator()(const Node &node)const{
        	return values.at(node.y*size + node.x);}
        short& operator[](const Node &node){
            return values[node.y*size + node.x];}
        std::vector<short>::iterator operator[](const size_t &index){
        	assert(index<size);
        	return std::next(values.begin(), size*index);
        }
        std::vector<short>::const_iterator operator[](const size_t &index)const{
        	assert(index<size);
        	return std::next(values.cbegin(), size*index);
        }
        Node top(const size_t &row, const size_t &column)const{
        	return Node{column, ((row>0)? row-1: size-1)};}
        Node top(const Node &node)const{
            return Node{node.x, ((node.y>0)? node.y-1: size-1)};}

        Node bottom(const size_t &row, const size_t &column)const{
            return Node{column, ((row<size-1)? row+1: 0)};}
        Node bottom(const Node &node)const{
            return Node{node.x, ((node.y<size-1)? node.y+1: 0)};}

        Node left(const size_t &row, const size_t &column)const{
            return Node{((column>0)? column-1: size-1), row};}
        Node left(const Node &node)const{
            return Node{((node.x>0)? node.x-1: size-1), node.y};}

        Node right(const size_t &row, const size_t &column)const{
            return Node{((column<size-1)? column+1: 0), row};}
        Node right(const Node &node)const{
            return Node{((node.x<size-1)? node.x+1: 0), node.y};}

        short calculate_neighbors(const Node &node)const{
        	return this->operator()(top(node))
        			+this->operator()(bottom(node))
        			+this->operator()(right(node))
        			+this->operator()(left(node));
        }
        void get_data(std::map<std::string, double> &data)const{
        	double M{0.0}, E{0.0};
        	Node node{};
        	for(node.y=0; node.y<size; ++node.y)
				for(node.x=0; node.x<size; ++node.x){
					const short &value{this->operator()(node)};
					M+=value;
					E+=value*calculate_neighbors(node);
				}
        	M /= size*size;
        	E /= size*size;
        	data["M"] = M;
        	data["E"] = E;
        }
};

class LatticeDataCollection{
protected:
	std::vector<size_t> amounts{};
	std::vector<std::map<std::string, double>> data{};
public:
	void add(const size_t &index, const std::map<std::string, double> &lattice_data){
		if(index == amounts.size()){
			amounts.push_back(0);
			std::map<std::string, double> next_entry;
			next_entry.emplace("M", 0.0);
			next_entry.emplace("E", 0.0);
			next_entry.emplace("M^2", 0.0);
			next_entry.emplace("E^2", 0.0);
			data.push_back(next_entry);
		}
		for(auto pair:lattice_data)
			data[index][pair.first] += pair.second;
		const double &M{lattice_data.at("M")}, &E{lattice_data.at("E")};
		data[index]["M^2"] += M * M;
		data[index]["E^2"] += E * E;
		++amounts[index];
	}
	void save(const std::string &path)const{
		auto sample = data[0];
		for(auto pair:sample){
			std::stringstream ss;
			ss << path << "/" << pair.first << ".dat";
			std::ofstream ofs{ss.str()};
			for(size_t index=0; index<data.size(); ++index)
				ofs << data.at(index).at(pair.first)/amounts.at(index) << "\n";
		}
	}
};

#endif // SURFACE_H_INCLUDED
