#include <iostream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

int getIndex(int size, int x, int y){
    return size*y + x;}

bool isMember(int size, int x, int y){
    return ((x>=y)&&(x<=size-y))||((x<=y)&&(x>=size-y));}

void printMatrix(vector<int> values){
    int size = sqrt(values.size());
    cout << "Matrix:\n";
    for(int y=0; y<size; y++){
        for(int x=0; x<size; x++){
            cout << values[getIndex(size, x, y)];
        }
        cout << "\n";
    }
    cout << "End of matrix. ";
}

vector<int> enterMatrix(){
    vector<int> values;
    cout << "Enter matrix here (type end to finish entery): ";
    string input;
    while(true){
        cin >> input;
        if(input == "end")
            break;
        int value = stoi(input);
        values.insert(values.end(), value);
    }
    return values;
}
    
int searchMax(vector<int> values){
    int max = values[0], size = sqrt(values.size());
    for(int y=0; y<size; y++)
        for(int x=0; x<size; x++){
            if(isMember(size, x, y))
                if(values[getIndex(size, x, y)] > max)
                    max = values[getIndex(size, x, y)];
        }
    return max;
}

int executeCommand(string command){
    if(command == "exit") return 0;
    if(command == "search"){
        vector<int> values = enterMatrix();
        printMatrix(values);
        int max = searchMax(values);
        cout << "Max number in group: " << max << "\n";
        values.clear();
    }
    return 1;
}

void userInterface(){
    cout << "This program is searching max among specific matrix elements:\n";
    string command;
    while(true){
        cout << "Enter command: ";
        cin >> command;
        int executeCode = executeCommand(command);
        if(executeCode == 0)
            break;
    }
    
}

int main()
{
    userInterface();
    return 0;
}
