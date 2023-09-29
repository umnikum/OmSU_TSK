#include <iostream>
#include <vector>

const int gdc(const int &a, const int &b){
	std::pair<int, int> pair{a, b};
	while(pair.first != pair.second)
		(pair.first < pair.second)? pair.second -= pair.first : pair.first -= pair.second;
	return pair.first;
}


const int gdc(const std::vector<int> &sequence){
	switch(sequence.size()){
	case 0: return 0;
	case 1: return sequence[0];
	default:
		int result=sequence[0];
		for(size_t i=1; i<sequence.size(); ++i)
			result = gdc(result, sequence[i]);
		return result;
	}
}

int main(){
	return 0;
}
