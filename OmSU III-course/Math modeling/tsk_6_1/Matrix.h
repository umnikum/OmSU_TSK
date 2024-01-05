#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>
#include <string>
#include <sstream>
#include <initializer_list>
#include <vector>

class MatrixSize{
    public:
        size_t rowCount;
        size_t columnCount;

        MatrixSize();
        MatrixSize(const std::pair<size_t, size_t> &size);
        MatrixSize(const MatrixSize &size);
        MatrixSize(const size_t &rowCount, const size_t &columnCount);
        MatrixSize T()const;
        bool isElement()const;
        bool isSimmetrical()const;
};

class Matrix{
    public:
        MatrixSize size;
        std::vector<double> values;

        Matrix();
        Matrix(const size_t &rowCount, const size_t &columnCount, const double &defaultValue=0.0);
        Matrix(const MatrixSize &size, const double &defaultValue=0.0);
        Matrix(const size_t &rowCount, const size_t &columnCount, const std::initializer_list<double> &values);
        Matrix(const MatrixSize &size, const std::initializer_list<double> &values);
        Matrix(const size_t &rowCount, const size_t &columnCount, const std::vector<double> &values);
        Matrix(const Matrix& matrix);
        Matrix(const Matrix* matrixp);
        std::vector<double>::iterator operator[](const size_t &index);
        double at(const size_t &rIndex, const size_t &cIndex)const;
        Matrix copy()const;
        Matrix T()const;
        Matrix operator+(const Matrix &matrix)const;
        Matrix operator-(const Matrix &matrix)const;
        friend Matrix operator-(const Matrix &matrix){
        	Matrix result{matrix.size};
        	for(size_t index=0; index<matrix.values.size(); ++index)
        		result.values[index] = -matrix.values[index];
        	return result;
        }
        Matrix operator*(const double &multyplier)const;
        std::vector<double> operator*(const std::vector<double> &vector)const;
        Matrix operator*(const Matrix &matrix)const;
        Matrix algebraicMinor(const size_t &rowIndex, const size_t &columnIndex)const;
        double det()const;
        Matrix inverse()const;
        std::string  toString()const;
};

MatrixSize::MatrixSize():rowCount(0),columnCount(0){}

MatrixSize::MatrixSize(const MatrixSize &size):rowCount(size.rowCount),columnCount(size.columnCount){}

MatrixSize::MatrixSize(const std::pair<size_t, size_t> &size):rowCount(size.first), columnCount(size.second){}

MatrixSize::MatrixSize(const size_t &rowCount, const size_t &columnCount):rowCount(rowCount),columnCount(columnCount){}

MatrixSize MatrixSize::T()const{return MatrixSize(this->columnCount, this->rowCount);}

bool MatrixSize::isSimmetrical()const{return columnCount==rowCount;}

bool MatrixSize::isElement()const{return (columnCount==1) && (rowCount==1);}

Matrix::Matrix(){};

Matrix::Matrix(const size_t &rowCount, const size_t &columnCount, const double &defaultValue):
		size(rowCount, columnCount), values(rowCount*columnCount){
    for(double &element:values)
        element = defaultValue;
}

Matrix::Matrix(const MatrixSize &size, const double &defaultValue):
		size(size), values(size.rowCount*size.columnCount){
	for(double &element:values)
	    element = defaultValue;
}

Matrix::Matrix(const size_t &rowCount, const size_t &columnCount, const std::initializer_list<double> &values):
		size(rowCount, columnCount), values(values){}

Matrix::Matrix(const MatrixSize &size, const std::initializer_list<double> &values):
		size(size), values(values){}

Matrix::Matrix(const size_t &rowCount, const size_t &columnCount, const std::vector<double> &values):
		size(rowCount, columnCount), values(values){}

Matrix::Matrix(const Matrix& matrix):size(matrix.size), values(matrix.values){}

Matrix::Matrix(const Matrix* matrixp):size(matrixp->size), values(matrixp->values){}

std::vector<double>::iterator Matrix::operator[](const size_t &index){return std::next(values.begin(), index*size.columnCount);}

double Matrix::at(const size_t &rIndex, const size_t &cIndex)const{return values.at(rIndex*size.columnCount+cIndex);}

Matrix Matrix::T()const{
    Matrix matrix{size.T()};

    for(size_t rowIndex=0; rowIndex<size.columnCount; ++rowIndex)
        for(size_t columnIndex=0; columnIndex<size.rowCount; ++columnIndex)
            matrix[rowIndex][columnIndex] = this->at(columnIndex, rowIndex);

    return matrix;
}

Matrix Matrix::operator+(const Matrix &matrix)const{
    Matrix result{this};

    if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
        for(size_t index=0; index<values.size(); ++index)
        	result.values[index] += matrix.values[index];

    return result;
}

Matrix Matrix::operator-(const Matrix &matrix)const{
	Matrix result{this};

	if((this->size.rowCount == matrix.size.rowCount)&&(this->size.columnCount == matrix.size.columnCount))
		for(size_t index=0; index<values.size(); ++index)
			result.values[index] -= matrix.values[index];

	return result;
}

Matrix Matrix::operator*(const double &multyplier)const{
    Matrix matrix{this};

    for(size_t index=0; index<values.size(); ++index)
    	matrix.values[index] *= multyplier;

    return matrix;
}

std::vector<double> Matrix::operator*(const std::vector<double> &vector)const{
    std::vector<double> result(size.rowCount);

    if(size.rowCount == vector.size())
        for(size_t rowIndex=0; rowIndex<size.rowCount; ++rowIndex)
            for(size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
                result[rowIndex] += this->at(rowIndex, columnIndex)*vector.at(columnIndex);

    return result;
}

Matrix Matrix::operator*(const Matrix &matrix)const{
    Matrix result{size.rowCount, matrix.size.columnCount};

    if(this->size.rowCount == matrix.size.columnCount)
        for(size_t rowIndex=0; rowIndex<this->size.rowCount; ++rowIndex)
            for(size_t columnIndex=0; columnIndex<matrix.size.columnCount; ++columnIndex)
                for(size_t sumIndex=0; sumIndex<this->size.columnCount; ++sumIndex)
                	result[rowIndex][columnIndex] += this->at(rowIndex, sumIndex)*matrix.at(sumIndex, columnIndex);

    return result;
}

Matrix Matrix::algebraicMinor(const size_t &rowIndex, const size_t &columnIndex)const{
    if((this->size.rowCount > rowIndex)&&(this->size.columnCount > columnIndex)){
        std::vector<double> values{};
        for(size_t rIndex=0; rIndex<this->size.rowCount; ++rIndex)
            if(rIndex != rowIndex)
                for(size_t cIndex=0; cIndex<this->size.columnCount; ++cIndex)
                    if(cIndex != columnIndex)
                        values.push_back(at(rIndex, cIndex));
        return Matrix{size.rowCount-1, size.columnCount-1, values};
    }else return Matrix{size.rowCount-1, size.columnCount-1};
}

double Matrix::det()const{
    if(size.isElement())
        return at(0, 0);
    else{
        double result=0.0;
        for(size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
            result+=at(0, columnIndex)*(-2*(columnIndex%2)+1)*algebraicMinor(0, columnIndex).det();
        return result;
    }
}

Matrix Matrix::inverse()const{
    if(size.isSimmetrical()){
        if(det() == 0.0)
            return Matrix{};
        else{
            Matrix matrix{size.rowCount, 2*size.columnCount};
            for(size_t rowIndex=0; rowIndex<size.rowCount; ++rowIndex)
            	for(size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
            		matrix[rowIndex][columnIndex] = at(rowIndex, columnIndex);

            for(size_t index=0; index<size.rowCount; ++index)
            	matrix[index][size.columnCount + index] = 1;

            for(size_t mainIndex=0; mainIndex<size.rowCount; ++mainIndex){
                if(matrix[mainIndex][mainIndex] == 0.0){
                    for(size_t rowIndex=mainIndex+1; rowIndex<size.rowCount; ++rowIndex)
                    	if((matrix[mainIndex][rowIndex] > 0.0)||(matrix[mainIndex][rowIndex] < -0.0)){
                            const double multyplicator = 1.0/matrix[rowIndex][mainIndex];
                            for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                                matrix[mainIndex][columnIndex] += multyplicator*matrix[rowIndex][columnIndex];
                            break;
                        }
                }else if(matrix[mainIndex][mainIndex] != 1.0){
                    const double multyplicator = 1.0/matrix[mainIndex][mainIndex];
                    for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                        matrix[mainIndex][columnIndex] *= multyplicator;
                }
                for(size_t rowIndex=mainIndex+1; rowIndex<size.rowCount; ++rowIndex)
                	if((matrix[rowIndex][mainIndex] > 0.0)||(matrix[rowIndex][mainIndex] < -0.0)){
                        const double multyplicator = matrix[rowIndex][mainIndex]/matrix[mainIndex][mainIndex];
                        for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                            matrix[rowIndex][columnIndex] -= multyplicator*matrix[mainIndex][columnIndex];
                    }
            }

            for(size_t mainIndex=size.rowCount-1; mainIndex>0; --mainIndex){
                for(size_t rowIndex=0; rowIndex<mainIndex; ++rowIndex){
                    if((matrix[rowIndex][mainIndex] > 0.0)||(matrix[rowIndex][mainIndex] < -0.0)){
                        const double multyplicator = matrix[rowIndex][mainIndex]/matrix[mainIndex][mainIndex];
                        for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; ++columnIndex)
                            matrix[rowIndex][columnIndex] -= multyplicator*matrix[mainIndex][columnIndex];
                    }
                }
            }
            Matrix result{size};
            for(size_t rowIndex=0; rowIndex<size.rowCount; ++rowIndex)
            	for(size_t columnIndex=0; columnIndex<size.columnCount; ++columnIndex)
            		result[rowIndex][columnIndex] = matrix.at(rowIndex, columnIndex+size.columnCount);
            return result;
        }
    }else return Matrix{};
}

std::string Matrix::toString()const{
    std::stringstream ss;
    for(size_t index=0; index<values.size(); ++index){
        ss << values[index] << "\t";
        if(index % size.columnCount == size.columnCount-1){
			ss.seekp(-1, std::ios_base::end);
			ss << "\n";
        }
    }
    return ss.str();
}

#endif // MATRIX_H_INCLUDED
