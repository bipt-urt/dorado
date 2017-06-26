// Dorado编译器工具集
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
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

#include "u16string.h"
#include "preProcess.h"

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
	for (auto element: lines)
	{
		cout<<to_bytes(element)<<endl;
	}
	return 0;
}