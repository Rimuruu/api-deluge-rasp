#include "manager.h"

#define PASSWORD "123"


int addTorrent(string magnet,string password){
	if(!system(NULL))return -1;
	if(PASSWORD != password) return -2;
	string cmd = "deluge-console add "+magnet;
	int r = system(cmd.c_str());
	return r;




}


