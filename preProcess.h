//预处理完成


bool greatFindDefineChart(u16string u16, int statement);

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
void isMatching(vector<u16string>& lines)
{
	vector<u16string> isHaveInclude;
	stack<u16string> isMatchingStack;
	isHaveInclude = getIsSharpRow(lines);
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
				doradoError(u"没有#ifdef/#ifndef与之匹配");
			}
			else
			{
				isMatchingStack.pop();
			}
		}
	}
	if(!isMatchingStack.empty())
	{
		doradoError(u"缺少endif");
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
						doradoError(u"程序有错");
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
	int statement = 1;// 自动机,1表示没遇到'/*',2表示已经遇到'/*';3表示遇到'*/'
	isMatching(lines);
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

//判断#define/#ifdef/#ifndef/#undef在第几行
int toFindDefineRow(vector<u16string>& lines)
{
	int getDefineRow = 1;
	vector<u16string> saveSharpRow;
	saveSharpRow = getIsSharpRow(lines);
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

//获取#define/#undef/#ifdef/#ifndef后面的内容
u16string getRelationContent(vector<u16string>& lines)
{
	int getRow;
	int getLinesRow = 1;
	u16string saveDefineContent;
	u16string text;
	getRow = toFindDefineRow(lines);
	if (getRow != 0)
	{
		for (auto element: lines)
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
				break;
			}
			else
			{
				getLinesRow++;
			}
		}
	}
	return saveDefineContent;
}

//删除或者插入查找表的元素
void insertOrDeleteChart(vector<u16string> lines, int statement)
{
	u16string getDefineContent;
	getDefineContent = getRelationContent(lines);
	greatFindDefineChart(getDefineContent, statement);
}

//创建查找表
bool greatFindDefineChart(u16string u16, int statement)
{
	static unordered_set<u16string> findIsDefine;
	if (statement == 1)
	{
		findIsDefine.insert(u16);
	}
	else if(statement == 2)
	{
		findIsDefine.erase(u16);
	}
	else if(statement == 3)
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

//删除#define行
vector<u16string> dealDefineContent(vector<u16string>& lines, int statement)
{
	vector<u16string> saveDealResult;
	int getDefineRow;
	int getLinesRow = 1;
	getDefineRow = toFindDefineRow(lines);
	for (auto element: lines)
	{
		if (getLinesRow == getDefineRow)
		{
			insertOrDeleteChart(lines, statement);
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
		else if(ele == u"#undef")
		{
			return saveFindRow=2;
		}
		else if(ele == u"#ifdef")
		{
			return saveFindRow=3;
		}
		else if(ele == u"#ifndef")
		{
			return saveFindRow=4;
		}
	}
	saveFindRow = 0;
	return saveFindRow;
}

//判断是否删除#ifdef的内容
bool isDeleteIfdefContent(vector<u16string>& lines)
{
	int getIfdefRowNumber;
	u16string ifdefContent;
	ifdefContent = getRelationContent(lines);
	return greatFindDefineChart(ifdefContent,3);
}

//找最后一个endif
int toFindEndifRow(vector<u16string>& lines)
{
	int lastEndifRow = 1;
	vector<u16string> isHaveInclude;
	stack<u16string> isMatchingStack;
	isHaveInclude = getIsSharpRow(lines);
	for (auto element: isHaveInclude)
	{
		if (element == u"#ifdef" || element == u"#ifndef")
		{
			isMatchingStack.push(element);
			lastEndifRow ++;
		}
		else if(element == u"#endif") 
		{
				isMatchingStack.pop();
				lastEndifRow ++;
				if(isMatchingStack.empty())
				{
					return lastEndifRow - 1;
				}
		}
		else
		{
			lastEndifRow ++;
		}
	}
	return lastEndifRow;
}

//删除#ifndef与否
vector<u16string> dealIfndefContent(vector<u16string>& lines,bool statement)
{
	vector<u16string> saveDealIfdef;
	bool isDelete;
	int getIfdefRowNumber = 0;
	int getEndifRowNumber = 0;
	int isWhereRow =1 ;
	getIfdefRowNumber = toFindDefineRow(lines);
	getEndifRowNumber = toFindEndifRow(lines);
	isDelete = isDeleteIfdefContent(lines);
	for (auto element: lines)
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


//删除#ifdef与否
vector<u16string> dealIfdefContent(vector<u16string>& lines,bool statement)
{
	vector<u16string> saveDealIfdef;
	bool isDelete;
	int getIfdefRowNumber = 0;
	int getEndifRowNumber = 0;
	int isWhereRow =1 ;
	getIfdefRowNumber = toFindDefineRow(lines);
	getEndifRowNumber = toFindEndifRow(lines);
	isDelete = isDeleteIfdefContent(lines);
	for (auto element: lines)
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

//最终处理#define语句
vector<u16string> isRelation(vector<u16string>& lines)
{
	int tofineDefineRow;
	vector<u16string> saveRelationRow;
	tofineDefineRow = getFirstDefineRow(lines);
	saveRelationRow = lines;
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