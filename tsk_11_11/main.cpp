#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Money {
    public:
        Money(vector<int> amounts, double pennys); //general entry method
        Money(double totalAmount);          //contains method to calculate bills and coins needed to get totalAmount
        Money& operator=(const Money& arg); //have no idea if it's still usefull, but it was at one point
        double totalAmount() const;         //double result of all valuables
        void display() const;               //print directly to screen with all coins and bills
        string toString()const;             //usefull function to get general info about calss without displaying it on screen
        friend bool operator==(const Money& arg1, const Money& arg2); //logical operators works fine in all ifs
        friend bool operator<(const Money& arg1 , const Money& arg2);
        friend bool operator>(const Money& arg1, const Money& arg2);
        Money& operator+(const Money& arg)const;    //general operators
        Money& operator-(const Money& arg)const;    //can lead in negative values, use with caution!
        Money& operator*(const double arg)const;
        Money& operator/(const double arg)const;
    private:
        const vector<int> nominals = {1, 2, 5, 10, 50, 100, 500, 1000, 5000}; // can put 200 and 2000 if needed
        vector<int> amounts;
        double pennys;
        void trim();                        //clear useless data from class
};

void Money::trim(){
    while((amounts.size()>1)&&(amounts[amounts.size()-1] == 0))
        amounts.pop_back();
    if(pennys >= 1.0){
        amounts[0] += (int)pennys;
        pennys -= ((int)pennys);
    }
}

Money::Money(vector<int> amounts, double pennys=0.0){   //{1, 0, 0} would revert to {0, 0, 1} amounts to get same indexes with nominals
    for(int amount:amounts)
        this->amounts.insert(this->amounts.begin(), amount);
    this->pennys = pennys;
}

Money::Money(double totalAmount=0.0){
    double leftover = totalAmount;
    for(int i=nominals.size()-1; i>=0; i--){
        int count = leftover/nominals[i];
        this->amounts.insert(this->amounts.begin(), count);
        if(count > 0)
            leftover -= count * nominals[i];
    }
    this->pennys = leftover;
    trim();
}

Money& Money::operator=(const Money& arg){
    Money* money = new Money(0.0);
    money->pennys = arg.pennys;
    money->amounts.clear();
    for(int amount:arg.amounts)
        money->amounts.insert(money->amounts.end(), amount);
    return *money;
}

double Money::totalAmount()const{
    double result = 0.0;
    for(int i=amounts.size()-1; i>=0; i--)
        result += amounts[i] * nominals[i];
    result += pennys;
    return result;
}

void Money::display()const{
    for(int i=amounts.size()-1; i>=0; i--){
        if(amounts[i] != 0){
            if(nominals[i] <= nominals[3])
                cout << "coins ";
            else cout << "bills ";
            cout << nominals[i] << "₽: " << amounts[i] << " ";
        }
    }
    cout << "Pennys: " << pennys << "\tTotal: " << totalAmount() << "₽";
}

string Money::toString()const{
    stringstream stream;
    stream << totalAmount() << "₽";
    return stream.str();
}

bool operator==(const Money& arg1, const Money& arg2){
    if(arg1.amounts.size() != arg2.amounts.size())
        return false;
    else if(arg1.pennys != arg2.pennys)
        return false;
    else for(int i=0; i<arg1.amounts.size(); i++)
            if(arg1.amounts[i] != arg2.amounts[i])
                return false;
    return true;
}

bool operator<(const Money& arg1, const Money& arg2){
    if(arg1.totalAmount() < arg2.totalAmount())
        return true;
    else return false;
}

bool operator>(const Money& arg1, const Money& arg2){
    if(arg1.totalAmount() > arg2.totalAmount())
        return true;
    else return false;
}

Money& Money::operator+(const Money& arg)const{
    Money* money = new Money(totalAmount() + arg.totalAmount());
    money->trim();
    return *money;
}

Money& Money::operator-(const Money& arg)const{
    Money* money = new Money(totalAmount() - arg.totalAmount());
    money->trim();
    return *money;
}

Money& Money::operator*(const double arg)const{
    Money* money = new Money(totalAmount()*arg);
    money->trim();
    return *money;
}

Money& Money::operator/(const double arg)const{
    Money* money = new Money(totalAmount()/arg);
    money->trim();
    return *money;
}

Money& readMoney() {                //Function to get class by general amounts of bills and coins from console
    string input;
    vector<int> values;
    double pennys;
    while(true){
        cin >> input;
        if(input == ".")
            break;
        values.insert(values.end(), stoi(input));
    }
    cin >> pennys;
    Money* money = new Money(values, pennys);
    return *money;
}

void interface(){                   //console UI to use multiple operations with classes
    string input;
    while(input != "exit"){
        cout << "Enter next operation:\n";
        Money money1 = readMoney();
        cin >> input;
        if(input == "=="){
            Money money2 = readMoney();
            cout << money1.toString();
            if(money1==money2)
                cout << "==";
            else cout << "!=";
            cout << money2.toString();
        }else if(input == ">"){
            Money money2 = readMoney();
            cout << money1.toString();
            if(money1>money2)
                cout << ">";
            else cout << "<=";
            cout << money2.toString();
        }else if(input == "<"){
            Money money2 = readMoney();
            cout << money1.toString();
            if(money1<money2)
                cout << "<n";
            else cout << ">=";
            cout << money2.toString();
        }else if(input == "+"){
            Money money2 = readMoney();
            Money result = money1 + money2;
            cout << money1.toString() << " + " << money2.toString() << " = " << result.toString() << endl;
            result.display();
        }else if(input == "-"){
            Money money2 = readMoney();
            Money result = money1 - money2;
            cout << money1.toString() << " + " << money2.toString() << " = " << result.toString() << endl;
            result.display();
        }else if(input == "*"){
            double multiplier;
            cin >> multiplier;
            Money result = money1 * multiplier;
            cout << multiplier << " * " << money1.toString() << " = " << result.toString() << endl;
            result.display();
        }else if(input == "/"){
            double denominator;
            cin >> denominator;
            Money result = money1 / denominator;
            cout << money1.toString() << "/" << denominator << " = " << result.toString() << endl;
            result.display();
        }
        cout << "\nWant to continue/exit?\n";
        cin >> input;
    }
}

int main() {
    interface();
    return 0;
}
