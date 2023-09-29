#include <iostream>
#include <vector>
#include <algorithm>

bool compare(int &a, int &b){
    return a > b;}

void sort(std::vector<int>::iterator begin, std::vector<int>::iterator end){
    size_t length = end - begin;
    switch(length){
        case 0:
        case 1: break;
        case 2:
            if(!compare(*begin, *std::next(begin)))
                std::swap(*begin, *std::next(begin));
            break;
        default:
            size_t insertSize=0, halfLength = length/2;
            auto insertTo=begin, insertFrom=std::next(begin, halfLength);
            sort(begin, insertFrom);
            sort(insertFrom, end);
            while(insertTo < end){
                while((std::next(insertFrom, insertSize)<end)&&(!compare(*insertTo, *std::next(insertFrom, insertSize))))
                    ++insertSize;
                if(insertSize>0){
                    auto delimiter=std::next(insertTo, insertSize);
                    std::vector<int> copy(insertSize);
                    for(auto it=insertFrom; it<std::next(insertFrom, insertSize); it=std::next(it))
                        copy[it-insertFrom] = *it;
                    for(auto it=std::next(insertFrom, insertSize-1); it>=delimiter; it=std::prev(it))
                        *it=*std::prev(it, insertSize);
                    for(auto it=insertTo; it<delimiter; it=std::next(it))
                        *it=copy[it-insertTo];
                    insertTo=std::next(insertTo, insertSize);
                    insertFrom=std::next(insertFrom, insertSize);
                    insertSize = 0;
                }
                insertTo=std::next(insertTo);
            }
    }
}

std::vector<std::string> split(const std::string &line, const char &delimiter=' '){
    std::vector<std::string> args;
    auto previousIt = line.begin();
    auto nextIt = previousIt;
    do{
        nextIt = std::find(previousIt, line.end(), delimiter);
        std::string arg = line.substr(previousIt-line.begin(), nextIt-previousIt);
        if(!arg.empty())
            args.push_back(arg);
        previousIt = nextIt+1;
    }while(nextIt != line.end());
    return args;
}

int main() {
	std::string line;
	getline(std::cin, line);
	std::vector<std::string> args = split(line);
	std::vector<int> arr;
	for(std::string &arg:args)
		arr.push_back(std::stoi(arg));
	sort(arr.begin(), arr.end());
	for(int &element:arr)
		std::cout << element << '\t';
	return 0;
}
