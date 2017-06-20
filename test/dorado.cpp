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
	fstream files=("1.c",std::ios::in);
	u32stream line;
	for(getline(file,line))
	 cout<<line<<endl;

	return 0;
}
