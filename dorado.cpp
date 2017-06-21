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
#include <stack>
#include <map>
#include <string>
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

vector<u16string> removeBlank(vector<u16string>& lines)
{
	vector<u16string> newli;
	for (auto element: lines)
	{
		u16string var;
		bool statement = true;
		for(auto ele: element)
		{
			if (statement)
			{
				if (ele != u' ' && ele != u'\t')
				{
					statement = false;
					var += ele;
				}
			}
			else
			{
				var += ele;
			}
		}
		newli.push_back(var);
	}
	return newli;
}

vector<u16string> preservePretreatment(vector<u16string>& lines)
{
	vector<u16string> newli;
	for (auto element: lines)
	{
		u16string var;
		if (element[0] == u'#')
		{
			for(auto ele: element)
			{
				if (ele == u'<' || ele == u' ' || ele == u'"')
				{
					break;
				}
				else
				{
					var += ele;
				}
			}
		}
		else
		{
			continue;
		}
		newli.push_back(var);
	}
	return newli;
}


int main()
{
	fstream files("test/a.cpp", std::ios::in);
	string line;
	vector<u16string> lines;
	while (getline(files, line))
	{
		lines.push_back(from_bytes(line));
	}
	lines = removeBlank(lines);
	lines = preservePretreatment(lines);
	for (auto element: lines)
	{
		cout<<to_bytes(element)<<endl;
	}
	return 0;
}