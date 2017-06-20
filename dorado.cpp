// Dorado编译器
// 2017 北京石油化工学院
// 王万霖 梁永宁
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

string to_bytes(u32string _str)
{
	wstring_convert<codecvt_utf8_utf16<char32_t>, char32_t> cvt;
	return cvt.to_bytes(_str);
}

u32string from_bytes(string _str)
{
	wstring_convert<codecvt_utf8_utf16<char32_t>, char32_t> cvt;
	return cvt.from_bytes(_str);
}

int main()
{
	fstream files=("1.c",std::ios::in);
	u32stream line;
	for(getline(file,line))
	 cout<<line<<endl;

	return 0;
}
