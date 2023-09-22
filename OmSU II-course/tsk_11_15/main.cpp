#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Bitstring{
    public:
        Bitstring(unsigned long part1, unsigned long part2);
        Bitstring operator&(const Bitstring& bitstring)const;
        Bitstring operator|(const Bitstring& bitstring)const;
        Bitstring operator~()const;
        Bitstring operator^(const Bitstring& bitstring)const;
        Bitstring operator<<(const int steps)const;
        Bitstring operator>>(const int steps)const;
        string str()const;
    protected:
        unsigned long part1;
        unsigned long part2;
    private:
        vector<unsigned long> bits()const;
};

Bitstring::Bitstring(unsigned long part1, unsigned long part2){
    this->part1 = part1;
    this->part2 = part2;
}

vector<unsigned long> Bitstring::bits()const{
    vector<unsigned long> bits;
    unsigned long bit = 1;
    for(short i=0; i<32; i++){
        bits.insert(bits.end(), bit);
        bit<<=1;
    }
    return bits;
}

Bitstring Bitstring::operator&(const Bitstring& bitstring)const{
    return Bitstring(part1&(bitstring.part1), part2&(bitstring.part2));}

Bitstring Bitstring::operator|(const Bitstring& bitstring)const{
    return Bitstring(part1|(bitstring.part1), part2|(bitstring.part2));}

Bitstring Bitstring::operator~()const{
    return Bitstring(~part1, ~part2);}

Bitstring Bitstring::operator^(const Bitstring& bitstring)const{
    return Bitstring(part1^(bitstring.part1), part2^(bitstring.part2));}

Bitstring Bitstring::operator<<(const int steps)const{
    return Bitstring((part1<<steps)|(part2>>(32-steps)), part2<<steps);}

Bitstring Bitstring::operator>>(const int steps)const{
    return Bitstring(part1>>steps, (part2>>steps)|(part1<<(32-steps)));}

string Bitstring::str()const{
    bool nonzero = false;
    stringstream stream;
    vector<unsigned long> bits = this->bits();
    for(short i=31; i>=0; i--){
        if(!nonzero){
            if((bits[i]&part1)){
                nonzero = true;
                stream << 1;
            }
        }else (bits[i]&part1) ? stream << 1 : stream << 0;
    }
    for(short i=31; i>=0; i--){
        if(!nonzero){
            if((bits[i]&part2)){
                nonzero = true;
                stream << 1;
            }
        }else (bits[i]&part2) ? stream << 1 : stream << 0;
    }
    if(!nonzero)
        stream << 0;
    return stream.str();
}

int ariphmeticInterface(string input){
    string result;
    unsigned long part1, part2;
    if(input == "~"){
        cin >> part1 >> part2;
        Bitstring bs(part1, part2);
        result = (~bs).str();
    }else{
        part1 = stol(input);
        cin >> part2;
        Bitstring bs1(part1, part2);
        cin >> input;
        if(input == "&"){
            cin >> part1 >> part2;
            Bitstring bs2(part1, part2);
            result = (bs1&bs2).str();
        }else   if(input == "|"){
            cin >> part1 >> part2;
            Bitstring bs2(part1, part2);
            result = (bs1|bs2).str();
        }else   if(input == "^"){
            cin >> part1 >> part2;
            Bitstring bs2(part1, part2);
            result = (bs1^bs2).str();
        }else   if(input == "<<"){
            int steps;
            cin >> steps;
            result = (bs1<<steps).str();
        }else   if(input == ">>"){
            int steps;
            cin >> steps;
            result = (bs1>>steps).str();
        }
    }
    cout << " = " << result << endl;
    return 1;
}

int executeCommand(string command){
    if(command == "exit") return 0;
    else return ariphmeticInterface(command);
    //return 1;
}

void userInterface(){
    cout << "This program works with bitstrings:\n";
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

