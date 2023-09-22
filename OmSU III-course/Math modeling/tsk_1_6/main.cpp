#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "Matrix.h"

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
        std::map<std::string, Matrix> matrixes;
        const int executeCommand(const std::string &command);
    private:
        const Matrix get(const std::string &name)const;
        const Matrix calculate(const std::string &matrixA, const std::string &matrixB, const std::string &operation)const;
        void readMatrix(const std::string &name);
        void readCommand(const std::vector<std::string> &args);
        void printCommand(const std::vector<std::string> &args)const;
        void countCommand(const std::vector<std::string> &args);
        void minorCommand(const std::vector<std::string> &args)const;
        void detCommand(const std::vector<std::string> &args)const;
        void inverseCommand(const std::vector<std::string> &args)const;
};

const Matrix UI::get(const std::string &name)const{
	size_t suffix = name.rfind("^");
	if(suffix != std::string::npos){
		std::string argName = name.substr(0, suffix);
	    if(this->matrixes.count(argName) > 0){
	    	Matrix matrix = this->matrixes.at(argName);
	    	if(name.substr(suffix+1, 1).compare("T") == 0)
	    		return matrix.T();
	    	else if(name.substr(suffix+1, 2).compare("-1") == 0)
	    		return matrix.inverse();
	    }
	}else{
		if(this->matrixes.count(name) > 0)
			return this->matrixes.at(name);
	}
	return Matrix();
}

void UI::readMatrix(const std::string &name){
    std::size_t rowCount, columnCount;
    std::vector<std::vector<double>> values;
    std::cout << "Enter matrix dimensions: ";
    std::cin >> rowCount >> columnCount;

    std::cout << "Values:\n";
    for(std::size_t rowIndex=0; rowIndex<rowCount; rowIndex++){
        values.push_back(std::vector<double>());
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++){
            double value;
            std::cin >> value;
            values[rowIndex].push_back(value);
        }
    }
    matrixes.emplace(name, Matrix(values));
}

void UI::readCommand(const std::vector<std::string> &args){
     readMatrix(args[1]);
}

void UI::printCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    if(this->matrixes.count(name) > 0)
        std::cout << get(name).toString();
    else std::cout << name << " - not found\n";
}

const Matrix UI::calculate(const std::string &matrixA, const std::string &matrixB, const std::string &operation)const{
    if(operation.compare("+") == 0)
        return get(matrixA) + get(matrixB);
    if(operation.compare("-") == 0)
        return get(matrixA) - get(matrixB);
    if(operation.compare("*") == 0)
        return get(matrixA) * get(matrixB);
    return matrixes.at(matrixA);
}

void UI::countCommand(const std::vector<std::string> &args){
    std::string operation = args[2];
    if(operation.compare("=") == 0){
        Matrix result;
        std::string name = args[1];

        if(args.size() == 4){
            std::string argName = args[3];
            result = get(argName);
        }else{
            result = calculate(args[3], args[5], args[4]);
        }

        if(matrixes.count(name) == 0)
            matrixes.emplace(name, result);
        else matrixes[name] = result;
    }
}

void UI::minorCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    size_t x, y;
    std::stringstream(args[2]) >> x;
    std::stringstream(args[3]) >> y;
    if(matrixes.count(name) == 0)
        std::cout << name << " - not found\n";
    else std::cout << get(name).algebraicMinor(x, y).toString();
}

void UI::detCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    if(matrixes.count(name) == 0)
        std::cout << name << " - not found\n";
    else std::cout << get(name).det() << std::endl;
}

void UI::inverseCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    if(matrixes.count(name) == 0)
        std::cout << name << " - not found\n";
    else std::cout << get(name).inverse().toString();
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
        }else if(commandName.compare("print") == 0){
            printCommand(args);
            return 3;
        }else if(commandName.compare("count") == 0){
            countCommand(args);
            return 4;
        }else if(commandName.compare("minor") == 0){
            minorCommand(args);
            return 5;
        }else if(commandName.compare("det") == 0){
            detCommand(args);
            return 6;
        }else if(commandName.compare("inverse") == 0){
            inverseCommand(args);
            return 7;
        }
    }
    return 1;
}

int main(){
    UI ui;
    std::string command;
    do{
        getline(std::cin, command);
        if(ui.executeCommand(command) == 0)
            break;
    }while(true);
    return 0;
}
