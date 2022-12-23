#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

template<class T, class Inheritor>
class Array{
    public:
        Array(vector<T> data = {}){
            for(T element:data)
                this->data.insert(this->data.end(), element);
        }
        Inheritor operator+(const Inheritor array)const{return static_cast<Inheritor*>(this)->operator+(array);}
        bool hasElement(T value){
            for(T element:data)
                if(element == value)
                    return true;
            return false;
        }
        virtual void foreach(){};
        void add(T element){
            if(!hasElement(element))
                data.insert(data.end(), element);
        }
        T operator[](const int index)const{return data[index];}
        int size(){return data.size();}
    protected:
        vector<T> data;
};

template <class T>
class sortArray: public Array<T, sortArray<T>>{
    public:
        sortArray(vector<T> data = {}){
            for(T element:data)
                this->data.insert(this->data.end(), element);
        }
        sortArray<T> operator+(const sortArray<T> array)const{
            sortArray<T> result(this->data);
            for(T element:array.data)
                result.add(element);
            return result;
        }
        void foreach(){
            int size = this->data.size();
            for(int i=0; i<size; i++)
                for(int j=i-1; j>=0; j--)
                    if(this->data[j+1] < this->data[j]){
                        T memory = this->data[j];
                        this->data[j] = this->data[j+1];
                        this->data[j+1] = memory;
                    }else break;
        }
};

template <class T>
class xorArray: public Array<T, xorArray<T>>{
    public:
        xorArray(vector<T> data = {}){
            for(T element:data)
                this->data.insert(this->data.end(), element);
        }
        xorArray<T> operator+(const xorArray<T> array)const{
            xorArray<T> result;
            for(T element:this->data)
                if(!array.hasElement(element))
                    result.add(element);
            for(T element:array.data)
                if(!this->hasElement(element))
                    result.add(element);
            return result;
        }
        void foreach(){
            int size = this->data.size();
            for(int i=0; i<size; i++)
                this->data[i] = sqrt(this->data[i]);
        }
};

vector<double> getValues(){
    vector<double> values;
    while(true){
        string input;
        cin >> input;
        if(input == "end")
            break;
        else values.insert(values.end(), stod(input));
    }
    return values;
}

int arrayInterface(){
    while(true){
        string input;
        cout << "Enter mode(xor/sort) and operation:\n";
        cin >> input;
        if(input == "exit")
            return 0;
        else if(input == "reboot")
            return 1;
        if(input == "xor"){
            xorArray<double> arr1(getValues());
            cin >> input;
            if(input == "+"){
                xorArray<double> arr2(getValues());
                xorArray<double> result = arr1 + arr2;
                cout << "Union: ";
                for(int i=0; i<result.size(); i++)
                    cout << result[i] << ' ';
                cout << endl;
            }
            if(input == "foreach"){
                arr1.foreach();
                cout << "Result of by element operation: ";
                for(int i=0; i<arr1.size(); i++)
                    cout << arr1[i] << ' ';
                cout << endl;
            }
        }else if(input == "sort"){
            sortArray<double> arr1(getValues());
            cin >> input;
            if(input == "+"){
                sortArray<double> arr2(getValues());
                sortArray<double> result = arr1 + arr2;
                cout << "Union: ";
                for(int i=0; i<result.size(); i++)
                    cout << result[i] << ' ';
                cout << endl;
            }
            if(input == "foreach"){
                arr1.foreach();
                cout << "Result of by element operation: ";
                for(int i=0; i<arr1.size(); i++)
                    cout << arr1[i] << ' ';
                cout << endl;
            }
        }
    }
    return 1;
}

int executeCommand(string command){
    if(command == "exit") return 0;
    else if(command == "start")
        return arrayInterface();
    return 1;
}

void userInterface(){
    cout << "This program works with ArrayClasses:\n";
    string command;
    while(true){
        cout << "Enter operation: ";
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

