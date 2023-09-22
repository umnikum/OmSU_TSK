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
        const SparseMatrix get(const std::string &name)const;
        void readMatrix(const std::string &name);
        void readSparseMatrix(const std::string &name);
        bool isSparse(const std::string &name)const;
        void readCommand(const std::vector<std::string> &args);
        void printCommand(const std::vector<std::string> &args)const;
        const SparseMatrix calculate(const std::string &matrixA, const std::string &matrixB, const std::string &operation);
        void countCommand(const std::vector<std::string> &args);
};

const SparseMatrix UI::get(const std::string &name)const{
    SparseMatrix result;
    size_t suffix = name.rfind("^T");
    std::string argName;
    bool needTransp = false;
    if(suffix != std::string::npos){
        argName = argName.substr(0, suffix);
        needTransp = true;
    }
    if(isSparse(argName)){
        result = sparseMatrixes.at(name);
    }else{
        SparseMatrix matrix(matrixes.at(name));
        result = matrix;
    }
    if(needTransp)
        result = result.T();
    return result;
}

void UI::readMatrix(const std::string &name){
    std::size_t rowCount, columnCount;
    std::vector<std::vector<double>> values;
    std::cout << "Enter matrix dimentions: ";
    std::cin >> rowCount >> columnCount;

    std::cout << "Values:\n";
    for(std::size_t rowIndex=0; rowIndex<rowCount; rowIndex++){
        std::vector<double> row;
        values.push_back(row);
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++){
            double value;
            std::cin >> value;
            values[rowIndex].push_back(value);
        }
    }
    Matrix matrix(values);
    matrixes.emplace(name, matrix);
}

void UI::readSparseMatrix(const std::string &name){
    std::size_t rowCount, columnCount, elementCount;
    std::map<std::size_t, std::map<std::size_t, double>> values;
    std::cout << "Enter matrix dimentions and amount of meaningfull elements: ";
    std::cin >> rowCount >> columnCount >> elementCount;

    std::cout << "Enter triplets of row/column indexes and value for each element:\n";
    for(std::size_t element=0; element<elementCount; element++){
        std::size_t rowIndex, columnIndex;
        double value;
        std:: cin >> rowIndex >> columnIndex >> value;
        if(values.count(rowIndex) == 0){
            std::map<std::size_t, double> row;
            values.emplace(rowIndex, row);
        }
        values[rowIndex][columnIndex] = value;
    }
    MatrixSize size(rowCount, columnCount);
    SparseMatrix matrix(size, values);
    sparseMatrixes.emplace(name, matrix);
}

bool UI::isSparse(const std::string &name)const{
    return sparseMatrixes.count(name) > 0;
}

void UI::readCommand(const std::vector<std::string> &args){
    if(args[1].compare("matrix") == 0)
        this->readMatrix(args[2]);
    else if(args[1].compare("sparse") == 0)
        this->readSparseMatrix(args[2]);
}

void UI::printCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    if(isSparse(name))
        std::cout << this->sparseMatrixes.at(name).toString();
    else if(this->matrixes.count(name) > 0)
        std::cout << this->matrixes.at(name).toString();
    else std::cout << name << " - not found\n";
}

const SparseMatrix UI::calculate(const std::string &matrixA, const std::string &matrixB, const std::string &operation){
    if(operation.compare("+") == 0)
        return get(matrixA) + get(matrixB);
    if(operation.compare("-") == 0)
        return get(matrixA) - get(matrixB);
    if(operation.compare("*") == 0)
        return get(matrixA) * get(matrixB);
    return get(matrixA);
}

void UI::countCommand(const std::vector<std::string> &args){
    std::string operation = args[2];
    if(operation.compare("=") == 0){
        SparseMatrix result;
        std::string name = args[1];
        
        if(args.size() == 3){
            std::string argName = args[3];
            result = get(argName);
        }else{
            result = calculate(args[3], args[5], args[4]);
        }
        
        if(sparseMatrixes.count(name) == 0)
            sparseMatrixes.emplace(name, result);
        else sparseMatrixes[name] = result;
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
