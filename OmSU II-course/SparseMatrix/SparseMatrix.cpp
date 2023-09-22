#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "SparseMatrix.h"
#include "Matrix.h"

SparseMatrix::SparseMatrix(){}

SparseMatrix::SparseMatrix(const MatrixSize &size, std::vector<double> &values, std::vector<size_t> &columnIndexes, std::vector<size_t> &rowIndexRanges){
    this->size = size;
    this->values = values;
    this->columnIndexes = columnIndexes;
    this->rowIndexRanges = rowIndexRanges;
}

SparseMatrix::SparseMatrix(const MatrixSize &size, std::map<size_t, std::map<size_t, double>> &valuesMap){
    MatrixSize newSize(size);
    this->size = newSize;

    std::vector<double> values;
    std::vector<size_t> columnIndexes;
    std::vector<size_t> rowIndexRanges;

    rowIndexRanges.push_back(0);
    for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++){
        rowIndexRanges.push_back(rowIndexRanges[rowIndex]);
        if(valuesMap.count(rowIndex) > 0){
            for(auto columnIndexValuePair:valuesMap.at(rowIndex)){
                values.push_back(columnIndexValuePair.second);
                columnIndexes.push_back(columnIndexValuePair.first);
                rowIndexRanges[rowIndex+1]++;
            }
        }
    }

    this->values = values;
    this->columnIndexes = columnIndexes;
    this->rowIndexRanges = rowIndexRanges;
    this->validate();
}

SparseMatrix::SparseMatrix(const SparseMatrix &sparseMatrix){
    MatrixSize size(sparseMatrix.size);
    std::vector<double> values(sparseMatrix.values);
    std::vector<size_t> columnIndexes(sparseMatrix.columnIndexes),
                        rowIndexRanges(sparseMatrix.rowIndexRanges);
    this->size = size;
    this->values = values;
    this->columnIndexes = columnIndexes;
    this->rowIndexRanges = rowIndexRanges;
}

SparseMatrix::SparseMatrix(const Matrix &matrix){
    MatrixSize size(matrix.size);
    this->size = size;

    std::vector<double> values;
    std::vector<size_t> columnIndexes, rowIndexRanges;
    rowIndexRanges.push_back(0);
    for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++){
        rowIndexRanges.push_back(rowIndexRanges[rowIndex]);
        for(std::size_t columnIndex=0; columnIndex<size.columnCount; columnIndex++){
            double value = matrix.values[rowIndex][columnIndex];
            if(std::abs(value) > accuracy){
                values.push_back(value);
                rowIndexRanges[rowIndex+1]++;
                columnIndexes.push_back(columnIndex);
            }
        }
    }
    this->values = values;
    this->columnIndexes = columnIndexes;
    this->rowIndexRanges = rowIndexRanges;
}

void SparseMatrix::operator=(const SparseMatrix &sparseMatrix){
    this->size = sparseMatrix.size;
    this->values = sparseMatrix.values;
    this->columnIndexes = sparseMatrix.columnIndexes;
    this->rowIndexRanges = sparseMatrix.rowIndexRanges;
}

void SparseMatrix::operator=(const SparseMatrix &&sparseMatrix){
    this->size = sparseMatrix.size;
    this->values = sparseMatrix.values;
    this->columnIndexes = sparseMatrix.columnIndexes;
    this->rowIndexRanges = sparseMatrix.rowIndexRanges;
    sparseMatrix.size = nullptr;
    sparseMatrix.values = nullptr;
    sparseMatrix.columnIndexes = nullptr;
    sparseMatrix.rowIndexRanges = nullptr;
}

void SparseMatrix::cut(){
    size_t index=values.size()-1, rowIndex=size.rowCount-1;
    while(index>0){
        while(rowIndexRanges[rowIndex] > index)
            rowIndex--;
        if(std::abs(values[index]) > accuracy){
            while(rowIndexRanges[rowIndex] > index)
                rowIndex--;
        }else{
            values.erase(values.begin()+index);
            columnIndexes.erase(columnIndexes.begin()+index);
            for(size_t rIndex=rowIndex+1; rIndex<size.rowCount+1; rIndex++)
                rowIndexRanges[rIndex]--;

        }
        index--;
    }
}

void SparseMatrix::validate(){
    double valueSwap = 0.0;
    size_t columnIndexSwap = 0;
    for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++)
        if(rowIndexRanges[rowIndex+1] - rowIndexRanges[rowIndex] > 1){
            for(size_t checkIndex=rowIndexRanges[rowIndex]+1; checkIndex<rowIndexRanges[rowIndex+1]; checkIndex++)
                for(size_t swapIndex=checkIndex-1; swapIndex>=rowIndexRanges[rowIndex]; swapIndex--){
                    if(columnIndexes[swapIndex]>columnIndexes[swapIndex+1]){
                        columnIndexSwap = columnIndexes[swapIndex+1];
                        columnIndexes[swapIndex+1] = columnIndexes[swapIndex];
                        columnIndexes[swapIndex] = columnIndexSwap;

                        valueSwap = values[swapIndex+1];
                        values[swapIndex+1] = values[swapIndex];
                        values[swapIndex] = valueSwap;
                    }else break;
                }
        }
}

const size_t SparseMatrix::getRowIndex(const size_t &valuesIndex)const{
    for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++)
        if((rowIndexRanges[rowIndex]<=valuesIndex)&&(valuesIndex<rowIndexRanges[rowIndex+1]))
            return rowIndex;
    return 0;
}

const double SparseMatrix::operator()(const size_t &rowIndex, const size_t &columnIndex)const{
    double result=0.0;
    for(size_t index=rowIndexRanges[rowIndex]; index < rowIndexRanges[rowIndex+1]; index++)
        if(columnIndexes[index] == columnIndex){
            result = values[index];
            break;
        }else if(columnIndexes[index] > columnIndex)
            break;
    return result;
}

const SparseMatrix SparseMatrix::T()const{
    MatrixSize size = this->size.T();
    std::vector<double> values;
    std::vector<size_t> rowIndexes, columnIndexRanges;

    columnIndexRanges.push_back(0);
    for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++){
        columnIndexRanges.push_back(columnIndexRanges[columnIndex]);
        size_t rowIndex=0, index=0;
        while(index < this->columnIndexes.size()){
            while(rowIndexRanges[rowIndex+1] <= index)
                rowIndex++;
            if(this->columnIndexes[index] == columnIndex){
                columnIndexRanges[columnIndex+1]++;
                rowIndexes.push_back(rowIndex);
                values.push_back(this->values[index]);
            }else if(this->columnIndexes[index] > columnIndex){
                index = rowIndexRanges[rowIndex+1]-1;
            }
            index++;
        }
    }

    SparseMatrix result(size, values, rowIndexes, columnIndexRanges);
    return result;
}

const SparseMatrix SparseMatrix::operator+(const Matrix &matrix)const{
    if(this->size==matrix.size){
        Matrix memory(matrix);
        for(size_t index=0; index<values.size(); index++)
            memory.values[this->getRowIndex(index)][columnIndexes[index]]+=this->values[index];
        SparseMatrix result(memory);
        return result;
    }else{
        SparseMatrix result(*this);
        return result;
    }
}

const SparseMatrix SparseMatrix::operator-(const Matrix &matrix)const{
    if(this->size==matrix.size){
        return (*this)+(matrix * -1);
    }else{
        SparseMatrix result(*this);
        return result;
    }
}

const SparseMatrix SparseMatrix::operator*(const Matrix &matrix)const{
    if(this->size.rowCount == matrix.size.columnCount){
        MatrixSize size(this->size.rowCount, matrix.size.columnCount);
        std::vector<double> values;
        std::vector<size_t> columnIndexes, rowIndexRanges;
        
        size_t index=0;
        rowIndexRanges.push_back(0);
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++){
           if(this->rowIndexRanges[rowIndex]<this->rowIndexRanges[rowIndex+1]){
               for(size_t columnIndex=0; columnIndex<matrix.size.columnCount; columnIndex++){
                    double value = 0.0;
                    for(size_t summIndex=this->rowIndexRanges[rowIndex]; summIndex<this->rowIndexRanges[rowIndex+1]; summIndex++)
                       value += this->values[summIndex] * matrix.values[this->columnIndexes[summIndex]][columnIndex];
                    if(std::abs(value) > accuracy){
                        columnIndexes.push_back(columnIndex);
                        values.push_back(value);
                        index++;
                    }
               }
           }
           rowIndexRanges.push_back(index);
        }
        SparseMatrix result(size, values, columnIndexes, rowIndexRanges);
        return result;
    }else{
        SparseMatrix result(*this);
        return result;
    }
}

const SparseMatrix SparseMatrix::operator*(const double &multyplier)const{
    SparseMatrix result(*this);
    for(size_t index=0; index<values.size(); index++)
        result.values[index]*=multyplier;
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator+(const SparseMatrix &sparseMatrix)const{
    if(this->size == sparseMatrix.size){
        MatrixSize size(this->size);
        std::vector<double> values;
        std::vector<size_t> columnIndexes, rowIndexRanges;
        size_t indexOfResult=0, indexOfSelf=0, indexOfMatrix=0;
        
        rowIndexRanges.push_back(0);
        for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++){
            while(this->rowIndexRanges[rowIndex+1] > indexOfSelf){
                double valueFromSelf=this->values[indexOfSelf];
                size_t columnIndex=this->columnIndexes[indexOfSelf];
                while((columnIndex > sparseMatrix.columnIndexes[indexOfMatrix])&&
                    (sparseMatrix.rowIndexRanges[rowIndex+1] > indexOfMatrix)){
                    values.push_back(sparseMatrix.values[indexOfMatrix]);
                    columnIndexes.push_back(sparseMatrix.columnIndexes[indexOfMatrix]);
                    indexOfMatrix++;
                    indexOfResult++;
                }
                if(columnIndex == sparseMatrix.columnIndexes[indexOfMatrix]){
                    valueFromSelf+=sparseMatrix.values[indexOfMatrix];
                    indexOfMatrix++;
                }
                if(std::abs(valueFromSelf)>accuracy){
                    values.push_back(valueFromSelf);
                    columnIndexes.push_back(columnIndex);
                    indexOfResult++;
                }
                indexOfSelf++;
            }
            while(sparseMatrix.rowIndexRanges[rowIndex+1] > indexOfMatrix){
                values.push_back(sparseMatrix.values[indexOfMatrix]);
                columnIndexes.push_back(sparseMatrix.columnIndexes[indexOfMatrix]);
                indexOfMatrix++;
                indexOfResult++;
            }
            rowIndexRanges.push_back(indexOfResult);
        }
        
        SparseMatrix result(size, values, columnIndexes, rowIndexRanges);
        return result;
    }else{
        SparseMatrix result(*this);
        return result;
    }
}

const SparseMatrix SparseMatrix::operator-(const SparseMatrix &sparseMatrix)const{
    if(this->size == sparseMatrix.size){
        MatrixSize size(this->size);
        std::vector<double> values;
        std::vector<size_t> columnIndexes, rowIndexRanges;
        size_t indexOfResult=0, indexOfSelf=0, indexOfMatrix=0;
        
        rowIndexRanges.push_back(0);
        for(size_t rowIndex=0; rowIndex<size.rowCount; rowIndex++){
            while(this->rowIndexRanges[rowIndex+1] > indexOfSelf){
                double valueFromSelf=this->values[indexOfSelf];
                size_t columnIndex=this->columnIndexes[indexOfSelf];
                while((columnIndex > sparseMatrix.columnIndexes[indexOfMatrix])&&
                    (sparseMatrix.rowIndexRanges[rowIndex+1] > indexOfMatrix)){
                    values.push_back(- sparseMatrix.values[indexOfMatrix]);
                    columnIndexes.push_back(sparseMatrix.columnIndexes[indexOfMatrix]);
                    indexOfMatrix++;
                    indexOfResult++;
                }
                if(columnIndex == sparseMatrix.columnIndexes[indexOfMatrix]){
                    valueFromSelf-=sparseMatrix.values[indexOfMatrix];
                    indexOfMatrix++;
                }
                if(std::abs(valueFromSelf)>accuracy){
                    values.push_back(valueFromSelf);
                    columnIndexes.push_back(columnIndex);
                    indexOfResult++;
                }
                indexOfSelf++;
            }
            while(sparseMatrix.rowIndexRanges[rowIndex+1] > indexOfMatrix){
                values.push_back(-sparseMatrix.values[indexOfMatrix]);
                columnIndexes.push_back(sparseMatrix.columnIndexes[indexOfMatrix]);
                indexOfMatrix++;
                indexOfResult++;
            }
            rowIndexRanges.push_back(indexOfResult);
        }
        SparseMatrix result(size, values, columnIndexes, rowIndexRanges);
        return result;
    }else{
        SparseMatrix result(*this);
        return result;
    }
}

const SparseMatrix SparseMatrix::operator*(const SparseMatrix &sparseMatrix)const{
    if(this->size.rowCount == sparseMatrix.size.columnCount){
        SparseMatrix matrix = sparseMatrix.T();
        MatrixSize size(this->size.rowCount, matrix.size.rowCount);
        std::vector<double> values;
        std::vector<size_t> columnIndexes, rowIndexRanges;
        size_t indexOfResult=0, indexOfSelf=0, indexOfMatrix=0;
        
        rowIndexRanges.push_back(0);
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; rowIndex++){
            for(size_t columnIndex=0; columnIndex<this->size.columnCount; columnIndex++){
                indexOfSelf = this->rowIndexRanges[rowIndex];
                double value=0;
                while(this->rowIndexRanges[rowIndex+1] > indexOfSelf){
                    size_t summIndex=this->columnIndexes[indexOfSelf];
                    while((summIndex > matrix.columnIndexes[indexOfMatrix])&&
                        (matrix.rowIndexRanges[columnIndex+1] > indexOfMatrix)){
                        indexOfMatrix++;
                    }
                    if(summIndex == matrix.columnIndexes[indexOfMatrix]){
                        value += matrix.values[indexOfMatrix] * this->values[indexOfSelf];
                        indexOfMatrix++;
                    }
                    indexOfSelf++;
                }
                if(std::abs(value)>accuracy){
                    values.push_back(value);
                    columnIndexes.push_back(columnIndex);
                    indexOfResult++;
                }
            }
            rowIndexRanges.push_back(indexOfResult);
            indexOfMatrix = 0;
        }
        SparseMatrix result(size, values, columnIndexes, rowIndexRanges);
        return result;
    }else{
        SparseMatrix result(*this);
        return result;
    }
}

const std::string SparseMatrix::toString()const{
    std::stringstream ss;
    std::vector<size_t> existingColumnIndexes, existingRowIndexes;
    for(size_t index=0; index<size.rowCount; index++)
        if(rowIndexRanges[index+1] - rowIndexRanges[index] > 0)
            existingRowIndexes.push_back(index);
    for(size_t index=0; index<size.columnCount; index++)
        for(size_t columnIndexIndex=0; columnIndexIndex<columnIndexes.size(); columnIndexIndex++)
            if(index == columnIndexes[columnIndexIndex]){
                existingColumnIndexes.push_back(index);
                break;
            }

    ss << "\t";
    for(std::size_t columnIndex:existingColumnIndexes)
        ss << "#" << columnIndex << "\t";
    ss << std::endl;
    for(std::size_t rowIndex:existingRowIndexes){
        ss << "#" << rowIndex << "\t";
        for(std::size_t columnIndex:existingColumnIndexes)
            ss << this->operator()(rowIndex, columnIndex) << "\t";
        ss << std::endl;
    }
    return ss.str();
}
