#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool wordCondition(string word){
    if(word.size() <= 1)
        return false;
    else return (word[1] == 'e')||(word[1] == 'E');
}

string readFile(string fileName){
    string longestWord, word;
    ifstream inputFileStream(fileName);
    streambuf *cinBuffer = cin.rdbuf(); //save old buffer
    cin.rdbuf(inputFileStream.rdbuf()); //redirect cin to file

    while(!cin.eof()){
        cin >> word;
        if(wordCondition(word)&&(word.size() >= longestWord.size()))
            longestWord = word;
    }

    cin.rdbuf(cinBuffer);               //reset to standard input again
    return longestWord;
}

string readConsole(){
    string longestWord;
    while(true){
        string word;
        cin >> word;
        if(wordCondition(word)&&(word.size() >= longestWord.size()))
            longestWord = word;
        if(word == "end")
            break;
    }
    return longestWord;
}


int searchInterface(){
    string input;
    cin >> input;
    if(input == "console")
        input = readConsole();
    else if(input == "file"){
        string fileName;
        cin >> fileName;
        input = readFile(fileName);
    }
    if(input.size() > 0)
        cout << "Found: " << input << endl;
    else cout << "Didn't find any words!\n";
    return 1;
}

int executeCommand(string command){
    if(command == "exit") return 0;
    if(command == "search") return searchInterface();
    return 1;
}

void userInterface(){
    cout << "This program is searching for the longest word in file with 2nd letter E:\n";
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
