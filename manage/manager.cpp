#include "manager.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <regex>
#define PASSWORD "123"
#define COMMANDADD "deluge-console add "
#define COMMANDINFO "deluge-console info "

using namespace std;

string getResult(string path){

   ifstream newfile(path);
   ostringstream sstr;

  
    if(newfile.is_open()){
    	sstr << newfile.rdbuf();
    	newfile.close();
    	return sstr.str();
    }
    else{
    	return "Failed";
    }

   
}


string getDir(char* path){
	int s = strlen(path);
	for (int i = s-1; i >= 0; --i)
	{
		if (path[i] == '/')
		{
			s = i;
			break;
		}
	}
	string final = path;
	final.resize(s);
	final.shrink_to_fit();
	return final;

}

char* getPath(){
	char* pBuf = (char*) malloc(sizeof(char)*256);
	int bytes = readlink("/proc/self/exe", pBuf, 256);
	return pBuf;
}


string addTorrent(string magnet,string password){
	if(!system(NULL))return "Failed";
	if(PASSWORD != password) return "Failed";
	char* exePath = getPath();
	string path =  getDir(exePath) + "/result.txt";
	string cmd = COMMANDADD+string("\"")+magnet+string("\"")+string(" > ")+path;
	int r = system(cmd.c_str());
	free(exePath);

	return getResult(path);

}

string getInfo(){
	if(!system(NULL))return "Failed";
	char* exePath = getPath();
	string path =  getDir(exePath) + "/info.txt";
	string cmd = COMMANDINFO+string(" > ")+path;
	int r = system(cmd.c_str());
	free(exePath);

	return getResult(path);

}

