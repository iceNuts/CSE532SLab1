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

mutex m;

class str_op{
public:
	template<typename T>
	T StringToNumber(const string &Text){
		stringstream ss(Text);
		T result;
		return ss >> result ? result : 0;
	}
	bool isblank(string &str){
		if(strlen(str.c_str())==0)
			return true;
		return false;
	}
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

		void recite(vector<struct_line>::iterator &it){
			unique_lock<mutex> lk(mut);
			if(counter > it -> num)
			{
				//check if counter valid
				cerr << "oops, counter got too large!!" << endl;
				it++;
				con.notify_all();
				return;
			}
			con.wait(lk, [=]{return counter == it->num;});
			//compare current character 
			if(it -> charname != curr_char){cout << endl << it -> charname << endl; curr_char = it -> charname;}
			cout << it -> text << endl;
			it++, counter++;
			lk.unlock();
			con.notify_all();
		}

};

class Player{
public:
	Player(Play& Object, const string& charName, ifstream& inputFile) : 
		_playObject(Object),
		_char_name(charName),
		_inputFile(inputFile),
		_playerThread(thread())
	{}
	void read(){
		str_op op;
		string str;
		while(!_inputFile.eof()){
			cout<<"we are here"<<_char_name<<endl;
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
	}

	void act(){
		vector<struct_line>::iterator it = inner_data.begin();
		while(it <= inner_data.end())
		{
			_playObject.recite(it);
			it++;
		}
	}

	void enter(){
		cout << _char_name << "ok" << endl;
		cout << _inputFile << endl;
		thread _t([&](){
			this->read();
			this->act();
		});
		_playerThread = move(_t);
	}

	void exit()
	{
		if(_playerThread.joinable()) 
			_playerThread.join();
	}

private:
	vector<struct_line> inner_data;
	thread& _playerThread;
	Play& _playObject;
	const string& _char_name;
	ifstream& _inputFile;

};

/**
 *  function that's used to launch separate thread per valid line in configuration file.
 *  The actual validation is done in main itself.
 *  this function makes sure that each line in the fragment is well formed, if yes, prepares an instance of struct_line
 *  and pushes into play object. The play object itself hold mutex while doing the insert.
 */


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
		vector<Player> players;
		vector<string> charstrs;
		vector<ifstream> files;
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
			 charstrs.push_back(move(charname));
			 files.push_back(move(playfilefs));
			 i++;

		}
	   for(unsigned int j = 0; j < i; j++){
			Player player = Player(ref(play), ref(charstrs[j]), ref(files[j]));
			 players.push_back(player);
	   }
	   for(unsigned int j = 0; j < i; j++){
			players[i].enter();
	   }
	   for(unsigned int j = 0; j < i; j++){
			players[i].exit();
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