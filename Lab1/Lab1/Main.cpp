/**
 * CSE532S Lab 1 FALL 2013
 * Authors --
 *		Amit Jha
 *		Di Yan
 *		Li Zeng
 */


#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <list>
#include <sstream>
#include <mutex> 
#include <chrono>
#include <condition_variable>


using namespace std;

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



class str_op{
public:
	template<typename T>
	//Covert a string to number
	T StringToNumber(const string &Text){
		stringstream ss(Text);
		T result;
		return ss >> result ? result : 0;
	}
	//checking whether a line is blank
	bool isblank(string &str){
		if(strlen(str.c_str())==0)
			return true;
		return false;
	}
	//delete the multiple white space
	vector<string> split(string str, char c = ' '){
		vector<string> result;
		stringstream s(str);
		string token="";
		while(getline(s,token,c)){
			if(token.compare(" ")!=0)
				result.push_back(token);
		}

		return result;
	}	
};

class Play{

	private :
		string playname;
		mutex mut;
		string curr_char ; //name of the current character whose text is being printed.
		
		int counter;
		condition_variable con;
	public:
		Play(){}
		Play(string playname_){
			playname = playname_;
			counter = 1;
		}
		/*Print out the character line in sequence
		 *Compare the line number to a counter which starts with 1
		 *when the counter is equal to the line number
		 *the line is allowed to print, then the counter will be incremented
		 *Otherwise, the line should wait till the counter reaches its number*/
		void recite(vector<struct_line>::iterator &it){
			unique_lock<mutex> lk(mut);
			if(counter > it -> num)
			{
				//check if counter valid, counter shouldn't be greater than the line number
				cerr << "Sorry, there must be bad format in the file" << endl;
				it++;
				con.notify_all();
				return;
			}
			//compare current character 
			con.wait(lk, [=]{return this->counter == it->num;});
			//check whether it is a new character's line
			if(it -> charname != curr_char){cout << endl << it -> charname << endl; curr_char = it -> charname;}
			cout << it -> text << endl;
			//increment the iterator as well as the counter
			it++, counter++;
			lk.unlock();
			con.notify_all();
		}

};

class Player{
public:
	/*A construtor which intialize the reference member variable with the reference
	 *that is passed to the constructor
	 *Also default construct the _playThread */
	Player(Play& Object, const string& charName, ifstream& inputFile) : 
		_playObject(Object),
		_char_name(charName),
		_inputFile(inputFile)
	{_playerThread=thread();}
	/* Overload the copy constructor
	 *prevent copying the thread member variable*/
	Player(const Player& _player):
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
	void read(){
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
		return;
	}
	/*Create a iterator of the struct_line container
	 *iterate the container 
	 *For every struct_line the iterator positioned at
	 *call recite method of the play object passed to the object 
	 *then print out the line in sequence*/
	void act(){
		vector<struct_line>::iterator it = inner_data.begin();
		while(it < inner_data.end())
		{
			_playObject.recite(it);
		}
		return;
	}
	/* For each player object created
	 *Launch a new thread to execute the read and act method*/
	void enter(){
		thread _t([this]{
			this->read();
			this->act();
		});

		_playerThread=move(_t);
	}
	/*call the join method on each thread spawned*/
	void exit()
	{
		if(_playerThread.joinable()) 
			_playerThread.join();
	}

private:
	vector<struct_line> inner_data;//container of struct_line 
	thread _playerThread;//a thread to launch for every player object
	Play& _playObject;//reference to the play object
	const string& _char_name;//reference to the character name
	ifstream& _inputFile; //reference to the character file

};


int main(int argc, char* argv[]){

	std::ifstream ifs;
	
	if(argc == 1)
	{
			cout << "usage: " << argv[0] << "<configuration_file_name>" << endl;
			std::system("pause");
			return 1;
	}

	cout << "Configuration File is :: " << argv[1] << endl;

	ifs.open (argv[1], std::ifstream::in);
	string playname="";
				
	if(ifs.good()){
		str_op op;
		//read the first non blank line which will be assigned playname.
		while(!ifs.eof() && op.isblank(playname)){
				getline(ifs,playname);
		}
		
		if(playname.length() == 0){
			cout << "Configuration file is empty, exiting" << endl;
			std::system("pause");
			return 1;
		}
		
		//should have playname by now. ready to initialize the class 
		Play play(playname);
		string str="";
		vector<Player> players;//To store the multiple player object
		vector<string> charstrs;//To store all the character name that is detected from the configuration file
		vector<ifstream> files;//To store all the character file that is detected from the configuration file
		//read one line at a time, validate that its well formed and based on that prepare threads to be launched.
		unsigned int i = 0;
		while(!ifs.eof()){
			 getline(ifs, str);
			 
			 if(op.isblank(str))
				continue;
			 
			 vector<string> tokens = op.split(str);
			if(tokens.size() < 2)
			 {
				 cout << "ignoring line as its not correctly formatted -- " <<  str;
				 continue;
			 }
			string charname(tokens [0]);
			 string filename(tokens [1]);
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
			 Player player = Player(ref(play), ref(charstrs[j]), ref(files[j]));
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

		return 0;
	}else{
		cout << "Configuartion File not found. Please correct the path and try again" << endl;
		return 1;
	}

	ifs.close();
	std::system("pause");
	return 0;
}