#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<string> split(const string &line, const char &delimeter = ' '){
    vector<string> args;
    auto previousIt = line.begin();
    auto nextIt = previousIt;
    do{
        nextIt = find(previousIt, line.end(), delimeter);
       string arg = line.substr(previousIt-line.begin(), nextIt-previousIt);
        if(!arg.empty())
            args.push_back(arg);
        previousIt = nextIt+1;
    }while(nextIt != line.end());
    return args;
}

bool isLeapYear(const int &year){
    return (year%400 == 0)||((year%4 == 0)&&(year%100 != 0));
}

class Date{
    public:
        int day;
        int month;
        int year;

        Date(const int &day = 1, const int &month = 1, const int &year = 1);
        Date(const string &date);

        void validate();
        bool operator==(const Date &date)const;
        //friendly bool operator<(const Date &a, const Date &b);
        //friendly bool operator>(const Date &a, const Date &b);
        string toString(const string &format = "numeric")const;
};

Date::Date(const int &day, const int &month, const int &year){
    this->day = day;
    this->month = month;
    this->year = year;
    validate();
}

Date::Date(const string &date){
    vector<string> numbers = split(date, '.');
    day = stoi(numbers[0]);
    month = stoi(numbers[1]);
    year = stoi(numbers[2]);
    validate();
}

void Date::validate(){
    switch(month){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(day > 31)
                day = 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(day > 30)
                day = 30;
            break;
        case 2:
            if(isLeapYear(year)){
                if(day > 29)
                    day = 29;
            }else{
                if(day > 28)
                    day = 28;
            }
            break;
        default:
            day = 1;
            month = 1;
    }
}

bool Date::operator==(const Date &date)const{
    return (day == date.day)&&(month == date.month)&&(year == date.year);
}

bool operator>(const Date &a, const Date &b){
    if(a == b)
        return false;
    else{
        if(b.year > a.year)
            return false;
        else{
            if(b.month > a.month)
                return false;
            else{
                return a.day > b.day;
            }
        }
    }
}

bool operator<(const Date &a, const Date &b){
    if(a == b)
        return false;
    else{
        if(b.year < a.year)
            return false;
        else{
            if(b.month < a.month)
                return false;
            else{
                return a.day < b.day;
            }
        }
    }
}

string Date::toString(const string &format)const{
    stringstream ss;
    if(format.compare("numeric") == 0){
        ss << day << '.' << month << '.' << year;
    }else if(format.compare("literal") == 0){
        string name;
        switch(month){
            case 1: name = "JAN";
                    break;
            case 2: name = "FEB";
                    break;
            case 3: name = "MAR";
                    break;
            case 4: name = "APR";
                    break;
            case 5: name = "MAY";
                    break;
            case 6: name = "JUN";
                    break;
            case 7: name = "JUL";
                    break;
            case 8: name = "AUG";
                    break;
            case 9: name = "SEN";
                    break;
            case 10: name = "OCT";
                    break;
            case 11: name = "NOV";
                    break;
            case 12: name = "DEC";
                    break;
            default: name = "JAN";
        }
        ss << day << " " << name << " " << year;
    }
    return ss.str();
}

class TechnicalData{
    public:
        string brandName;
        string model;
        int constructionYear;
        
        TechnicalData(){};
        TechnicalData(const string &brandName, const string &model, const int &constructionYear);
        
        string toString()const;
};

TechnicalData::TechnicalData(const string &brandName, const string &model, const int &constructionYear){
    this->brandName = brandName;
    this->model = model;
    this->constructionYear = constructionYear;
}

string TechnicalData::toString()const{
    stringstream ss;
    ss << brandName << " " << model << " produced in: " << constructionYear;
    return ss.str();
}

class IdentificationNumber{
    public:
        string id;
        string regionalCode;
        
        IdentificationNumber(){};
        IdentificationNumber(const string &id, const string &regionalCode);
        IdentificationNumber(const string &data);
        string toString()const;
};

IdentificationNumber::IdentificationNumber(const string &id, const string &regionalCode){
    this->id = id;
    this->regionalCode = regionalCode;
}

IdentificationNumber::IdentificationNumber(const string &data){
    vector<string> parts = split(data);
    this->id = parts[0];
    this->regionalCode = parts[1];
};

string IdentificationNumber::toString()const{
    return id + " " + regionalCode;
}

class Automobile{
    public:
        Date registrationDate;
        string color;
        TechnicalData data;
        IdentificationNumber number;

        Automobile(const string &color,
                   const string &brandName, const string &model, const int &constructionYear,
                   const string &registrationDate,
                   const string &id, const string &regionalCode);
        
        bool fit(const string &criteria, const string &value)const;
        string toString()const;
};

Automobile::Automobile(const string &color,
                       const string &brandName, const string &model, const int &constructionYear,
                       const string &registrationDate,
                       const string &id, const string &regionalCode){
    this->registrationDate = Date(registrationDate);
    this->data = TechnicalData(brandName, model, constructionYear);
    this->color = color;
    this->number = IdentificationNumber(id, regionalCode);
}


bool Automobile::fit(const string &criteria, const string &value)const{
    if(criteria.compare("color") == 0){
        return color.compare(value) == 0;
    }else if(criteria.compare("brand") == 0){
        return data.brandName.compare(value) == 0;
    }else if(criteria.compare("model") == 0){
        return data.model.compare(value) == 0;
    }else if(criteria.compare("constructionYear") == 0){
        return data.constructionYear == stoi(value);
    }else if(criteria.compare("registrationDate") == 0){
        Date date(value);
        return registrationDate == date;
    }else if(criteria.compare("id") == 0){
        return number.id.compare(value) == 0;
    }else if(criteria.compare("regionalCode") == 0){
        return number.regionalCode.compare(value) == 0;
    }else return false;
}

string Automobile::toString()const{
    stringstream ss;
    ss << color << "\t" << data.toString() << endl;
    ss << "Registered:\t" << registrationDate.toString("literal") << "\twith number: " << number.toString() << endl;
    return ss.str();
}

class UI{
    public:
        map<string, Automobile> cars;
        const int executeCommand(const string &command);
    private:
        void registerAutomobile(vector<string> &args);
        void removeAutomobile(const vector<string> &args);
        void searchCommand(vector<string> &args)const;
        vector<Automobile> search(vector<string> &args)const;
};

void UI::registerAutomobile(vector<string> &args){
    Automobile car(args[1], args[2], args[3], stoi(args[4]), args[5], args[6], args[7]);
    cars.emplace(car.number.toString(), car);
}

void UI::removeAutomobile(const vector<string> &args){
    cars.erase(args[1] + " " + args[2]);
}

vector<Automobile> UI::search(vector<string> &args)const{
	map<string, string> items;
	int i=1;
	while(i+1 < args.size()){
	    items.emplace(args[i], args[i+1]);
	    i+=2;
	}
	vector<Automobile> results;
	for(auto pair:cars){
	    Automobile car = pair.second;
	    bool found = true;
	    for(auto criteria:items){
	        if(!car.fit(criteria.first, criteria.second)){
	            found = false;
	            break;
	        }
	    }
	    if(found)
	        results.push_back(car);
	}
	return results;
}

void UI::searchCommand(vector<string> &args)const{
    vector<Automobile> results = search(args);
    if(results.empty())
        cout << "Nothing found! Try again\n";
    else{
        cout << "Results:\n";
        for(auto car:results)
            cout << car.toString() << endl;
    }
}

const int UI::executeCommand(const string &command){
    vector<string> args = split(command);
    if(!args.empty()){
        string commandName = args[0];
        if(commandName.compare("exit") == 0)
            return 0;
        else if(commandName.compare("register") == 0){
            registerAutomobile(args);
            return 2;
        }else if(commandName.compare("search") == 0){
            searchCommand(args);
            return 3;
        }else if(commandName.compare("remove") == 0){
            removeAutomobile(args);
            return 4;
        }
    }
    return 1;
}

int main()
{
    UI ui;
    string command;
    ui.executeCommand("register black Honda Octavia 2010 10.9.2020 y123ba 13rus");
    ui.executeCommand("register blue Subaru Impreza 2003 6.6.2008 x001xx 785jp");
    ui.executeCommand("register black Hummer v1 1990 6.6.2008 u001sa CAL");
    ui.executeCommand("register crimson Lada 107 1995 1.3.1995 x007am 30rus");
    ui.executeCommand("register silver Nissan Skyline 2002 8.10.2014 x133lx 55rus");
    ui.executeCommand("register pink Mazda RX7 2000 7.8.2013 a001ha 47kz");
    ui.executeCommand("register silver Toyota RAV4 2001 24.5.2011 o024yy 55rus");
    ui.executeCommand("register white Mercedes W210 1995 1.1.2003 y009tm 50rus");
    ui.executeCommand("register black BMW 750IL 2001 1.2.2003 o248bh 77rus");
    ui.executeCommand("register white Horse Arabic 2010 10.7.2010 k000oh 55rus");
    do{
        getline(cin, command);
        if(ui.executeCommand(command) == 0)
            break;
    }while(true);
    return 0;
}

