#ifndef SPARSEMATRIX_H_INCLUDED
#define SPARSEMATRIX_H_INCLUDED
#include "Matrix.h"

class SparseMatrix{
    public:
        static constexpr double accuracy = 0.001;
        std::size_t rawCount;
        std::size_t columnCount;
        std::map<std::size_t, std::map<std:: size_t, double>> rawValues;

        SparseMatrix(const std::size_t &rawCount, const std::size_t &columnCount, std::map<std::size_t, std::map<std::size_t, double>> &values);
        SparseMatrix(const SparseMatrix& dolledMatrix);
	SparseMatrix(const Matrix& matrix);
        void operator=(const SparseMatrix &sparseMatrix);
        const std::pair<std::vector<std::size_t>, std::vector<std::size_t>> getIndexes()const;
        void cut();
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
