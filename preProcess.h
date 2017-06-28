// 去掉空格
vector<u16string> removeBlank(vector<u16string>& _lines)
{
	vector<u16string> removeBlankSourceFile;
	for (auto element: _lines)
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
vector<u16string> getIsSharpRow(vector<u16string>& _lines)
{
	vector<u16string> saveHaveSharpRow;
	for (auto element: _lines)
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
void isMatching(vector<u16string>& _lines)
{
	vector<u16string> isHaveInclude;
	stack<u16string> isMatchingStack;
	isHaveInclude = getIsSharpRow(_lines);
	for (auto element: isHaveInclude)
	{
		if (element == u"#ifdef" || element == u"#ifndef")
		{
			isMatchingStack.push(element);
		}
		else if(element == u"#endif") 
		{
			if (isMatchingStack.empty())
			{
				doradoError(207);
			}
			else
			{
				isMatchingStack.pop();
			}
		}
	}
	if(!isMatchingStack.empty())
	{
		doradoError(207);
	}
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

// 找头文件的名字
vector<u16string> findgetHeaderFileName(vector<u16string>& _lines, const int& _getIncludeRowtoCount)
{
	int findGetIncludeRowtoCount = 0;
	vector<u16string> vec;
	u16string u16;
	int statement = 1;
	for (auto element: _lines)
	{
		if (findGetIncludeRowtoCount == _getIncludeRowtoCount-1)
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
vector<u16string> expansionIncludeFile(vector<u16string>& _lines)
{
	vector<u16string> isHaveInclude;
	vector<u16string> getHeaderFileName;
	int getIncludeRowtoCount = 1; // 获取include行数
	int toCount;// 让头文件按顺序插入源文件中
	isHaveInclude = getIsSharpRow(_lines);
	while (getIncludeRowtoCount != 0)
	{
		u16string header;
		getIncludeRowtoCount = checkTheInclude(isHaveInclude);
		if (getIncludeRowtoCount != 0)
		{
			toCount = getIncludeRowtoCount;
			getHeaderFileName = findgetHeaderFileName(_lines,getIncludeRowtoCount);
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
				_lines.insert(_lines.begin()+toCount, element);
				toCount++;
			}
			_lines.erase(_lines.begin()+getIncludeRowtoCount-1);
			isHaveInclude = getIsSharpRow(_lines);
		}
		
	}
	return _lines;
}

// 删掉(注释)//后面的内容
vector<u16string> secondToRemoveExplabation(vector<u16string>& _lines)
{
	vector<u16string> vec;
	for (auto element: _lines)
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
				else if(element[saveRemoveExplabationRow.size()-1] != u'"' &&
						element[saveRemoveExplabationRow.size()+1] != u'/' &&
						element[saveRemoveExplabationRow.size()-1] != u'/')
				{
					if (element[saveRemoveExplabationRow.size()+1] != u'*')
					{
						doradoError(209);
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
vector<u16string> removeExplabation(vector<u16string>& _lines)
{
	vector<u16string> saveRemoveExplabationFile;
	vector<u16string> isHaveInclude;
	int statement = 1;// 自动机,1表示没遇到/*,2表示已经遇到/*;3表示遇到*/
	isMatching(_lines);
	for (auto element: _lines)
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

// 判断#define/#ifdef/#ifndef/#undef在第几行
int toFindDefineRow(vector<u16string>& _lines)
{
	int getDefineRow = 1;
	vector<u16string> saveSharpRow;
	saveSharpRow = getIsSharpRow(_lines);
	for (auto ele: saveSharpRow)
	{
		if (ele == u"#define" || ele == u"#ifdef" || ele == u"#ifndef" || ele == u"#undef")
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

// 获取#define/#undef/#ifdef/#ifndef后面的内容
u16string getRelationContent(vector<u16string>& _lines)
{
	int getRow;
	int getLinesRow = 1;
	u16string saveDefineContent;
	u16string text;
	getRow = toFindDefineRow(_lines);
	if (getRow != 0)
	{
		for (auto element: _lines)
		{
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
					}
					else if(statement == 2)
					{
						if (ele == u' ' || ele == u'\t')
						{
							continue;
						}
						else
						{
							saveDefineContent += ele;
						}
					}
				}
				return saveDefineContent;
			}
			else
			{
				getLinesRow++;
			}
		}
	}
	return saveDefineContent;
}

// 创建查找表
bool greatFindDefineChart(const u16string& u16, const int& _statement)
{
	static unordered_set<u16string> findIsDefine;
	if (_statement == 1)
	{
		findIsDefine.insert(u16);
	}
	else if(_statement == 2)
	{
		findIsDefine.erase(u16);
	}
	else if(_statement == 3)
	{
		if(find(findIsDefine.begin(), findIsDefine.end(), u16) != findIsDefine.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// 删除或者插入查找表的元素
void insertOrDeleteChart(vector<u16string> _lines, const int& statement)
{
	u16string getDefineContent;
	getDefineContent = getRelationContent(_lines);
	greatFindDefineChart(getDefineContent, statement);
}

// 删除#define行
vector<u16string> dealDefineContent(vector<u16string>& _lines, const int& _statement)
{
	vector<u16string> saveDealResult;
	int getDefineRow;
	int getLinesRow = 1;
	getDefineRow = toFindDefineRow(_lines);
	for (auto element: _lines)
	{
		if (getLinesRow == getDefineRow)
		{
			insertOrDeleteChart(_lines, _statement);
			getLinesRow++;
		}
		else
		{
			getLinesRow++;
			saveDealResult.push_back(element);
		}
	}
	return saveDealResult;
}

// 判断#define #ifdef #ifndef那个在先
int getFirstDefineRow(vector<u16string>& _lines)
{
	int saveFindRow;
	vector<u16string> toGetSharpRow;
	toGetSharpRow = getIsSharpRow(_lines);
	for (auto ele: toGetSharpRow)
	{
		if (ele == u"#define")
		{
			return saveFindRow = 1;
		}
		else if(ele == u"#undef")
		{
			return saveFindRow = 2;
		}
		else if(ele == u"#ifdef")
		{
			return saveFindRow = 3;
		}
		else if(ele == u"#ifndef")
		{
			return saveFindRow = 4;
		}
	}
	saveFindRow = 0;
	return saveFindRow;
}

// 判断是否删除#ifdef的内容
bool isDeleteIfdefContent(vector<u16string>& _lines)
{
	int getIfdefRowNumber;
	u16string ifdefContent;
	ifdefContent = getRelationContent(_lines);
	return greatFindDefineChart(ifdefContent, 3);
}

// 找最后一个endif
int toFindEndifRow(vector<u16string>& _lines)
{
	int lastEndifRow = 1;
	vector<u16string> isHaveInclude;
	stack<u16string> isMatchingStack;
	isHaveInclude = getIsSharpRow(_lines);
	for (auto element: isHaveInclude)
	{
		if (element == u"#ifdef" || element == u"#ifndef")
		{
			isMatchingStack.push(element);
			lastEndifRow++;
		}
		else if(element == u"#endif") 
		{
			isMatchingStack.pop();
			lastEndifRow++;
			if(isMatchingStack.empty())
			{
				return lastEndifRow-1;
			}
		}
		else
		{
			lastEndifRow++;
		}
	}
	return lastEndifRow;
}

// 删除#ifndef与否
vector<u16string> dealIfndefContent(vector<u16string>& _lines, const bool& _statement)
{
	vector<u16string> saveDealIfdef;
	bool isDelete;
	int getIfdefRowNumber = 0;
	int getEndifRowNumber = 0;
	int isWhereRow =1 ;
	getIfdefRowNumber = toFindDefineRow(_lines);
	getEndifRowNumber = toFindEndifRow(_lines);
	isDelete = isDeleteIfdefContent(_lines);
	for (auto element: _lines)
	{
		if (isWhereRow == getIfdefRowNumber )
		{
			isWhereRow++;
		}
		else if(isWhereRow > getIfdefRowNumber && isWhereRow < getEndifRowNumber)
		{
			if (isDelete == 0)
			{
				saveDealIfdef.push_back(element);
				isWhereRow++; 
			}
			else
			{
				isWhereRow++;
			}
		}
		else if(isWhereRow == getEndifRowNumber)
		{
			isWhereRow++;
		}
		else 
		{
			isWhereRow++;
			saveDealIfdef.push_back(element);
		}
	}
	return saveDealIfdef;
}


// 删除#ifdef与否
vector<u16string> dealIfdefContent(vector<u16string>& _lines, const bool& _statement)
{
	vector<u16string> saveDealIfdef;
	bool isDelete;
	int getIfdefRowNumber = 0;
	int getEndifRowNumber = 0;
	int isWhereRow =1 ;
	getIfdefRowNumber = toFindDefineRow(_lines);
	getEndifRowNumber = toFindEndifRow(_lines);
	isDelete = isDeleteIfdefContent(_lines);
	for (auto element: _lines)
	{
		if (isWhereRow == getIfdefRowNumber)
		{
			isWhereRow++;
		}
		else if(isWhereRow > getIfdefRowNumber && isWhereRow < getEndifRowNumber)
		{
			if (isDelete == 1)
			{
				saveDealIfdef.push_back(element);
				isWhereRow++; 
			}
			else
			{
				isWhereRow++;
			}
		}
		else if(isWhereRow == getEndifRowNumber)
		{
			isWhereRow++;
		}
		else 
		{
			isWhereRow++;
			saveDealIfdef.push_back(element);
		}
	}
	return saveDealIfdef;
}

// 最终处理#define语句
vector<u16string> isRelation(vector<u16string>& _lines)
{
	int tofineDefineRow;
	vector<u16string> saveRelationRow;
	tofineDefineRow = getFirstDefineRow(_lines);
	saveRelationRow = _lines;
	while (tofineDefineRow != 0)
	{
		if (tofineDefineRow == 1)
		{
			saveRelationRow = dealDefineContent(saveRelationRow, 1);
			tofineDefineRow = getFirstDefineRow(saveRelationRow);
			continue;
		}
		else if(tofineDefineRow == 2)
		{
			saveRelationRow = dealDefineContent(saveRelationRow, 2);
			tofineDefineRow = getFirstDefineRow(saveRelationRow);
			continue;
		}
		else if(tofineDefineRow == 3)
		{
			saveRelationRow = dealIfdefContent(saveRelationRow, 3);
			tofineDefineRow = getFirstDefineRow(saveRelationRow);
		}
		else if(tofineDefineRow == 4)
		{
			saveRelationRow = dealIfndefContent(saveRelationRow, 3);
			tofineDefineRow = getFirstDefineRow(saveRelationRow);
			break;
		}
	}
	return saveRelationRow;
}

// 这是预处理程序的主函数
vector<u16string> preProcess(vector<u16string>& _lines)
{
	_lines = removeBlank(_lines);
	_lines = removeExplabation(_lines);
	_lines = expansionIncludeFile(_lines);
	_lines = removeExplabation(_lines);
	_lines = isRelation(_lines);
	return _lines;
}