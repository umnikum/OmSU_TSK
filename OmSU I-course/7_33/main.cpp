#include <iostream>
#include <vector>
#include <algorithm>
#include "Navigation.h"

std::vector<std::string> split(const std::string &line){
    std::vector<std::string> args;
    auto previousIt = line.begin();
    auto nextIt = previousIt;
    do{
        nextIt = std::find(previousIt, line.end(), ' ');
        std::string arg = line.substr(previousIt-line.begin(), nextIt-previousIt);
        if(!arg.empty())
            args.push_back(arg);
        previousIt = nextIt+1;
    }while(nextIt != line.end());
    return args;
}

class UI{
    public:
        NavigationMap map{0};
        const int executeCommand(const std::string &command);
    private:
        void readCommand(const std::vector<std::string> &args);
        void searchCommand(const std::vector<std::string> &args)const;
};

void UI::readCommand(const std::vector<std::string> &args){
    int rowCount, columnCount;
    std::vector<std::vector<double>> values;
    std::cout << "Enter matrix dimensions: ";
    std::cin >> rowCount >> columnCount;

    std::cout << "Values:\n";
    for(int rowIndex=0; rowIndex<rowCount; ++rowIndex){
        values.push_back(std::vector<double>{});
        for(int columnIndex=0; columnIndex<columnCount; ++columnIndex){
            double value;
            std::cin >> value;
            values[rowIndex].push_back(value);
        }
    }
    this->map = NavigationMap{rowCount};
    for(int rowIndex=0; rowIndex<rowCount; ++rowIndex)
    	for(int columnIndex=rowIndex+1; columnIndex<columnCount; ++columnIndex)
    		if(values[rowIndex][columnIndex] != 0.0)
    			this->map.setConnection(Connection(rowIndex, columnIndex, values[rowIndex][columnIndex]));
}

void UI::searchCommand(const std::vector<std::string> &args)const{
	const int size = this->map.nodes.size();
	for(int nodeA=0; nodeA<size; ++nodeA)
		for(int nodeB=nodeA+1; nodeB<size; ++nodeB){
			const Route route = map(nodeA, nodeB, 2);
			if(route.length() == 2.0)
				std::cout << route.toString() << std::endl;
		}
}

const int UI::executeCommand(const std::string &command){
    std::vector<std::string> args = split(command);
    if(!args.empty()){
        std::string commandName = args[0];
        if(commandName.compare("exit") == 0)
            return 0;
        else if(commandName.compare("read") == 0){
            readCommand(args);
            return 2;
        }else if(commandName.compare("search") == 0){
            searchCommand(args);
            return 3;
        }
    }
    return 1;
}

int main(){
    UI ui{};
    std::string command;
    do{
        getline(std::cin, command);
        if(ui.executeCommand(command) == 0)
            break;
    }while(true);
    return 0;
}

