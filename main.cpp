#include <iostream>
#include <regex>
#include <string>

using namespace std;

bool isMagnet(string magnet){
	string pattern = "magnet:\\?xt=urn:btih:(.)*";
	regex reg(pattern);
	cout << regex_match(magnet,reg) <<endl;
	return regex_match(magnet,reg);


}



int main(int argc, char const *argv[])
{

	cout << argc << endl;
	cout << argv[1] << endl;

	if (argc < 2)
	{
		cout << "No argument" << endl;
		return -1;
	}
	else if(argc > 2){
		cout << "Too much argument" << endl;
		return -1;
	}
	else if(!isMagnet(argv[1])){
		cout << "Not a magnet link" << endl;
		return -1;
	}

	






	return 0;
}

