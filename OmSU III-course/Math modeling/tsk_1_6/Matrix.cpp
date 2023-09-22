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
    return MatrixSize(this->columnCount, this->rowCount);
}

const bool MatrixSize::isSimmetrical()const{
    return columnCount==rowCount;
}

const bool MatrixSize::isElement()const{
    return (columnCount==1) && (rowCount==1);
}

Matrix::Matrix(){};

Matrix::Matrix(const size_t &rowCount, const size_t &columnCount, double defaultValue){
    this->size = MatrixSize(rowCount, columnCount);

    for(size_t rowIndex=0; rowIndex<rowCount; rowIndex++){
        values.push_back(std::vector<double>());
        for(size_t columnIndex=0; columnIndex<columnCount; columnIndex++)
            values[rowIndex].push_back(defaultValue);
    }
}

Matrix::Matrix(const MatrixSize &size, double defaultValue){
    this->size = {size};

    for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++){
        values.push_back(std::vector<double>());
        for(size_t columnIndex=0; columnIndex<size.columnCount; columnIndex++)
            values[rowIndex].push_back(defaultValue);
    }
}

Matrix::Matrix(const std::initializer_list< std::initializer_list<double> > &values){
    this->size = MatrixSize(values.size(), (*(values.begin())).size());
    for(auto row:values)
        this->values.push_back(std::vector<double>(row));
}

Matrix::Matrix(const std::vector<std::vector<double>> &values){
    this->size = MatrixSize(values.size(), values.at(0).size());
    this->values = values;
}

Matrix::Matrix(const Matrix& matrix){
    this->size = {matrix.size};
    this->values = std::vector<std::vector<double>>(matrix.values);
}

const Matrix Matrix::T()const{
    std::vector<std::vector<double>> values;

    for(size_t rowIndex=0; rowIndex<size.columnCount; rowIndex++){
        values.push_back(std::vector<double>());
        for(size_t columnIndex=0; columnIndex<size.rowCount; columnIndex++)
            values[rowIndex].push_back(this->values[columnIndex][rowIndex]);
    }

    return Matrix(values);
}

const Matrix Matrix::operator+(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++)
            for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++)
                values[rowIndex][columnIndex] += matrix.values[rowIndex][columnIndex];

    return Matrix(values);
}

const Matrix Matrix::operator-(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++)
            for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++)
                values[rowIndex][columnIndex] -= matrix.values[rowIndex][columnIndex];

    return Matrix(values);
}

const Matrix Matrix::operator*(const double &multyplier)const{
    std::vector<std::vector<double>> values(this->values);

    for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++)
        for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++)
            values[rowIndex][columnIndex] *= multyplier;

    return Matrix(values);
}

const Matrix Matrix::operator*(const Matrix &matrix)const{
    std::vector<std::vector<double>> values;

    if(this->size.rowCount == matrix.size.columnCount)
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++){
            values.push_back(std::vector<double>());
            for(size_t columnIndex=0; columnIndex<matrix.size.columnCount; columnIndex++){
                double summ = 0.0;
                for(size_t sumIndex=0; sumIndex<this->size.columnCount; sumIndex++)
                    summ +=this->values[rowIndex][sumIndex]*matrix.values[sumIndex][columnIndex];
                values[rowIndex].push_back(summ);
            }
        }

    return Matrix(values);
}

const Matrix Matrix::algebraicMinor(const size_t &rowIndex, const size_t &columnIndex)const{
    if((this->size.rowCount > rowIndex)&&(this->size.columnCount > columnIndex)){
        std::vector<std::vector<double>> values;
        for(size_t rIndex=0; rIndex<this->size.rowCount; rIndex++)
            if(rIndex != rowIndex){
                values.push_back(std::vector<double>());
                for(size_t cIndex=0; cIndex<this->size.columnCount; cIndex++)
                    if(cIndex != columnIndex)
                        (*std::prev(values.end())).push_back(this->values[rIndex][cIndex]);
            }
        return (Matrix(values));
    }else return Matrix();
}

const double Matrix::det()const{
    if(size.isElement())
        return values[0][0];
    else{
        double result=0.0;
        for(size_t columnIndex=0; columnIndex<size.columnCount; columnIndex++)
            result+=values[0][columnIndex]*(-2*(columnIndex%2)+1)*algebraicMinor(0, columnIndex).det();
        return result;
    }
}

const Matrix Matrix::inverse()const{
    if(size.isSimmetrical()){
        if(det() == 0.0)
            return Matrix();
        else{
            std::vector<std::vector<double>> values(this->values);
            for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++)
                for(size_t columnIndex=0; columnIndex<size.columnCount; columnIndex++)
                    if(rowIndex == columnIndex)
                        values[rowIndex].push_back(1);
                    else values[rowIndex].push_back(0);

            for(size_t mainIndex=0; mainIndex<size.rowCount; mainIndex++){
                if(values[mainIndex][mainIndex] == 0.0){
                    for(size_t rowIndex=mainIndex+1; rowIndex<size.rowCount; rowIndex++)
                    	if((values[mainIndex][rowIndex] > 0.0)||(values[mainIndex][rowIndex] < -0.0)){
                            const double multyplicator = 1.0/values[rowIndex][mainIndex];
                            for(size_t columnIndex=mainIndex; columnIndex<values[rowIndex].size(); columnIndex++)
                                values[mainIndex][columnIndex] += multyplicator*values[rowIndex][columnIndex];
                            break;
                        }
                }else if(values[mainIndex][mainIndex] != 1.0){
                    const double multyplicator = 1.0/values[mainIndex][mainIndex];
                    for(size_t columnIndex=mainIndex; columnIndex<values[mainIndex].size(); columnIndex++)
                        values[mainIndex][columnIndex] *= multyplicator;
                }
                for(size_t rowIndex=mainIndex+1; rowIndex<size.rowCount; rowIndex++)
                	if((values[rowIndex][mainIndex] > 0.0)||(values[rowIndex][mainIndex] < -0.0)){
                        const double multyplicator = values[rowIndex][mainIndex]/values[mainIndex][mainIndex];
                        for(size_t columnIndex=mainIndex; columnIndex<values[rowIndex].size(); columnIndex++)
                            values[rowIndex][columnIndex] -= multyplicator*values[mainIndex][columnIndex];
                    }
            }

            for(size_t mainIndex=size.rowCount-1; mainIndex>0; mainIndex--){
                for(size_t rowIndex=0; rowIndex<mainIndex; rowIndex++){
                    if((values[rowIndex][mainIndex] > 0.0)||(values[rowIndex][mainIndex] < -0.0)){
                        const double multyplicator = values[rowIndex][mainIndex]/values[mainIndex][mainIndex];
                        for(size_t columnIndex=mainIndex; columnIndex<values[rowIndex].size(); columnIndex++)
                            values[rowIndex][columnIndex] -= multyplicator*values[mainIndex][columnIndex];
                    }
                }
            }
            for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++)
                values[rowIndex].erase(values[rowIndex].begin(), values[rowIndex].begin()+size.columnCount);
            return Matrix(values);
        }
    }else return Matrix();
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
