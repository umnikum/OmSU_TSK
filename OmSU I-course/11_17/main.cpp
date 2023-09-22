#include <string>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

class Dictionary{
    public:
        map<char, int> counters;
        int count(const char &c);
        string print()const;
};

int Dictionary::count(const char &c){
    if(counters.count(c) == 0){
        counters[c] = 1;
    }else{
        counters.at(c)++;
    }
    return counters.at(c);
}

string Dictionary::print()const{
    stringstream ss;
    for(auto pair:counters)
        ss << pair.first << " - " << pair.second << endl;
    return ss.str();
}

int main()
{
    Dictionary d;
    string s;
    cout << "Enter the phrase to count letters:\n";
    cin >> s;
    for(char c:s)
        if((c - 'a' >= 0)&&(c - 'z' <= 0)) // Condition to count letters
            d.count(c);
    cout << d.print();
    return 0;
}

