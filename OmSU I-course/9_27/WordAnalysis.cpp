#include "WordAnalysis.h"

std::vector<std::string> split(const std::string &line, const char &delimiter){
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

WordScore::WordScore(const std::string &word){
	for(const char &c:word)
		if(this->counters.count(c) > 0)
			this->counters.at(c)++;
		else this->counters.emplace(c, 1);
}

bool WordScore::equal(const WordScore &score)const{
	for(const std::pair<char, int> &pair:this->counters)
		if(score.counters.count(pair.first) == 0){
			return false;
		}else if(pair.second != score.counters.at(pair.first))
			return false;
	for(const std::pair<char, int> &pair:score.counters)
		if(this->counters.count(pair.first) == 0){
			return false;
		}else if(pair.second != this->counters.at(pair.first))
			return false;
	return true;
}

const std::string WordScore::toString()const{
	std::stringstream topss, botss;
	for(const std::pair<char, int> &pair:this->counters){
		topss << pair.first << "\t";
		botss << pair.second << "\t";
	}
	topss << std::endl << botss.str();
	return topss.str();
}

AnagramList::AnagramList(const WordScore &score){this->identity = score;}
AnagramList::AnagramList(const std::string &word){
	this->identity = WordScore(word);
	this->words.emplace(word, 1);
}
const int AnagramList::size()const{return this->words.size();}
bool AnagramList::isCompatible(const std::string &word)const{return this->identity.equal(WordScore(word));}
bool AnagramList::isCompatible(const WordScore &score)const{return this->identity.equal(score);}
void AnagramList::place(const std::string &word){
	if(this->words.count(word) == 0)
		this->words.emplace(word, 1);
	else ++this->words.at(word);
}

const std::string AnagramList::toString(const bool mode)const{
	std::stringstream ss;
	for(const std::pair<std::string, int> &pair:this->words)
		(mode)? ss << pair.first << "(" << pair.second << ")\t" : ss << pair.first << '\t';
	return ss.str();
}

bool AnagramDictionary::place(const std::string &word){
	const WordScore score(word);
	for(AnagramList &anagram:this->mentions)
		if(anagram.isCompatible(score)){
			anagram.place(word);
			return true;
		}
	this->mentions.push_back(AnagramList(word));
	return false;
}

const std::string AnagramDictionary::toString(const bool mode)const{
	std::stringstream ss;
	for(const AnagramList &anagram:this->mentions)
		if(anagram.size() > 1)
			ss << anagram.toString(mode) << std::endl;
	return ss.str();
}
