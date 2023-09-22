#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

class MatrixSize{
    public:
        size_t rowCount;
        size_t columnCount;

        MatrixSize();
        MatrixSize(const std::pair<size_t, size_t> &size);
        MatrixSize(const MatrixSize &size);
        MatrixSize(const size_t &rowCount, const size_t &columnCount);
        const MatrixSize T()const;
        const bool isElement()const;
        const bool isSimmetrical()const;
};

class Matrix{
    public:
        MatrixSize size;
        std::vector<std::vector<double>> values;

        Matrix();
        Matrix(const size_t &rawCount, const size_t &columnCount, double defaultValue=0);
        Matrix(const MatrixSize &size, double defaultValue=0);
        Matrix(const std::initializer_list<std::initializer_list<double>> &values);
        Matrix(const std::vector<std::vector<double>> &values);
        Matrix(const Matrix& matrix);
        const Matrix T()const;
        const Matrix operator+(const Matrix &matrix)const;
        const Matrix operator-(const Matrix &matrix)const;
        const Matrix operator*(const double &multyplier)const;
        const Matrix operator*(const Matrix &matrix)const;
        const Matrix algebraicMinor(const size_t &rowIndex, const size_t &columnIndex)const;
        const double det()const;
        const Matrix inverse()const;
        const std::string  toString()const;
};

#endif // MATRIX_H_INCLUDED
