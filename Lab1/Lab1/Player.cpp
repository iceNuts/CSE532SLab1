#include "Player.h"

Player::Player(Play& Object, const string& charName, ifstream& inputFile) : 
	_playObject(Object),
	_char_name(charName),
	_inputFile(inputFile)
{_playerThread=thread();}
/* Overload the copy constructor
*prevent copying the thread member variable*/
Player::Player(const Player& _player):
	_playObject(_player._playObject),
	_char_name(_player._char_name),
	_inputFile(_player._inputFile)
{
	_playerThread=thread();
}


/* read lines in the character file that passed to the play object
*ignore the badly formatted line
*store each line in the struct_line
*push each struct_line into a container*/
void Player::read(){
	str_op op;
	string str;
	while(!_inputFile.eof()){
		getline(_inputFile,str);
		if(op.isblank(str) || (str.find_first_of(" ") == string::npos) )
			continue;
		string linenum_s = str.substr(0,str.find_first_of(" "));
		string text = str.substr(str.find_first_of(" ")+1);
		int linenum  = op.StringToNumber<int>(linenum_s);
		if(linenum == 0 || op.isblank(text))
			continue;

		struct_line str_line ;
		str_line.num = linenum;
		str_line.charname = _char_name;
		str_line.text = text;

		inner_data.push_back(str_line);
	}
	//sort vector to avoid disorder
	struct linenum_sort_bool{
		inline bool operator() (const struct_line& struct1, const struct_line& struct2)
		{
			return (struct1.num < struct2.num);
		}
	};
	sort(inner_data.begin(), inner_data.end(), linenum_sort_bool());
}
/*Create a iterator of the struct_line container
*iterate the container 
*For every struct_line the iterator positioned at
*call recite method of the play object passed to the object 
*then print out the line in sequence*/
void Player::act(){
	vector<struct_line>::iterator it = inner_data.begin();
	while(it < inner_data.end())
	{
		_playObject.recite(it);
	}
	return;
}
/* For each player object created
*Launch a new thread to execute the read and act method*/
void Player::enter(){
	thread _t([this]{
		this->read();
		this->act();
	});

	_playerThread=move(_t);
}

void Player::exit()
{
	if(_playerThread.joinable()) 
		_playerThread.join();
}