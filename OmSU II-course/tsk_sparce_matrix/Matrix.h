#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

class Matrix{
    public:
        std::size_t rawCount;
        std::size_t columnCount;
        std::vector<std::vector<double>> values;

        Matrix(const std::size_t &rawCount, const std::size_t &columnCount, std::vector<std::vector<double>> &values);
        Matrix(const std::size_t &rawCount, const std::size_t &columnCount, double defaultValue);
        Matrix(const std::initializer_list<std::initializer_list<double>> &values);
        Matrix(const Matrix& matrix);
        void operator=(const Matrix &matrix);
        const Matrix T()const;
        const Matrix operator+(const Matrix &matrix)const;
        const Matrix operator-(const Matrix &matrix)const;
        const Matrix operator*(const double &multyplier)const;
        const Matrix operator*(const Matrix &matrix)const;
        const std::string  toString()const;
};

#endif // MATRIX_H_INCLUDED
