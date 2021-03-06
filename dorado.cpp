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
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

#include "u16string.h"
#include "dorado.h"
#include "preProcess.h"
#include "splitBlock.h"
#include "participle.h"
#include "msm.h"

int main(int argc, char* argv[])
{
	fstream getSourceFile("test/a.cpp", std::ios::in);
	string getSourceFileEachRow;
	vector<u16string> lines;
	while (getline(getSourceFile, getSourceFileEachRow))
	{
		lines.push_back(from_bytes(getSourceFileEachRow));
	}
	preProcess(lines);
	auto blocks = splitBlock(lines);
	auto mainLines = fetchMainFunction(blocks);
	vector<string> mips;
	for (auto line: mainLines)
	{
		vector<u16string> mips16 = wordSegment(line);
		for (auto element: mips16)
		{
			mips.push_back(to_bytes(element));
		}
	}
	mips.push_back("end");
	msm(mips);
	return 0;
}