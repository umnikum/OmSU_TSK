#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

double searchMax(const vector<double> array){
    double max = array[0];
    for(double element:array)
        if(element > max)
            max = element;
    return max;
}

double searchMin(const vector<double> array){
    double min = array[0];
    for(double element:array)
        if(element < min)
            min = element;
    return min;
}

vector<double> append(const vector<double> arrayA, const vector<double> arrayB){
    vector<double> result;
    for(double element:arrayA)
        result.insert(result.end(), element);
    for(double element:arrayB)
        result.insert(result.end(), element);
    return result;
}

double function(const vector<double> arrayA, const vector<double> arrayB, const vector<double> arrayC){
    double minA = searchMin(arrayA),
           minB = searchMin(arrayB),
           minC = searchMin(arrayC),
           maxA = searchMax(arrayA),
           maxB = searchMax(arrayB),
           maxBC = searchMax(append(arrayB, arrayC));
    if(minA > maxB)
        return (minB/maxA + minC/maxBC);
    else return (maxBC + minC);
}

vector<double> enterArray(string input){
    vector<double> array;
    istringstream splitter;
    splitter.str(input);
    for (string line; getline(splitter, line, ' ');)
        array.insert(array.end(), stod(line));
    return array;
}

void userInterface(){
    string input;
    while(true){
        getline(cin, input);
        if(input == "exit")
            break;
        vector<double> arrayA = enterArray(input), arrayB, arrayC;
        getline(cin, input);
        arrayB = enterArray(input);
        getline(cin, input);
        arrayC = enterArray(input);
        cout << "Result: " << function(arrayA, arrayB, arrayC) << endl;
    }
}

int main()
{
    userInterface();
    return 0;
}

