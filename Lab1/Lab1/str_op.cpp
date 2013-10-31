#include "str_op.h"


//checking whether a line is blank
bool str_op::isblank(string &str){
		if(strlen(str.c_str())==0)
			return true;
		return false;
}
	//delete the multiple white space
vector<string> str_op::split(string str, char c ){
		vector<string> result;
		stringstream s(str);
		string token="";
		while(getline(s,token,c)){
			if(token.compare(" ")!=0)
				result.push_back(token);
		}

		return result;
	}	