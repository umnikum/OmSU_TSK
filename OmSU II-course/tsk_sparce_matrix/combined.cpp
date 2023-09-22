#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

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

class SparseMatrix{
    public:
        static constexpr double accuracy = 0.001;
        std::size_t rawCount;
        std::size_t columnCount;
        std::map<std::size_t, std::map<std:: size_t, double>> rawValues;

        SparseMatrix(const std::size_t &rawCount, const std::size_t &columnCount, std::map<std::size_t, std::map<std::size_t, double>> &values);
        SparseMatrix(const SparseMatrix& dolledMatrix);
        SparseMatrix(const Matrix& matrix);
        void operator=(const SparseMatrix &dolledMatrix);
        const std::pair<std::vector<std::size_t>, std::vector<std::size_t>> getIndexes()const;
        void cut();
        const SparseMatrix T()const;
        const SparseMatrix operator+(const Matrix &matrix)const;
        const SparseMatrix operator-(const Matrix &matrix)const;
        const SparseMatrix operator*(const Matrix &matrix)const;
        const SparseMatrix operator*(const double &multyplier)const;
        const SparseMatrix operator+(const SparseMatrix &dolledMatrix)const;
        const SparseMatrix operator-(const SparseMatrix &dolledMatrix)const;
        const SparseMatrix operator*(const SparseMatrix &dolledMatrix)const;
        const std::string  toString()const;
};

Matrix::Matrix(const std::size_t &rawCount, const std::size_t &columnCount, std::vector<std::vector<double>> &values){
    this->rawCount = rawCount;
    this->columnCount = columnCount;
    this->values = values;
}

Matrix::Matrix(const std::size_t &rawCount, const std::size_t &columnCount, double defaultValue){
    this->rawCount = rawCount;
    this->columnCount = columnCount;
    for(std::size_t rawIndex=0; rawIndex<rawCount; rawIndex++){
        std::vector<double> raw(columnCount);
        values.push_back(raw);
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++)
            raw.push_back(defaultValue);
    }
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> &values){
    for(auto raw:values){
        this->rawCount++;
        std::vector<double> newRaw = raw;
        this->values.push_back(newRaw);
    }
    this->columnCount = this->values[0].size();
}

Matrix::Matrix(const Matrix& matrix){
    this->rawCount = matrix.rawCount;
    this->columnCount = matrix.columnCount;
    std::vector<std::vector<double>> newValues(matrix.values);
    this->values = newValues;
}

void Matrix::operator=(const Matrix& matrix){
    this->rawCount = matrix.rawCount;
    this->columnCount = matrix.columnCount;
    this->values = matrix.values;
}

const Matrix Matrix::T()const{
    std::vector<std::vector<double>> values;

    for(std::size_t rawIndex=0; rawIndex<columnCount; rawIndex++){
        std::vector<double> raw(this->rawCount);
        values.push_back(raw);
        for(std::size_t columnIndex=0; columnIndex<rawCount; columnIndex++)
            raw.push_back(this->values[columnIndex][rawIndex]);
    }

    Matrix result(this->columnCount, this->rawCount, values);
    return result;
}

const Matrix Matrix::operator+(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->rawCount == matrix.rawCount)&&(this->columnCount == matrix.columnCount))
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
            for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
                values[rawIndex][columnIndex] += matrix.values[rawIndex][columnIndex];

    Matrix result(this->rawCount, this->columnCount, values);
    return result;
}

const Matrix Matrix::operator-(const Matrix &matrix)const{
    std::vector<std::vector<double>> values(this->values);

    if((this->rawCount == matrix.rawCount)&&(this->columnCount == matrix.columnCount))
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
            for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
                values[rawIndex][columnIndex] -= matrix.values[rawIndex][columnIndex];

    Matrix result(this->rawCount, this->columnCount, values);
    return result;
}

const Matrix Matrix::operator*(const double &multyplier)const{
    std::vector<std::vector<double>> values(this->values);

    for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++)
        for(std::size_t columnIndex=0; columnIndex<this->columnCount; columnIndex++)
            values[rawIndex][columnIndex] *= multyplier;

    Matrix result(this->rawCount, this->columnCount, values);
    return result;
}

const Matrix Matrix::operator*(const Matrix &matrix)const{
    std::vector<std::vector<double>> values;

    if(this->rawCount == matrix.columnCount)
        for(std::size_t rawIndex=0; rawIndex<this->rawCount; rawIndex++){
	    std::vector<double> raw(matrix.columnCount);
            values.push_back(raw);
            for(std::size_t columnIndex=0; columnIndex<matrix.columnCount; columnIndex++){
                double summ = 0.0;
                for(std::size_t sumIndex=0; sumIndex<this->columnCount; sumIndex++)
                    summ +=this->values[rawIndex][sumIndex]*matrix.values[sumIndex][columnIndex];
                values[rawIndex].push_back(summ);
            }
        }

    Matrix result(this->rawCount, matrix.columnCount, values);
    return result;
}

const std::string Matrix::toString()const{
    std::stringstream ss;
    for(std::vector<double> raw:values){
        for(double value:raw)
            ss << value << "\t";
        ss.seekp(-1, std::ios_base::end);
        ss << "\n";
    }
    return ss.str();
}

SparseMatrix::SparseMatrix(const std::size_t &rawCount, const std::size_t &columnCount, std::map<std::size_t, std::map<std::size_t, double>> &values){
    this->rawCount = rawCount;
    this->columnCount = columnCount;
    this->rawValues = values;
}

SparseMatrix::SparseMatrix(const SparseMatrix &dolledMatrix){
    this->rawCount = dolledMatrix.rawCount;
    this->columnCount = dolledMatrix.columnCount;
    std::map<std::size_t, std::map<std:: size_t, double>> values(dolledMatrix.rawValues);
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

void SparseMatrix::operator=(const SparseMatrix &dolledMatrix){
    this->rawCount = dolledMatrix.rawCount;
    this->columnCount = dolledMatrix.columnCount;
    this->rawValues = dolledMatrix.rawValues;
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

const SparseMatrix SparseMatrix::operator+(const SparseMatrix &dolledMatrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if((this->rawCount == dolledMatrix.rawCount)&&(this->columnCount == dolledMatrix.columnCount)){
        for(auto rawPair:this->rawValues){
            std::map<std::size_t, double> newRaw;
            values.emplace(rawPair.first, newRaw);
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] = valuePair.second;
        }
        for(auto rawPair:dolledMatrix.rawValues){
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

const SparseMatrix SparseMatrix::operator-(const SparseMatrix &dolledMatrix)const{
    std::map<std::size_t, std::map<std:: size_t, double>> values;

    if((this->rawCount == dolledMatrix.rawCount)&&(this->columnCount == dolledMatrix.columnCount)){
        for(auto rawPair:this->rawValues){
            std::map<std::size_t, double> newRaw;
            values.emplace(rawPair.first, newRaw);
            for(auto valuePair:rawPair.second)
                values[rawPair.first][valuePair.first] = valuePair.second;

        }
        for(auto rawPair:dolledMatrix.rawValues){
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

const SparseMatrix SparseMatrix::operator*(const SparseMatrix &dolledMatrix)const{
    std::map<std::size_t, std::map<std::size_t, double>> values;
    SparseMatrix tMatrix = dolledMatrix.T();

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

    SparseMatrix result(this->rawCount, dolledMatrix.columnCount, values);
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

std::vector<std::string> split(const std::string &line){
    std::vector<std::string> args;
    auto previousIt = line.begin();
    auto nextIt = previousIt;
    do{
        nextIt = std::find(previousIt, line.end(), ' ');
        std::string arg = line.substr(previousIt-line.begin(), nextIt-previousIt);
        if(!arg.empty())
            args.push_back(arg);
        previousIt = nextIt+1;
    }while(nextIt != line.end());
    return args;
}

class UI{
    public:
        std::map<std::string, Matrix> matrixes;
        std::map<std::string, SparseMatrix> dolledMatrixes;
        const int executeCommand(const std::string &command);
    private:
        void readMatrix(const std::string &name);
        void readSparseMatrix(const std::string &name);
        bool isSparse(const std::string &name)const;
        void readCommand(const std::vector<std::string> &args);
        void printCommand(const std::vector<std::string> &args)const;
        void countCommand(const std::vector<std::string> &args);
};


void UI::readMatrix(const std::string &name){
    std::size_t rawCount, columnCount;
    std::vector<std::vector<double>> values;
    std::cout << "Enter matrix dimentions: ";
    std::cin >> rawCount >> columnCount;

    std::cout << "Values:\n";
    for(std::size_t rawIndex=0; rawIndex<rawCount; rawIndex++){
        std::vector<double> raw;
        values.push_back(raw);
        for(std::size_t columnIndex=0; columnIndex<columnCount; columnIndex++){
            double value;
            std::cin >> value;
            values[rawIndex].push_back(value);
        }
    }
    Matrix matrix(rawCount, columnCount, values);
    matrixes.emplace(name, matrix);
}

void UI::readSparseMatrix(const std::string &name){
    std::size_t rawCount, columnCount, elementCount;
    std::map<std::size_t, std::map<std::size_t, double>> values;
    std::cout << "Enter matrix dimentions and amount of meaningfull elements: ";
    std::cin >> rawCount >> columnCount >> elementCount;

    std::cout << "Enter triplets of raw/column indexes and value for each element:\n";
    for(std::size_t element=0; element<elementCount; element++){
        std::size_t rawIndex, columnIndex;
        double value;
        std:: cin >> rawIndex >> columnIndex >> value;
        if(values.count(rawIndex) == 0){
            std::map<std::size_t, double> raw;
            values.emplace(rawIndex, raw);
        }
        values[rawIndex][columnIndex] = value;
    }
    SparseMatrix matrix(rawCount, columnCount, values);
    dolledMatrixes.emplace(name, matrix);
}

bool UI::isSparse(const std::string &name)const{
    return dolledMatrixes.count(name) > 0;
}

void UI::readCommand(const std::vector<std::string> &args){
    if(args[1].compare("matrix") == 0)
        this->readMatrix(args[2]);
    else if(args[1].compare("dolled") == 0)
        this->readSparseMatrix(args[2]);
}

void UI::printCommand(const std::vector<std::string> &args)const{
    std::string name = args[1];
    if(isSparse(name))
        std::cout << this->dolledMatrixes.at(name).toString();
    else if(this->matrixes.count(name) > 0)
        std::cout << this->matrixes.at(name).toString();
    else std::cout << name << " - not found\n";
}

void UI::countCommand(const std::vector<std::string> &args){
    if(args[2].compare("=") == 0){
        SparseMatrix result;
        if(isSparse(args[3])){
            SparseMatrix matrix(dolledMatrixes.at(args[3]));
        }else{
            SparseMatrix matrix(matrixes.at(args[3]));
        }
        if(dolledMatrixes.count(args[1]) == 0)
            dolledMatrixes.emplace(args[1], result);
        else dolledMatrixes[args[1]] = result;
    }
}

const int UI::executeCommand(const std::string &command){
    std::vector<std::string> args = split(command);
    if(!args.empty()){
        std::string commandName = args[0];
        if(commandName.compare("exit") == 0)
            return 0;
        if(commandName.compare("read") == 0){
            readCommand(args);
            return 2;
        }
        if(commandName.compare("print") == 0){
            printCommand(args);
            return 3;
        }
        if(commandName.compare("count") == 0){
            countCommand(args);
            return 4;
        }
    }
    return 1;
}

int main()
{
    UI ui;
    std::string command;
    do{
        getline(std::cin, command);
        if(ui.executeCommand(command) == 0)
            break;
    }while(true);
    return 0;
}
