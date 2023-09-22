#include <iostream>
#include <string>
#include <sstream>
#include <initializer_list>
#include <vector>
#include "Matrix.h"

MatrixSize::MatrixSize(){
    rowCount = 0;
    columnCount = 0;
}

MatrixSize::MatrixSize(const MatrixSize &size){
    rowCount = size.rowCount;
    columnCount = size.columnCount;
}

MatrixSize::MatrixSize(const std::pair<size_t, size_t> &size){
    rowCount = size.first;
    columnCount = size.second;
}

MatrixSize::MatrixSize(const size_t &rowCount, const size_t &columnCount){
    this->rowCount = rowCount;
    this->columnCount = columnCount;
}

const MatrixSize MatrixSize::T()const{
    MatrixSize size(this->columnCount, this->rowCount);
    return size;
}

const bool MatrixSize::operator==(const MatrixSize &size)const{
    return (this->columnCount==size.columnCount)&&(this->rowCount==size.rowCount);
}

Matrix::Matrix(const size_t &rowCount, const size_t &columnCount, double defaultValue){
    MatrixSize size(rowCount, columnCount);
    this->size = size;

    for(size_t rowIndex=0; rowIndex<rowCount; rowIndex++){
        std::vector<double> row;
        values.push_back(row);
        for(size_t columnIndex=0; columnIndex<columnCount; columnIndex++)
            row.push_back(defaultValue);
    }
}

Matrix::Matrix(const MatrixSize &size, double defaultValue){
    this->size = {size};

    for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++){
        std::vector<double> row;
        values.push_back(row);
        for(size_t columnIndex=0; columnIndex<size.columnCount; columnIndex++)
            row.push_back(defaultValue);
    }
}

Matrix::Matrix(const std::initializer_list< std::initializer_list<double> > &values){
    MatrixSize size(values.size(), (*(values.begin())).size());
    this->size = size;
    for(auto row:values){
        std::vector<double> newRow = row;
        this->values.push_back(newRow);
    }
}

Matrix::Matrix(const std::vector<std::vector<double>> &values){
    MatrixSize size(values.size(), values.at(0).size());
    this->size = size;
    this->values = values;
}

Matrix::Matrix(const Matrix& matrix){
    this->size = {matrix.size};
    std::vector<std::vector<double>> newValues(matrix.values);
    this->values = newValues;
}

void Matrix::operator=(const Matrix& matrix){
    this->size = matrix.size;
    this->values = matrix.values;
}

const Matrix Matrix::T()const{
    std::vector<std::vector<double>> values;

    for(size_t rowIndex=0; rowIndex<size.columnCount; rowIndex++){
        std::vector<double> row;
        values.push_back(row);
        for(size_t columnIndex=0; columnIndex<size.rowCount; columnIndex++)
            row.push_back(this->values[columnIndex][rowIndex]);
    }

    Matrix result(values);
    return result;
}

const Matrix Matrix::operator+(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++)
            for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++)
                values[rowIndex][columnIndex] += matrix.values[rowIndex][columnIndex];

    Matrix result(values);
    return result;
}

const Matrix Matrix::operator-(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++)
            for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++)
                values[rowIndex][columnIndex] -= matrix.values[rowIndex][columnIndex];

    Matrix result(values);
    return result;
}

const Matrix Matrix::operator*(const double &multyplier)const{
    std::vector<std::vector<double>> values(this->values);

    for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++)
        for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++)
            values[rowIndex][columnIndex] *= multyplier;

    Matrix result(values);
    return result;
}

const Matrix Matrix::operator*(const Matrix &matrix)const{
    std::vector<std::vector<double>> values;

    if(this->size.rowCount == matrix.size.columnCount)
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++){
	    std::vector<double> row;
            values.push_back(row);
            for(size_t columnIndex=0; columnIndex<matrix.size.columnCount; columnIndex++){
                double summ = 0.0;
                for(size_t sumIndex=0; sumIndex<this->size.columnCount; sumIndex++)
                    summ +=this->values[rowIndex][sumIndex]*matrix.values[sumIndex][columnIndex];
                values[rowIndex].push_back(summ);
            }
        }

    Matrix result(values);
    return result;
}

const std::string Matrix::toString()const{
    std::stringstream ss;
    for(std::vector<double> row:values){
        for(double value:row)
            ss << value << "\t";
        ss.seekp(-1, std::ios_base::end);
        ss << "\n";
    }
    return ss.str();
}
