#ifndef CONFIG_H_
#define CONFIG_H_

#include <map>
#include <string>
#include <iostream>
#include <fstream>

class Config{
public:
	std::map<std::string, std::map<std::string, std::string>> units;
	Config(const std::string &file_name="config.ini"):units{}{
		std::ifstream ifs{file_name};
		std::string line;
		getline(ifs, line);
		do{
			std::string unit_name = line.substr(line.find(":")+1, line.rfind(":")-1);
			std::map<std::string, std::string> unit;
			if(line.empty()){
				getline(ifs, line);
				continue;
			}
			do{
				getline(ifs, line);
				if(line.empty()) continue;
				if(line.front()!=':'){
					size_t equal_sign_position = line.find("=");
					std::string key = line.substr(0, equal_sign_position),
								value = line.substr(equal_sign_position+1);
					unit[key] = value;
				}else break;
			}while(!ifs.eof());
			units[unit_name] = unit;
		}while(!ifs.eof());
		ifs.close();
	}
};


#endif /* CONFIG_H_ */
