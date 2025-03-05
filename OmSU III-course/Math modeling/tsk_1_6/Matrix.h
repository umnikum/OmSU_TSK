#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <initializer_list>
#include <vector>
#include <functional>

class MatrixSize{
    public:
        std::size_t rowCount;
        std::size_t columnCount;

        MatrixSize():rowCount(0),columnCount(0){}
        MatrixSize(const std::pair<size_t, std::size_t> &size):rowCount(size.first), columnCount(size.second){}
        MatrixSize(const MatrixSize &size):rowCount(size.rowCount),columnCount(size.columnCount){}
        MatrixSize(const std::size_t &rowCount, const std::size_t &columnCount):rowCount(rowCount),columnCount(columnCount){}
        std::size_t size()const{return rowCount*columnCount;}
        MatrixSize T()const{return MatrixSize(this->columnCount, this->rowCount);}
        bool is_single_element_matrix()const{return (columnCount==1) && (rowCount==1);}
        bool is_symmetric()const{return columnCount==rowCount;}
        bool is_inside(const std::size_t &row, const std::size_t &column)const{return (row<rowCount and column<columnCount);}
        friend bool operator==(const MatrixSize &l, const MatrixSize &r){return l.rowCount==r.rowCount and l.columnCount==r.columnCount;}
        friend std::string to_string(const MatrixSize &size){
        	std::stringstream ss;
        	ss << size.rowCount << ", " << size.columnCount;
			return ss.str();
        }
};

template<class Type = double>
class Matrix{
    public:
        MatrixSize size;
        Type* values=nullptr;

        Matrix(){}
        /*Matrix(const std::size_t &rowCount, const std::size_t &columnCount, const Type &defaultValue={0.0}):size(rowCount, columnCount){
        	const std::size_t length = size.size();
        	values = new Type[length];
            for(std::size_t i=0; i<length; ++i)
                *(values+i) = defaultValue;
        }*/
        Matrix(const MatrixSize &size, const Type &defaultValue={0.0}):size(size){
        	const std::size_t length = size.size();
        	values = new Type[length];
        	for(std::size_t i=0; i<length; ++i)
        	    *(values+i) = defaultValue;
        }
        /*Matrix(const std::size_t &rowCount, const std::size_t &columnCount, const std::initializer_list<Type> &values):size(rowCount, columnCount){
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        		*(this->values+i) = values[i];
        }*/
        Matrix(const MatrixSize &size, const std::initializer_list<Type> &values):size(size){
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        		*(this->values+i) = *(values.begin()+i);
        }
        /*Matrix(const std::size_t &rowCount, const std::size_t &columnCount, const Type *values):size(rowCount, columnCount){
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        	   *(this->values+i) = values[i];
        }*/
        Matrix(const MatrixSize &size, const std::function<Type(const std::size_t &i, const std::size_t &j)> func):size{size}{
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        	   *(this->values+i) = func(i/size.rowCount, i%size.columnCount);
        }
        Matrix(const Matrix& matrix):size(matrix.size){
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        	   *(this->values+i) = matrix.values[i];
        }
        Matrix(const Matrix* matrixp):size(matrixp->size){
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        	   *(this->values+i) = matrixp->values[i];
        }
        ~Matrix(){
        	delete [] values;
        	values = nullptr;
        }
        Matrix<Type>& operator=(const Matrix<Type> &matrix){
        	size = matrix.size;
        	delete [] values;
        	const std::size_t length = size.size();
        	this->values = new Type[size.size()];
        	for(std::size_t i=0; i<length; ++i)
        	     *(this->values+i) = matrix.values[i];
        	return *this;
        }
        Type* operator[](const std::size_t &index){return (values+index*size.columnCount);}
        const Type* operator[](const std::size_t &index)const{return (values+index*size.columnCount);}
        Type& at(const std::size_t &rIndex, const std::size_t &cIndex){
        	assert(size.is_inside(rIndex, cIndex));
        	return values[rIndex*size.columnCount+cIndex];
        }
        const Type& at(const std::size_t &rIndex, const std::size_t &cIndex)const{
        	assert(size.is_inside(rIndex, cIndex));
        	return values[rIndex*size.columnCount+cIndex];
        }
        //Matrix<Type> copy()const;
        Matrix<Type> T()const{
            Matrix<Type> matrix{size.T()};

            for(std::size_t rowIndex=0; rowIndex<size.columnCount; ++rowIndex)
                for(std::size_t columnIndex=0; columnIndex<size.rowCount; ++columnIndex)
                    matrix[rowIndex][columnIndex] = this->at(columnIndex, rowIndex);

            return matrix;
        }
        Matrix<Type> operator+(const Matrix &matrix)const{
        	assert(size == matrix.size);
            Matrix<Type> result{this};
            std::size_t length=size.size();
            if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
                for(std::size_t index=0; index<length; ++index)
                	result.values[index] += matrix.values[index];

            return result;
        }
        Matrix<Type> operator-(const Matrix &matrix)const{
        	assert(size == matrix.size);
        	Matrix<Type> result{this};
        	std::size_t length=size.size();
        	for(std::size_t index=0; index<length; ++index)
        		result.values[index] -= matrix.values[index];

        	return result;
        }
        friend Matrix<Type> operator-(const Matrix &matrix){
        	Matrix<Type> result{matrix.size};
        	for(std::size_t index=0; index<matrix.values.size(); ++index)
        		result.values[index] = -matrix.values[index];
        	return result;
        }
        Matrix<Type> operator*(const Type &multyplier)const{
            Matrix<Type> matrix{this};
            std::size_t length=size.size();
            for(std::size_t index=0; index<length; ++index)
            	matrix.values[index] *= multyplier;

            return matrix;
        }
        std::vector<Type> operator*(const std::vector<Type> &vector)const{
        	assert(size.rowCount==vector.size());
            std::vector<Type> result(size.rowCount);
            for(std::size_t rowIndex=0; rowIndex<size.rowCount; ++rowIndex)
                for(std::size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
                    result[rowIndex] += this->at(rowIndex, columnIndex)*vector.at(columnIndex);

            return result;
        }
        Matrix<Type> operator*(const Matrix<Type> &matrix)const{
        	assert(size.rowCount == matrix.size.columnCount);
            Matrix<Type> result{size.rowCount, matrix.size.columnCount};
            for(std::size_t rowIndex=0; rowIndex<this->size.rowCount; ++rowIndex)
            	for(std::size_t sumIndex=0; sumIndex<this->size.columnCount; ++sumIndex)
            		for(std::size_t columnIndex=0; columnIndex<matrix.size.columnCount; ++columnIndex)
                        result[rowIndex][columnIndex] += this->at(rowIndex, sumIndex)*matrix.at(sumIndex, columnIndex);

            return result;
        }
        Matrix<Type> algebraicMinor(const std::size_t &rowIndex, const std::size_t &columnIndex)const{
        	assert((this->size.rowCount > rowIndex)and(this->size.columnCount > columnIndex));
            std::vector<Type> values{};
            for(std::size_t rIndex=0; rIndex<this->size.rowCount; ++rIndex)
                if(rIndex != rowIndex)
                    for(std::size_t cIndex=0; cIndex<this->size.columnCount; ++cIndex)
                        if(cIndex != columnIndex)
                            values.push_back(at(rIndex, cIndex));
            return Matrix<Type>{size.rowCount-1, size.columnCount-1, values.to_array()};
        }
        Type det()const{
            if(size.is_single_element_matrix())
                return at(0, 0);
            else{
                Type result=0.0;
                for(std::size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
                    result+=at(0, columnIndex)*(-2*(columnIndex%2)+1)*algebraicMinor(0, columnIndex).det();
                return result;
            }
        }
        Matrix<Type> inverse()const{
            assert(size.is_symmetric());
            if(det() == 0.0)
                return Matrix<Type>{};
            else{
                Matrix<Type> matrix{size.rowCount, 2*size.columnCount};
                for(std::size_t rowIndex=0; rowIndex<size.rowCount; ++rowIndex)
                	for(std::size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
                    	matrix[rowIndex][columnIndex] = at(rowIndex, columnIndex);

                for(std::size_t index=0; index<size.rowCount; ++index)
                   	matrix[index][size.columnCount + index] = 1;

                for(std::size_t mainIndex=0; mainIndex<size.rowCount; ++mainIndex){
                   	if(matrix[mainIndex][mainIndex] == 0.0){
                   		for(std::size_t rowIndex=mainIndex+1; rowIndex<size.rowCount; ++rowIndex)
                   			if((matrix[mainIndex][rowIndex] > 0.0)||(matrix[mainIndex][rowIndex] < -0.0)){
                   				const Type multyplicator = 1.0/matrix[rowIndex][mainIndex];
                   				for(std::size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                   					matrix[mainIndex][columnIndex] += multyplicator*matrix[rowIndex][columnIndex];
                   				break;
                   			}
                   	}else if(matrix[mainIndex][mainIndex] != 1.0){
                   		const Type multyplicator = 1.0/matrix[mainIndex][mainIndex];
                   		for(std::size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                   			matrix[mainIndex][columnIndex] *= multyplicator;
                   	}
                   	for(std::size_t rowIndex=mainIndex+1; rowIndex<size.rowCount; ++rowIndex)
                   		if((matrix[rowIndex][mainIndex] > 0.0)||(matrix[rowIndex][mainIndex] < -0.0)){
                   			const Type multyplicator = matrix[rowIndex][mainIndex]/matrix[mainIndex][mainIndex];
                   			for(std::size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                   				matrix[rowIndex][columnIndex] -= multyplicator*matrix[mainIndex][columnIndex];
                   		}
                }

                for(std::size_t mainIndex=size.rowCount-1; mainIndex>0; --mainIndex){
                    for(std::size_t rowIndex=0; rowIndex<mainIndex; ++rowIndex){
                        if((matrix[rowIndex][mainIndex] > 0.0)||(matrix[rowIndex][mainIndex] < -0.0)){
                            const Type multyplicator = matrix[rowIndex][mainIndex]/matrix[mainIndex][mainIndex];
                            for(std::size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                                matrix[rowIndex][columnIndex] -= multyplicator*matrix[mainIndex][columnIndex];
                        }
                    }
                }
                Matrix<Type> result{size};
                for(std::size_t rowIndex=0; rowIndex<size.rowCount; ++rowIndex)
                	for(std::size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
                		result[rowIndex][columnIndex] = matrix.at(rowIndex, columnIndex+size.columnCount);
                return result;
            }
        }
        friend std::string to_string(const Matrix<Type> &matrix){
            std::stringstream ss;
            std::size_t length=matrix.size.size();
            for(std::size_t index=0; index<length; ++index){
                ss << to_string(matrix.values[index]);
                ss << ((index % matrix.size.columnCount == matrix.size.columnCount-1)? "\n" : "\t");
            }
            return ss.str();
        }
};

#endif // MATRIX_H_INCLUDED
