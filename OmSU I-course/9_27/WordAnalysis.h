#ifndef WORDANALYSIS_H_
#define WORDANALYSIS_H_

#include <string>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <algorithm>

std::vector<std::string> split(const std::string &line, const char &delimiter=' ');

class WordScore{
public:
	WordScore(const std::string &word="");
	bool equal(const WordScore &score)const;
	const std::string toString()const;
private:
	std::map<char, int> counters{};
};

class AnagramList{
public:
	std::map<std::string, int> words{};
	AnagramList(const WordScore &score);
	AnagramList(const std::string &word);
	const int size()const;
	bool isCompatible(const std::string &word)const;
	bool isCompatible(const WordScore &score)const;
	void place(const std::string &word);
	const std::string toString(const bool mode=false)const;
private:
	WordScore identity;
};

class AnagramDictionary{
public:
	bool place(const std::string &word);
	const std::string toString(const bool mode=false)const;
private:
	std::list<AnagramList> mentions{};
};

#endif /* WORDANALYSIS_H_ */
