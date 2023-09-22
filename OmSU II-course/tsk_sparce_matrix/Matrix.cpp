#include <iostream>
#include <string>
#include <sstream>
#include <initializer_list>
#include <vector>
#include <map>
#include "Matrix.h"


Matrix::Matrix(const std::size_t &rawCount, const std::size_t &columnCount, std::vector<std::vector<double>> &values){
    this->rawCount = rawCount;
    this->columnCount = columnCount;
    this->values = values;
}

Matrix::Matrix(const std::size_t &rawCount, const std::size_t &columnCount, double defaultValue){
    this->rawCount = rawCount;
    this->columnCount = columnCount;
    for(std::size_t rawIndex=0; rawIndex<rawCount; rawIndex++){
        std::vector<double> raw(columnCount);
        values.push_back(raw);
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++)
            raw.push_back(defaultValue);
    }
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> &values){
    for(auto raw:values){
        this->rawCount++;
        std::vector<double> newRaw = raw;
        this->values.push_back(newRaw);
    }
    this->columnCount = this->values[0].size();
}

Matrix::Matrix(const Matrix& matrix){
    this->rawCount = matrix.rawCount;
    this->columnCount = matrix.columnCount;
    std::vector<std::vector<double>> newValues(matrix.values);
    this->values = newValues;
}

void Matrix::operator=(const Matrix& matrix){
    this->rawCount = matrix.rawCount;
    this->columnCount = matrix.columnCount;
    this->values = matrix.values;
}

const Matrix Matrix::T()const{
    std::vector<std::vector<double>> values;

    for(std::size_t rawIndex=0; rawIndex<columnCount; rawIndex++){
        std::vector<double> raw(this->rawCount);
        values.push_back(raw);
        for(std::size_t columnIndex=0; columnIndex<rawCount; columnIndex++)
            raw.push_back(this->values[columnIndex][rawIndex]);
    }

    Matrix result(this->columnCount, this->rawCount, values);
    return result;
}

const Matrix Matrix::operator+(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->rawCount == matrix.rawCount)&&(this->columnCount == matrix.columnCount))
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
            for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
                values[rawIndex][columnIndex] += matrix.values[rawIndex][columnIndex];

    Matrix result(this->rawCount, this->columnCount, values);
    return result;
}

const Matrix Matrix::operator-(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->rawCount == matrix.rawCount)&&(this->columnCount == matrix.columnCount))
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
            for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
                values[rawIndex][columnIndex] -= matrix.values[rawIndex][columnIndex];

    Matrix result(this->rawCount, this->columnCount, values);
    return result;
}

const Matrix Matrix::operator*(const double &multyplier)const{
    std::vector<std::vector<double>> values(this->values);

    for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
        for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
            values[rawIndex][columnIndex] *= multyplier;

    Matrix result(this->rawCount, this->columnCount, values);
    return result;
}

const Matrix Matrix::operator*(const Matrix &matrix)const{
    std::vector<std::vector<double>> values;

    if(this->rawCount == matrix.columnCount)
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++){
	    std::vector<double> raw(matrix.columnCount);
            values.push_back(raw);
            for(std::size_t columnIndex=0; columnIndex<matrix.columnCount; columnIndex++){
                double summ = 0.0;
                for(std::size_t sumIndex=0; sumIndex<this->columnCount; sumIndex++)
                    summ +=this->values[rawIndex][sumIndex]*matrix.values[sumIndex][columnIndex];
                values[rawIndex].push_back(summ);
            }
        }

    Matrix result(this->rawCount, matrix.columnCount, values);
    return result;
}

const std::string Matrix::toString()const{
    std::stringstream ss;
    for(std::vector<double> raw:values){
        for(double value:raw)
            ss << value << "\t";
        ss.seekp(-1, std::ios_base::end);
        ss << "\n";
    }
    return ss.str();
}
