#include "Play.h"


Play::Play(){}

Play::Play(string playname_){
	playname = playname_;
	counter = ONE;
}

/*Print out the character line in sequence
*Compare the line number to a counter which starts with 1
*when the counter is equal to the line number
*the line is allowed to print, then the counter will be incremented
*Otherwise, the line should wait till the counter reaches its number*/
void Play::recite(vector<struct_line>::iterator &it){
	unique_lock<mutex> lk(mut);
	if(counter > it -> num)
	{
		//check if counter valid, counter shouldn't be greater than the line number
		cerr << "Sorry, there must be bad format in the file" << endl;
		it++;
		lk.unlock();
		con.notify_all();
		return;
	}
	//compare current character 
	con.wait(lk, [=]{return this->counter == it->num;});
	//check whether it is a new character's line
	if(it -> charname != curr_char){
		cout << endl << it -> charname << "." << endl; 
		curr_char = it -> charname;
	}
	cout << it -> text << endl;
	//increment the iterator as well as the counter
	it++, counter++;
	lk.unlock();
	con.notify_all();
}