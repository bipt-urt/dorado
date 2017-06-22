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
//去掉空格
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
//提取＃开头的函数
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
//检查＃ifdef和#endif是否匹配
int checkCombination(vector<u16string>& lines)
{
	stack<u16string> temStack;
	for (auto element: lines)
	{
		if (element == u"#ifdef")
		{
			temStack.push(element);
		}
		else if(element == u"#endif") 
		{
			if (temStack.empty())
			{
				cout<<"没有#ifdef与之匹配"<<endl;
				return -1;
			}
			else
			{
				while (true)
				{
					if (temStack.top() == u"#ifdef")
					{
						temStack.pop();
						break;
					}
					else
					{
						if (temStack.empty())
						{
							cout<<"没有#ifdef与之匹配"<<endl;
							return -1;
						}
						else
						{
							temStack.pop();
						}
					}
				}
			}
		}
		else
		{
			continue;
		}
	}
	if (temStack.empty())
	{
		cout<<"匹配成功"<<endl;
	}
	else
	{
		cout<<"没#endif与之匹配"<<endl;
	}
	return 0;
}
//检查＃include是否都展开了
int checkTheInclude(vector<u16string>& combination)
{
	int count = 0;
	for (auto element: combination)
	{
		if (element == u"#include")
			{
				count++;
				break;
			}
		else
		{
			count++;
		}
	}
	return count;
}

//　展开include文件
vector<u16string> openInclude(vector<u16string>& lines, vector<u16string>& vecHead)
{
	vector<u16string> combination;
	int count=1;
	//u16string temString;
	combination = preservePretreatment(lines);
	//count = checkTheInclude(combination);
	//cout<<count<<endl;
	while (count != 0)
	{
		count = checkTheInclude(combination);
		lines.erase(lines.begin()+count-1);
		cout<<count<<endl;
	}
	return combination;
}

int main()
{
	fstream files("test/a.cpp", std::ios::in);
	fstream headerfile("include/stdio.h", std::ios::in);
	string line;
	string header;
	vector<u16string> lines;
	vector<u16string> vecHead;
	while (getline(headerfile, header))
	{
		vecHead.push_back(from_bytes(header));
	}
	while (getline(files, line))
	{
		lines.push_back(from_bytes(line));
	}
	lines = removeBlank(lines);
	vecHead = openInclude(lines,vecHead);
	//lines = preservePretreatment(lines);
	//checkCombination(lines);
	return 0;
}