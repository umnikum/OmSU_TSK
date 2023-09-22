#ifndef SPARSEMATRIX_H_INCLUDED
#define SPARSEMATRIX_H_INCLUDED
#include "Matrix.h"

class SparseMatrix{
    public:
        static constexpr double accuracy = 0.001;
        MatrixSize size;
        std::vector<double> values;
        std::vector<size_t> columnIndexes, rowIndexRanges;

        SparseMatrix();
        SparseMatrix(const MatrixSize &size, std::vector<double> &values, std::vector<size_t> &columnIndexes, std::vector<size_t> &rowIndexRanges);
        SparseMatrix(const MatrixSize &size, std::map<size_t, std::map<size_t, double>> &values);
        SparseMatrix(const SparseMatrix &sparseMatrix);
        SparseMatrix(const Matrix &matrix);
        void operator=(const SparseMatrix &sparseMatrix);
        void operator=(const SparseMatrix &&sparseMatrix);
        void cut();
        void validate();
        const size_t getRowIndex(const size_t &valuesIndex)const;
        const double operator()(const size_t &rowIndex, const size_t &columnIndex)const;
        const SparseMatrix T()const;
        const SparseMatrix operator+(const Matrix &matrix)const;
        const SparseMatrix operator-(const Matrix &matrix)const;
        const SparseMatrix operator*(const Matrix &matrix)const;
        const SparseMatrix operator*(const double &multyplier)const;
        const SparseMatrix operator+(const SparseMatrix &sparseMatrix)const;
        const SparseMatrix operator-(const SparseMatrix &sparseMatrix)const;
        const SparseMatrix operator*(const SparseMatrix &sparseMatrix)const;
        const std::string  toString()const;
};

#endif // SPARSEMATRIX_H_INCLUDED
