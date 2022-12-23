#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

char beginingSign = '(';
char endingSign = ')';

class Node{
    public:
        Node(string value);
        Node(Node* root, string value);
        ~Node();
        string getValue()const;
        Node* findChild(string value);
        void addChild(Node* child);
        Node* removeChild(string value);
        vector<Node*> getChildren()const;
        Node* getRoot()const;
        string toString()const;
    protected:
        Node* root;
        vector<Node*> children;
        string value;
    private:
        Node* removeMyChild(string value);
};

Node::Node(string value){
    this->value = value;
    root = nullptr;
}

Node::Node(Node* root, string value){
    this->value = value;
    this->root = root;
    root->addChild(this);
}

Node::~Node(){
    for(Node* child:children)
        delete child;
}

Node* Node::removeMyChild(string value){
    Node* found = nullptr;
    for(int i=0; i<children.size(); i++){
        if(children[i]->getValue() == value){
            found = children[i];
            found->root = nullptr;
            children.erase(children.begin() + i);
            break;
        }
    }
    return found;
}

string Node::getValue()const{
    return value;}

Node* Node::findChild(string value){
    Node* found = nullptr;
    if(this->value == value)
        return this;
    for(Node* child:children){
        found = child->findChild(value);
        if(found != nullptr)
            break;
    }
    return found;
}

Node* Node::removeChild(string value){
    Node* found = findChild(value);
    if(found == nullptr)
        return nullptr;
    if(this->value == value){
        this->root = nullptr;
        return this;
    }
    found->root->removeMyChild(value);
    return found;
}

void Node::addChild(Node* child){
    children.insert(children.end(), child);
    child->root = this;
}

vector<Node*> Node::getChildren()const{
    return children;}

Node* Node::getRoot()const{
    return root;}

string Node::toString()const{
    stringstream stream;
    stream << beginingSign << value;
    for(Node* child:children)
        stream << child->toString();
    stream << endingSign;
    return stream.str();
}

class NodeReader{
    public:
        NodeReader(){};
        Node* readStream(istream stream);
        Node* readLine(string data);
    private:
        size_t findClosestSign(string data);
        string exstractValue(string data);
};

size_t NodeReader::findClosestSign(string data){
    size_t  begining = data.find(beginingSign),
            ending = data.find(endingSign);
    return (begining < ending)?begining:ending;
}

string NodeReader::exstractValue(string data){
    return data.substr(0, findClosestSign(data));}

Node* NodeReader::readLine(string data){
    size_t currentSign = findClosestSign(data);
    data = data.substr(currentSign+1);
    Node* root = new Node(exstractValue(data));
    currentSign = findClosestSign(data);

    Node* node = root;
    while(currentSign != string::npos){
        if(data[currentSign] == endingSign){
            data = data.substr(currentSign+1);
            node = node->getRoot();
        }else   if(data[currentSign] == beginingSign){
            data = data.substr(currentSign+1);
            Node* newNode = new Node(node, exstractValue(data));
            node = newNode;
        }
        currentSign = findClosestSign(data);
    }
    return root;
}

Node* NodeReader::readStream(istream stream){
    stringstream memory;
    for(string line; getline(stream, line);)
        memory << line;
    string text = memory.str();
    return readLine(text);
}

int treeInterface(){
    string input;
    cout << "Enter tree:\n";
    cin >> input;
    NodeReader reader;
    Node* root = reader.readLine(input);
    while(true){
        cout << "Enter tree command: ";
        cin >> input;
        if(input == "exit")
            return 0;
        else if(input == "reboot")
            return 1;
        else{
            if(input == "print")
                cout << "Current tree: " << root->toString() << endl;
            if(input == "remove"){
                cin >> input;
                Node* removed = root->removeChild(input);
                cout << "Removed: " << removed->toString() << endl;
            }
            if(input == "add"){
                cin >> input;
                Node* address = root->findChild(input);
                cin >> input;
                Node* child = reader.readLine(input);
                address->addChild(child);
                cout << "Inserted: " << child->toString() << endl;
            }
        }
    }
    return 1;
}

int executeCommand(string command){
    if(command == "exit") return 0;
    else if(command == "start")
        return treeInterface();
    //return 1;
}

void userInterface(){
    cout << "This program works with trees:\n";
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
