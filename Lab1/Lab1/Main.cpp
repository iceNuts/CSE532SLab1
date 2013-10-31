/**
* CSE532S Lab 1 FALL 2013
* Authors --
*		Amit Jha
*		Di Yan
*		Li Zeng
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>

#include "Player.h"

int main(int argc, char* argv[]){

	std::ifstream ifs;

	if(argc == ONE)
	{
		cout << "usage: " << argv[0] << "<configuration_file_name>" << endl;
		return ONE;
	}

	cout << "Configuration File is :: " << argv[1] << endl;

	ifs.open (argv[ONE], std::ifstream::in);
	string playname="";

	if(ifs.good()){
		str_op op;
		//read the first non blank line which will be assigned playname.
		while(!ifs.eof() && op.isblank(playname)){
			getline(ifs,playname);
		}

		if(playname.length() == 0){
			cout << "Configuration file is empty, exiting" << endl;
			return ONE;
		}

		//should have playname by now. ready to initialize the class 
		Play play(playname);
		string str="";
		vector<Player> players;//To store the multiple player object
		vector<string> charstrs;//To store all the character name that is detected from the configuration file
		vector<ifstream> files;//To store all the character file that is detected from the configuration file
		//read one line at a time, validate that its well formed and based on that populate the vectors
		unsigned int i = 0;
		while(!ifs.eof()){
			getline(ifs, str);

			if(op.isblank(str))
				continue;

			vector<string> tokens = op.split(str);
			if(tokens.size() < TWO)
			{
				cout << "ignoring line as its not correctly formatted -- " <<  str;
				continue;
			}
			string charname(tokens [ZERO]);
			string filename(tokens [ONE]);
			std::ifstream playfilefs;
			playfilefs.open(filename,std::ifstream::in);
			if(!playfilefs.good())
			{
				cout << "ignoring line as file is in accessible " <<  str << endl;
				continue;
			}
			//push the character name and the file to the vector
			charstrs.push_back(move(charname));
			files.push_back(move(playfilefs));
			i++;
		}
		//for each character, create a player object
		for(unsigned int j = 0; j <= i-1; j++){
			players.push_back(move(Player(ref(play), ref(charstrs[j]), ref(files[j]))));
		}
		//call the enter method for all the player objects
		for(unsigned int j = 0; j <= i-1; j++){
			players[j].enter();
		}
		//call the exit method for all the player objects
		for(unsigned int j = 0; j <= i-1; j++){
			players[j].exit();
		}
	}else{
		cout << "Configuartion File not found. Please correct the path and try again" << endl;
		return ONE;
	}

	ifs.close();
	return 0;
}