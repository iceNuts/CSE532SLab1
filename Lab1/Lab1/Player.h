#if ! defined (PLAYER_H)
#define PLAYER_H

#include "Play.h"
#include <fstream>


class Player{
public:
	/*A construtor which intialize the reference member variable with the reference
	*that is passed to the constructor
	*Also default construct the _playThread */
	Player(Play& Object, const string& charName, ifstream& inputFile);
	/* Overload the copy constructor
	*prevent copying the thread member variable*/
	Player(const Player& _player);

	void read();
	/*call the join method on each thread spawned*/
	void act();
	void enter();
	void exit();



private:
	vector<struct_line> inner_data;//container of struct_line 
	thread _playerThread;//a thread to launch for every player object
	Play& _playObject;//reference to the play object
	const string& _char_name;//reference to the character name
	ifstream& _inputFile; //reference to the character file

};

#endif /* defined PLAYER_H */