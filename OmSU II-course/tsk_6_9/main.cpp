#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class dictionary{
    public:
        dictionary(string str);
        int length() const;
        short value(const char key) const;
        char key(const short value) const;
    protected:
        string data;
};

dictionary::dictionary(string str){
    data = str;}

int dictionary::length() const{
    return data.size();}

short dictionary::value(const char key) const{
    for(int i=0; i<data.size(); i++)
        if(data[i] == key)
            return i;
    return -1;
}

char dictionary::key(const short value) const{
    return data[value];}

class encription{
    public:
        encription(dictionary& set, string code);
        string encript(string word) const;
        string decript(string code) const;
    protected:
        dictionary* set;
        vector<short> key;
    private:
        int numberLength(const int number) const;
        vector<short> toNumericCode(string code) const;
        string toCode(vector<short> numericCode) const;
};

int encription::numberLength(const int number) const{
    int length = 0, digits = number;
    do{
        length++;
        digits /= 10;
    }while(digits > 0);
    return length;
}

vector<short> encription::toNumericCode(string code) const{
    vector<short> numericCode;
    int cycleLength = numberLength((set->length()) - 1), cycle = 0;
    short digit = 0;
    for(char letter:code){
        digit = digit*10 + (letter-'0');
        cycle++;
        if(cycle == cycleLength){
            cycle = 0;
            numericCode.insert(numericCode.end(), digit);
            digit = 0;
        }
    }
    return numericCode;
}

string encription::toCode(vector<short> numericCode) const{
    stringstream code;
    int cycleLength = numberLength((set->length()) - 1);
    for(short digit:numericCode){
        if(numberLength(digit) < cycleLength)
            for(int i=numberLength(digit); i<cycleLength; i++)
                code << '0';
        code << digit;
    }
    return code.str();
}

encription::encription(dictionary& set, string code){
    this->set = &set;
    key = toNumericCode(code);
}

string encription::encript(string word) const{
    vector<short> code;
    for(char letter:word)
        code.insert(code.end(), set->value(letter));
    int keyLength = key.size(), setLength = set->length();
    for(int i=0; i<code.size(); i++)
        code[i] = (code[i] + key[(i%keyLength)]) % setLength;
    return toCode(code);
}

string encription::decript(string code) const{
    vector<short> numericCode = toNumericCode(code);
    int keyLength = key.size(), setLength = set->length();
    for(int i=0; i<numericCode.size(); i++)
        numericCode[i] = (numericCode[i] - key[(i%keyLength)] + setLength) % setLength;
    stringstream stream;
    for(short value:numericCode)
        stream << set->key(value);
    return stream.str();
}

int encriptionInterface(){
    string input;
    cout << "Enter dictionary: ";
    cin >> input;
    dictionary set(input);
    cout << "Enter key: ";
    cin >> input;
    encription encr(set, input);
    cout << "Use commands << to encript and >> to decript words\n";
    while(true){
        cout << "Enter command: ";
        cin >> input;
        if(input == "exit")
            return 0;
        else{
            if(input == "<<"){
                cin >> input;
                cout << "Code: " << encr.encript(input) << endl;
            }
            if(input == ">>"){
                cin >> input;
                cout << "Translation: " << encr.decript(input) << endl;
            }
        }
    }
}

int executeCommand(string command){
    if(command == "exit") return 0;
    if(command == "encript") return encriptionInterface();
    return 1;
}

void userInterface(){
    cout << "This program is encripting words:\n";
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
