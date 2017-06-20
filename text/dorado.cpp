#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <locale>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
	fstream files("a.cpp",std::ios::in);
	string line;
	while(getline(files,line))
	 {
		 cout<<line<<endl;
	 }
	return 0;
}
