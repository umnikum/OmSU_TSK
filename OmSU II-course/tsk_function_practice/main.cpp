#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

/*class Function{
    protected:
        vector<double> params;
    public:
        Function(const vector<double> &params){
            setParams(params);}
        virtual int dimentions()const;
        void setParams(const vector<double> &newParams){
            params = newParams;
        }
        virtual double calculateFor(const vector<double> &x)const;
        virtual string toString()const;
};*/

class MyFunction{
    protected:
        vector<double> params;
    public:
        MyFunction();
        int dimentions()const{
            return 3;}
        void setParams(const vector<double> &newParams){
            params = newParams;}
        double operator()(const vector<double> &x)const;
        string toString()const;
};

MyFunction::MyFunction(){
    vector<double> defaultParams = {0.0, 0.0, 1.0};
    setParams(defaultParams);
}

double MyFunction::operator()(const vector<double> &x)const{
    double denominator = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    denominator *= denominator * denominator;
    return (params[0] * x[0] + params[1] * x[1] + params[2] * x[2])/denominator;
}

string MyFunction::toString()const{
    stringstream ss;
    ss << "({" << params[0] << ", " << params[1] << ", "<< params[2] << "}*r)/r^3";
    return ss.str();
}

class History{
    private:
        vector<string> lines;
    public:
        //History(const string &fileName);
        void addLine(const string &line){
            lines.insert(lines.end(), line);}
        string print(const int linesCount)const;
};

/*History::History(const string &fileName){
    ifstream inputFileStream(fileName);
    streambuf *cinBuffer = cin.rdbuf(); //save old buffer
    cin.rdbuf(inputFileStream.rdbuf()); //redirect cin to file

    while(!cin.eof()){
        string line;
        cin.getLine(line);
        addLine(line);
    }

    cin.rdbuf(cinBuffer);
}*/

string History::print(const int linesCount=0)const{
    stringstream ss;
    if(linesCount<=0)
        for(string line:lines)
            ss << line << endl;
    else{
        auto start = (lines.size()<linesCount)?lines.begin():lines.end()-linesCount;
        for(auto it=start; it<lines.end(); it++)
            ss << *it << endl;
    }
    return ss.str();
}

class UI{
    private:
        History history;
        MyFunction function;
        vector<string>& splitLine(const string &line)const;
        string man(const string &argument="")const;
    public:
        static const constexpr char* const manual[] = {"Enter command:",
                             "This program calculate function:",
                             "Known commands:",
                             "man command_name - describes command functionality",
                             "exit - finishing program execution",
                             "setparams params_vector - setting parameters(type double)  for used function",
                             "calculate value_vector - calculate function for specified argument value(type double)",
                             "history lines_counter - writes specified amount of lines which contains previous calculations or all if not specified"};
        bool executeCommand(const string &command);
};

vector<string>& UI::splitLine(const string &line)const{
    vector<string> *arguments = new vector<string>();
    istringstream iss(line);
    string s;
    while (getline(iss, s, ' '))
        arguments->insert(arguments->end(), s);
    return *arguments;
}

string UI::man(const string &argument)const{
    stringstream ss;
    if(argument.compare("man") == 0){
        ss << manual[3] << endl;
    }else if(argument.compare("exit") == 0){
        ss << manual[4]<< endl;
    }else if(argument.compare("setparams") == 0){
        ss << manual[5]<< endl;
    }else if(argument.compare("calculate") == 0){
        ss << manual[6]<< endl;
    }else if(argument.compare("history") == 0){
        ss << manual[7]<< endl;
    }else{
        ss << manual[1] << function.toString() << endl;
        for(int i=2; i<8; i++)
            ss << manual[i] << endl;
    }
    return ss.str();
}

bool UI::executeCommand(const string &command){
    vector<string> &arguments = splitLine(command);
    string commandName = arguments[0];
    if(commandName.compare("setparams") == 0){
        vector<double> params;
        for(int i=1; i<arguments.size(); i++)
            params.insert(params.end(), stod(arguments[i]));
        function.setParams(params);
    }else if(commandName.compare("calculate") == 0){
        vector<double> x;
        for(int i=1; i<arguments.size(); i++)
            x.insert(x.end(), stod(arguments[i]));
        stringstream ss;
        ss << function.toString() << " = " << function(x) << " for r={";
        for(double xi:x)
            ss << xi << ",";
        ss.seekp(-1, std::ios_base::end);
        ss << "}";
        history.addLine(ss.str());
        cout << ss.str() << endl;
    }else if(commandName.compare("history") == 0){
        int linesCount = 0;
        if(arguments.size() > 1)
            linesCount = stoi(arguments[1]);
        cout << history.print(linesCount);
    }else if(commandName.compare("man") == 0){
        if(arguments.size() > 1)
            cout << man(arguments[1]);
        else cout << man();
    }else if(commandName.compare("exit") == 0){
        return false;
    }
    return true;
}

int main()
{
    UI ui;
    string command;
    cout << ui.manual[1] << endl << ui.manual[0] << endl;
    do{
        cout << ">>";
        getline(cin, command);
    }while(ui.executeCommand(command));
    return 0;
}
