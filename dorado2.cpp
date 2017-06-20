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

class typeAttribute
{
	public:
		
};

int main()
{
	u32string a = U"你好，世界！";
	cout<<to_bytes(a)<<endl;
	return 0;
}
