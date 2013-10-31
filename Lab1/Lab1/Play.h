#if ! defined (PLAY_H)
#define PLAY_H

#include <string>
#include <mutex>
#include <future>
#include <iostream>
#include "str_op.h"

using namespace std;

class Play{

private :
	string playname;
	mutex mut;
	string curr_char ; //name of the current character whose text is being printed.
	int counter;
	condition_variable con;
public:
	Play();
	Play(string playname_);
	void recite(vector<struct_line>::iterator &it);

};

#endif /* defined PLAY_H */