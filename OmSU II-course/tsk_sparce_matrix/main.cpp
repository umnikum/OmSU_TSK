#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "Matrix.h"
#include "SparseMatrix.h"

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
        std::map<std::string, SparseMatrix> sparseMatrixes;
        const int executeCommand(const std::string &command);
    private:
        void readMatrix(const std::string &name);
        void readSparseMatrix(const std::string &name);
        bool isSparse(const std::string &name)const;
        void readCommand(const std::vector<std::string> &args);
        void printCommand(const std::vector<std::string> &args)const;
        void countCommand(const std::vector<std::string> &args);
};


void UI::readMatrix(const std::string &name){
    std::size_t rawCount, columnCount;
    std::vector<std::vector<double>> values;
    std::cout << "Enter matrix dimentions: ";
    std::cin >> rawCount >> columnCount;

    std::cout << "Values:\n";
    for(std::size_t rawIndex=0; rawIndex<rawCount; rawIndex++){
        std::vector<double> raw;
        values.push_back(raw);
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++){
            double value;
            std::cin >> value;
            values[rawIndex].push_back(value);
        }
    }
    Matrix matrix(rawCount, columnCount, values);
    matrixes.emplace(name, matrix);
}

void UI::readSparseMatrix(const std::string &name){
    std::size_t rawCount, columnCount, elementCount;
    std::map<std::size_t, std::map<std::size_t, double>> values;
    std::cout << "Enter matrix dimentions and amount of meaningfull elements: ";
    std::cin >> rawCount >> columnCount >> elementCount;

    std::cout << "Enter triplets of raw/column indexes and value for each element:\n";
    for(std::size_t element=0; element<elementCount; element++){
        std::size_t rawIndex, columnIndex;
        double value;
        std:: cin >> rawIndex >> columnIndex >> value;
        if(values.count(rawIndex) == 0){
            std::map<std::size_t, double> raw;
            values.emplace(rawIndex, raw);
        }
        values[rawIndex][columnIndex] = value;
    }
    SparseMatrix matrix(rawCount, columnCount, values);
    sparseMatrixes.emplace(name, matrix);
}

bool UI::issparse(const std::string &name)const{
    return sparseMatrixes.count(name) > 0;
}

void UI::readCommand(const std::vector<std::string> &args){
    if(args[1].compare("matrix") == 0)
        this->readMatrix(args[2]);
    else if(args[1].compare("sparse") == 0)
        this->readsparseMatrix(args[2]);
}

void UI::printCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    if(issparse(name))
        std::cout << this->sparseMatrixes.at(name).toString();
    else if(this->matrixes.count(name) > 0)
        std::cout << this->matrixes.at(name).toString();
    else std::cout << name << " - not found\n";
}

void UI::countCommand(const std::vector<std::string> &args){
    if(args[2].compare("=") == 0){
        sparseMatrix result;
        if(issparse(args[3])){
            SparseMatrix matrix(sparseMatrixes.at(args[3]));
        }else{
            SparseMatrix matrix(matrixes.at(args[3]));
        }
        if(sparseMatrixes.count(args[1]) == 0)
            sparseMatrixes.emplace(args[1], result);
        else sparseMatrixes[args[1]] = result;
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
        }else if(commandName.compare("print") == 0){
            printCommand(args);
            return 3;
        }else if(commandName.compare("count") == 0){
            countCommand(args);
            return 4;
        }
    }
    return 1;
}

int main()
{
    UI ui;
    std::string command;
    do{
        getline(std::cin, command);
        if(ui.executeCommand(command) == 0)
            break;
    }while(true);
    return 0;
}
