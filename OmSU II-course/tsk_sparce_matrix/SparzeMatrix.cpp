#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "SparseMatrix.h"
#include "Matrix.h"

SparseMatrix::SparseMatrix(const std::size_t &rawCount, const std::size_t &columnCount, std::map<std::size_t, std::map<std::size_t, double>> &values){
    this->rawCount = rawCount;
    this->columnCount = columnCount;
    this->rawValues = values;
}

SparseMatrix::SparseMatrix(const SparseMatrix &sparseMatrix){
    this->rawCount = sparseMatrix.rawCount;
    this->columnCount = sparseMatrix.columnCount;
    std::map<std::size_t, std::map<std:: size_t, double>> values(sparseMatrix.rawValues);
    this->rawValues = values;
}

SparseMatrix::SparseMatrix(const Matrix &matrix){
    this->rawCount = matrix.rawCount;
    this->columnCount = matrix.columnCount;
    std::map<std::size_t, std::map<std:: size_t, double>> values;
    for(std::size_t rawIndex=0; rawIndex<rawCount; rawIndex++){
        std::map<std::size_t, double> raw;
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++)
            raw.emplace(columnIndex, matrix.values[rawIndex][columnIndex]);
        values.emplace(rawIndex, raw);
    }
    this->rawValues = values;
    this->cut();
}

void SparseMatrix::operator=(const SparseMatrix &sparseMatrix){
    this->rawCount = sparseMatrix.rawCount;
    this->columnCount = sparseMatrix.columnCount;
    this->rawValues = sparseMatrix.rawValues;
}

const std::pair<std::vector<std::size_t>, std::vector<std::size_t>> SparseMatrix::getIndexes()const{
    std::vector<std::size_t> rawIndexes, columnIndexes;
    std::pair<std::vector<std::size_t>, std::vector<std::size_t>> indexes = std::make_pair(rawIndexes, columnIndexes);

    for(auto rawPair:rawValues){
        for(auto valuePair:rawPair.second){
            bool found = false;
            for(std::size_t index:indexes.second)
                if(index == valuePair.first){
                    found = true;
                    break;
                }
            if(!found)
                indexes.second.push_back(valuePair.first);
        }
        indexes.first.push_back(rawPair.first);
    }
    std::sort(indexes.second.begin(), indexes.second.end());
    return indexes;
}

void SparseMatrix::cut(){
    std::map<std::size_t, std::map<std:: size_t, double>> clearValues;
    for(auto rawPair:rawValues){
        std::map<std::size_t, double> newRaw;
        for(auto valuePair:rawPair.second){
            if(std::abs(valuePair.second) > accuracy)
                newRaw[valuePair.first] = valuePair.second;
        }
        if(!newRaw.empty())
            clearValues[rawPair.first] = newRaw;
    }
    this->rawValues = clearValues;
}

const SparseMatrix SparseMatrix::T()const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;
    std::vector<std::size_t> columnIndexes = this->getIndexes().second;

    for(auto rawIndex:columnIndexes){
        std::map<std::size_t, double> newRaw;
        values.emplace(rawIndex, newRaw);
        for(auto rawPair:this->rawValues)
            if(rawPair.second.count(rawIndex) > 0)
                newRaw[rawPair.first] = rawPair.second[rawIndex];
    }

    SparseMatrix result(this->columnCount, this->rawCount, values);
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator+(const Matrix &matrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if((this->rawCount == matrix.rawCount)&&(this->columnCount == matrix.columnCount)){
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
            for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
                values[rawIndex][columnIndex] = matrix.values[rawIndex][columnIndex];

        for(auto rawPair:this->rawValues)
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] += valuePair.second;
    }

    SparseMatrix result(this->rawCount, this->columnCount, values);
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator-(const Matrix &matrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if((this->rawCount == matrix.rawCount)&&(this->columnCount == matrix.columnCount)){
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
            for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
                values[rawIndex][columnIndex] = -matrix.values[rawIndex][columnIndex];

        for(auto rawPair:this->rawValues)
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] += valuePair.second;
    }

    SparseMatrix result(this->rawCount, this->columnCount, values);
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator*(const Matrix &matrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if(this->rawCount == matrix.columnCount)
        for(auto rawPair:rawValues){
            std::map<std::size_t, double> newRaw;
            values.emplace(rawPair.first, newRaw);
            for(std::size_t columnIndex=0; columnIndex<matrix.columnCount; columnIndex++){
                values[rawPair.first][columnIndex] = 0.0;
                for(auto valuePair:rawPair.second)
                    values[rawPair.first][columnIndex] += valuePair.second * matrix.values[columnIndex][valuePair.first];
            }
        }

    SparseMatrix result(this->rawCount, matrix.columnCount, values);
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator*(const double &multyplier)const{
    std::map<std::size_t, std::map<std::size_t, double>> values(this->rawValues);

    for(auto rawPair:values)
        for(auto valuePair:rawPair.second)
            values[rawPair.first][valuePair.first] *= multyplier;

    SparseMatrix result(this->rawCount, this->columnCount, values);
    return result;
}

const SparseMatrix SparseMatrix::operator+(const SparseMatrix &sparseMatrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if((this->rawCount == sparseMatrix.rawCount)&&(this->columnCount == sparseMatrix.columnCount)){
        for(auto rawPair:this->rawValues){
            std::map<std::size_t, double> newRaw;
            values.emplace(rawPair.first, newRaw);
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] = valuePair.second;
        }
        for(auto rawPair:sparseMatrix.rawValues){
            if(values.count(rawPair.first) == 0){
                std::map<std::size_t, double> newRaw;
                values.emplace(rawPair.first, newRaw);
            }
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] += valuePair.second;
        }
    }

    SparseMatrix result(this->rawCount, this->columnCount, values);
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator-(const SparseMatrix &sparseMatrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if((this->rawCount == sparseMatrix.rawCount)&&(this->columnCount == sparseMatrix.columnCount)){
        for(auto rawPair:this->rawValues){
            std::map<std::size_t, double> newRaw;
            values.emplace(rawPair.first, newRaw);
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] = valuePair.second;

        }
        for(auto rawPair:sparseMatrix.rawValues){
            if(values.count(rawPair.first) == 0){
                std::map<std::size_t, double> newRaw;
                values.emplace(rawPair.first, newRaw);
            }
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] -= valuePair.second;
        }
    }

    SparseMatrix result(this->rawCount, this->columnCount, values);
    result.cut();
    return result;
}

const SparseMatrix SparseMatrix::operator*(const SparseMatrix &sparseMatrix)const{
    std::map<std::size_t, std::map<std::size_t, double>> values;
    SparseMatrix tMatrix = sparseMatrix.T();

    if(this->rawCount == tMatrix.rawCount)
        for(auto rawPair:this->rawValues){
            std::map<std::size_t, double> newRaw;
            values.emplace(rawPair.first, newRaw);
            for(auto columnPair:tMatrix.rawValues){
                values[rawPair.first][columnPair.first] = 0.0;
                for(auto valuePair:rawPair.second)
                    if(columnPair.second.count(valuePair.first) > 0)
                        values[rawPair.first][columnPair.first] += valuePair.second * columnPair.second[valuePair.first];
            }
        }

    SparseMatrix result(this->rawCount, sparseMatrix.columnCount, values);
    result.cut();
    return result;
}

const std::string SparseMatrix::toString()const{
    std::stringstream ss;
    auto indexes = getIndexes();

    ss << "\t";
    for(std::size_t columnIndex:indexes.second)
        ss << "#" << columnIndex << "\t";
    ss << std::endl;
    for(std::size_t rawIndex:indexes.first){
        ss << "#" << rawIndex << "\t";
        for(std::size_t columnIndex:indexes.second){
            if(this->rawValues.at(rawIndex).count(columnIndex) > 0)
                ss << this->rawValues.at(rawIndex).at(columnIndex) << "\t";
            else ss << " \t";
        }
        ss << std::endl;
    }
    return ss.str();
}
