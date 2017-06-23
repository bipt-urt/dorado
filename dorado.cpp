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
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

int greatFindDefine(u16string u16, int statement);

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

// 去掉空格
vector<u16string> removeBlank(vector<u16string>& lines)
{
	vector<u16string> removeBlankSourceFile;
	for (auto element: lines)
	{
		u16string saveRemoveBlankRow;
		bool statement = true;
		for (auto ele: element)
		{
			if (statement)
			{
				if (ele != u' ' && ele != u'\t')
				{
					statement = false;
					saveRemoveBlankRow += ele;
				}
			}
			else
			{
				saveRemoveBlankRow += ele;
			}
		}
		removeBlankSourceFile.push_back(saveRemoveBlankRow);
	}
	return removeBlankSourceFile;
}

// 提取＃开头的函数
vector<u16string> getIsSharpRow(vector<u16string>& lines)
{
	vector<u16string> saveHaveSharpRow;
	for (auto element: lines)
	{
		u16string saveSharpLow;
		if (element[0] == u'#')
		{
			for (auto ele: element)
			{
				if (ele == u'<' || ele == u'"' || ele == u' ')
				{
					break;
				}
				else
				{
					saveSharpLow += ele;
				}
			}
		}
		saveHaveSharpRow.push_back(saveSharpLow);
	}
	return saveHaveSharpRow;
}

// 检查＃ifdef和#endif是否匹配
bool isMatching(vector<u16string>& lines)
{
	stack<u16string> isMatchingStack;
	for (auto element: lines)
	{
		if (element == u"#ifdef" || element == u"#ifndef")
		{
			isMatchingStack.push(element);
		}
		else if(element == u"#endif") 
		{
			if (isMatchingStack.empty())
			{
				cout<<"没有#ifdef/#ifndef与之匹配a"<<endl;
				return false;
			}
			else
			{
				isMatchingStack.pop();
			}
		}
		else
		{
			continue;
		}
	}
	if (isMatchingStack.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

// 检查＃include是否都展开了
int checkTheInclude(vector<u16string>& combination)
{
	int toCount = 0;
	for (auto element: combination)
	{
		if (element == u"#include")
		{
			toCount++;
			return toCount;
		}
		else
		{
			toCount++;
		}	
	}
	toCount = 0;
	return toCount;
}

// 找头文件的名子
vector<u16string> findgetHeaderFileName(vector<u16string>& lines, int getIncludeRowtoCount)
{
	int findGetIncludeRowtoCount = 0;
	vector<u16string> vec;
	u16string u16;
	int statement = 1;
	for (auto element: lines)
	{
		if (findGetIncludeRowtoCount == getIncludeRowtoCount-1)
		{
			for (auto ele: element)
			{
				if (ele == u'<' || ele == u'"')
				{
					statement = 2;
				}
				if (statement == 2)
				{
					if (ele == u'<' || ele == u'"' || ele == u'>')
					{
						continue;
					}
					else
					{
						u16 += ele;	
					}
				}
			}
			vec.push_back(u16);
			break;
		}
		else
		{
			findGetIncludeRowtoCount++;
		}
	}
	return vec;
}

// 展开include文件
vector<u16string> expansionIncludeFile(vector<u16string>& lines)
{
	vector<u16string> isHaveInclude;
	vector<u16string> getHeaderFileName;
	int getIncludeRowtoCount = 1; // 获取include行数
	int toCount;// 让头文件按顺序插入源文件中
	isHaveInclude = getIsSharpRow(lines);
	//isMatching(isHaveInclude);
	while (getIncludeRowtoCount != 0)
	{
		u16string header;
		getIncludeRowtoCount = checkTheInclude(isHaveInclude);
		if (getIncludeRowtoCount != 0)
		{
			toCount = getIncludeRowtoCount;
			getHeaderFileName = findgetHeaderFileName(lines,getIncludeRowtoCount);
			for (auto element: getHeaderFileName)
			{
				for (auto ele: element)
				{
					header += ele;
				}
			}
			string fileName = to_bytes(header);
			if (fileName[0] != '/')
			{
				fileName = "include/" + fileName;
			}
			fstream headfile(fileName, std::ios::in);
			string line;
			vector<u16string> vecHead;
			while (getline(headfile, line))
			{
				vecHead.push_back(from_bytes(line));
			}
			isHaveInclude.erase(isHaveInclude.begin()+getIncludeRowtoCount-1);
			for (auto element: vecHead)
			{
				lines.insert(lines.begin()+toCount, element);
				toCount++;
			}
			lines.erase(lines.begin()+getIncludeRowtoCount-1);
			isHaveInclude = getIsSharpRow(lines);
		}
		
	}
	return lines;
}

// 删掉(注释)//后面的内容
vector<u16string> secondToRemoveExplabation(vector<u16string>& lines)
{
	vector<u16string> vec;
	for (auto element: lines)
	{
		u16string saveRemoveExplabationRow;
		for (auto ele: element)
		{
			if (ele != u'/')
			{
				saveRemoveExplabationRow += ele;
			}
			else
			{
				if (element[saveRemoveExplabationRow.size()-1] != u'"' && element[saveRemoveExplabationRow.size()+1] == u'/')
				{
					break;
				}
				else if(element[saveRemoveExplabationRow.size()-1] != u'"' && element[saveRemoveExplabationRow.size()+1] != u'/' && element[saveRemoveExplabationRow.size()-1] != u'/')
				{
					if (element[saveRemoveExplabationRow.size()+1] != u'*')
					{
						cout<<"程序有错"<<endl;
						saveRemoveExplabationRow += ele;
						continue;
					}
				}
				else
				{
					saveRemoveExplabationRow += ele;
				}
			}
		}
		vec.push_back(saveRemoveExplabationRow);
	}
	return vec;
}

// 去掉/*到*/之间的内容
vector<u16string> removeExplabation(vector<u16string>& lines)
{
	vector<u16string> saveRemoveExplabationFile;
	vector<u16string> isHaveInclude;
	int statement = 1;// 自动机,1表示没遇到/*,2表示已经遇到/*;3表示遇到*/
	isHaveInclude = getIsSharpRow(lines);
	isMatching(isHaveInclude);
	for (auto element: lines)
	{
		u16string saveRemoveExplabationRow;
		for (auto ele: element)
		{
			if (statement == 1)
			{
				if (ele == u'/' && element[saveRemoveExplabationRow.size()+1] == u'*')
				{
					statement = 2;
					continue;
				}
				else
				{
					saveRemoveExplabationRow += ele;
				}
			}
			else if(statement == 2 || statement == 3)
			{
				if (ele == u'*')
				{
					statement = 3;
					continue;
				}
				else if(statement == 3)
				{
					if (ele == u'/')
					{
					statement = 1;
					}
					else
					{
						statement = 2;
					}
				}
			}
		}
		saveRemoveExplabationFile.push_back(saveRemoveExplabationRow);
	}
	saveRemoveExplabationFile = secondToRemoveExplabation(saveRemoveExplabationFile);
	return saveRemoveExplabationFile;
}

//判断#define #ifdef #ifndef 在第几行
int toFindDefineRow(vector<u16string>& lines)
{
	int getDefineRow = 1;
	vector<u16string> saveSharpRow;
	saveSharpRow = getIsSharpRow(lines);
	for (auto ele: saveSharpRow)
	{
		if (ele == u"#define" || ele == u"#ifdef" || ele == u"ifndef")
		{
			break;
		}
		else
		{
			getDefineRow++;
		}
	}
	return getDefineRow;
}

//获取#define后面的内容
void getRelationContent(vector<u16string>& lines)
{
	int getRow;
	int getLinesRow;
	getRow = toFindDefineRow(lines);
	u16string saveDefineContent;
	if (getRow != 0)
	{
		for (auto element: lines)
		{
			int statement = 1;
			if (getLinesRow == getRow - 1)		
			int statement = 1;
			if (getLinesRow == getRow)
			{
				for (auto ele: element)
				{
					if (statement == 1)
					{
						if (ele == u' ')
						{
							statement = 2;
							continue;
						}
						else
						{
							continue;
						}
					}
					else if(statement == 2)
					{
						if (ele != u' ')
						{
							saveDefineContent += ele;
						}
						else
						{
							continue;
						}
					}
				}
				break;
			}
			else
			{
				getLinesRow++;
			}
			//cout<<"%"<<saveDefineContent.size()<<endl;
		}
		//cout<<"$"<<saveDefineContent.size()<<"$"<<to_bytes(saveDefineContent)<<endl;
		greatFindDefine(saveDefineContent, 1);
	}
}


int greatFindDefine(u16string u16, int statement)
{
	unordered_set<u16string> findIsDefine;
	cout<<u16.size()<<"@"<<to_bytes(u16)<<endl;
	if (statement == 1)
	{
		findIsDefine.insert(u16);
	}
	else if(statement == 2)
	{
		/*if(search != example.end()) 
		{
	
		}
		else
		{
			std::cout << "Not found\n";
		}*/
	}
	for(auto ele:findIsDefine)
	{
		cout<<to_bytes(ele)<<"sefrf"<<endl;
	}
}

//删除#define行
vector<u16string> dealDefineContent(vector<u16string>& lines)
{
	vector<u16string> saveDealResult;
	int getDefineRow;
	int getLinesRow = 1;
	getDefineRow = toFindDefineRow(lines);
	for (auto element: lines)
	{
		if (getLinesRow == getDefineRow)
		{
			getRelationContent(lines);
			//element.erase(lines.begin()+getLinesRow);
			getLinesRow ++;
		}
		else
		{
			getLinesRow ++;
			saveDealResult.push_back(element);
		}
	}
	return saveDealResult;
}

//判断#define #ifdef #ifndef那个在先
int getFirstDefineRow(vector<u16string>& lines)
{
	int saveFindRow;
	vector<u16string> toGetSharpRow;
	toGetSharpRow = getIsSharpRow(lines);
	for (auto ele: toGetSharpRow)
	{
		if (ele == u"#define")
		{
			return saveFindRow=1;
		}
		else if(ele == u"#ifdef")
		{
			return saveFindRow=2;
		}
		else if(ele == u"#ifndef")
		{
			return saveFindRow=3;
		}
		else
		{
			continue;
		}
	}
	saveFindRow = 0;
	return saveFindRow;
}

//最终处理#define语句
vector<u16string> isRelation(vector<u16string>& lines)
{
	int tofineDefineRow;
	vector<u16string> saveRelationRow;
	tofineDefineRow = getFirstDefineRow(lines);
	while (tofineDefineRow != 0)
	{
		if (tofineDefineRow == 1)
		{
			saveRelationRow = dealDefineContent(lines);
			break;
		}
	}
	return saveRelationRow;
}

int main()
{
	fstream getSourceFile("test/a.cpp", std::ios::in);
	string getSourceFileEachRow;
	vector<u16string> lines;
	while (getline(getSourceFile, getSourceFileEachRow))
	{
		lines.push_back(from_bytes(getSourceFileEachRow));
	}
	lines = removeBlank(lines);
	lines = removeExplabation(lines);
	lines = expansionIncludeFile(lines);
	lines = removeExplabation(lines);
	lines = isRelation(lines);
	/*for (auto element: lines)
	{
		cout<<to_bytes(element)<<endl;
	}*/
	return 0;
}