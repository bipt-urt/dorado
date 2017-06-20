// Dorado编译器
// 2017 北京石油化工学院
// 王万霖 梁永宁
// g++ dorado.cpp -o dorado --std=c++11
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

string to_bytes(u16string _str)
{
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> cvt;
	return cvt.to_bytes(_str);
}

u16string from_bytes(string _str)
{
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> cvt;
	return cvt.from_bytes(_str);
}

int main()
{
	fstream files("text/a.cpp",std::ios::in);
	string line;
	while(getline(files,line))
	{ 
		u16string da=from_bytes(line);
		u16string newline=u"";
		bool statement=true;
		for(auto ele:da)
		{ 
			if(statement==true)
			{
				if(ele!=u' ' && ele!=u'\t')
				{
					statement=false;
					newline+=ele; 
				}
			}
			else
			{
				newline+=ele; 
			}
		}
		cout<<to_bytes(newline)<<endl;
	}
}
