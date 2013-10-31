#if ! defined (STR_OP_H)
#define STR_OP_H

#include <string> 
#include <vector>
#include <sstream>

using namespace std;
class str_op{

public:
	template<typename T>
	T StringToNumber(const string &Text);

	bool isblank(string &str);

	vector<string> split(string str, char c = ' ');

};

template<typename T>
T str_op::StringToNumber(const string &Text){
	stringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
}



/**
*  Struct to hold the data.
*	num -- line number.
*  charname -- name of the character (Queen, King, etc).
*  text -- actual text from the fragment file.
*/

struct struct_line{
	int num;
	string charname;
	string text;

};

enum CONST {ZERO,ONE,TWO, THREE};

#endif /* defined STR_OP_H */