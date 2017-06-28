struct word
{
	u16string _word;
	u16string _type;
};

u16string keywordTable(const u16string& element)
{
	u16string empty;
	map<u16string, u16string> maps = 
	{
		{u"int", u"type"}, {u"char", u"type"},
		{u"=", u"operator"}, {u"+", u"operator"}, {u"*", u"operator"},
		{u"/", u"operator"}, {u"-", u"operator"},
		{u"if", u"ifSentence"}, {u"while", u"whileSentence"},
		{u"(", u"lBracket"}, {u")", u"rBracket"},
		{u";", u"endl"},{u"[", u"lBracket"}, {u"]", u"rBracket"},
	};
	auto search = maps.find(element);
	if (search != maps.end()) 
	{
		return search->second;
	}
	else
	{
		return empty;
	}
}

void toJudge(vector<word>& word);
vector<u16string> creatAssemblyCode(vector<word>& _code);

vector<u16string> getCategory(const char16_t& word)
{
	vector<u16string> empty;
	map<char16_t, vector<u16string>> lookUpTable = {
		{u'a', {u"alphabet"}}, {u'b', {u"alphabet"}}, {u'c', {u"alphabet"}},
		{u'd', {u"alphabet"}}, {u'e', {u"alphabet"}}, {u'f', {u"alphabet"}},
		{u'g', {u"alphabet"}}, {u'h', {u"alphabet"}}, {u'i', {u"alphabet"}},
		{u'j', {u"alphabet"}}, {u'k', {u"alphabet"}}, {u'l', {u"alphabet"}},
		{u'm', {u"alphabet"}}, {u'n', {u"alphabet"}}, {u'o', {u"alphabet"}},
		{u'p', {u"alphabet"}}, {u'q', {u"alphabet"}}, {u'r', {u"alphabet"}},
		{u's', {u"alphabet"}}, {u't', {u"alphabet"}}, {u'u', {u"alphabet"}},
		{u'v', {u"alphabet"}}, {u'w', {u"alphabet"}}, {u'x', {u"alphabet"}},
		{u'y', {u"alphabet"}}, {u'z', {u"alphabet"}},
		{u'A', {u"alphabet"}}, {u'B', {u"alphabet"}}, {u'C', {u"alphabet"}},
		{u'D', {u"alphabet"}}, {u'E', {u"alphabet"}}, {u'F', {u"alphabet"}},
		{u'G', {u"alphabet"}}, {u'H', {u"alphabet"}}, {u'I', {u"alphabet"}},
		{u'J', {u"alphabet"}}, {u'K', {u"alphabet"}}, {u'L', {u"alphabet"}},
		{u'M', {u"alphabet"}}, {u'N', {u"alphabet"}}, {u'O', {u"alphabet"}},
		{u'P', {u"alphabet"}}, {u'Q', {u"alphabet"}}, {u'R', {u"alphabet"}},
		{u'S', {u"alphabet"}}, {u'T', {u"alphabet"}}, {u'U', {u"alphabet"}},
		{u'V', {u"alphabet"}}, {u'W', {u"alphabet"}}, {u'X', {u"alphabet"}},
		{u'Y', {u"alphabet"}}, {u'Z', {u"alphabet"}},
		{u'0', {u"number",u"alphabet"}}, {u'1', {u"number",u"alphabet"}},
		{u'2', {u"number",u"alphabet"}}, {u'3', {u"number",u"alphabet"}},
		{u'4', {u"number",u"alphabet"}}, {u'5', {u"number",u"alphabet"}},
		{u'6', {u"number",u"alphabet"}}, {u'7', {u"number",u"alphabet"}},
		{u'8', {u"number",u"alphabet"}}, {u'9', {u"number",u"alphabet"}},
		{u'=', {u"operator"}},
		{u';', {u"endl"}},
		{u'[', {u"lBracket"}}, {u']', {u"rBracket"}},
		{u'(', {u"lBracket"}}, {u')', {u"rBracket"}},
		{u'_', {u"alphabet"}},
		{u'-', {u"operator"}},{u'+', {u"operator"}},{u'*', {u"operator"}},
		{u'/', {u"operator"}}
	};
	auto search = lookUpTable.find(word);
	if (search != lookUpTable.end()) 
	{
		return search->second;
	}
	else
	{
		return empty;
	}
}

u16string variableLabel(const u16string& _element)
{
	u16string empty;
	vector<u16string> temp;
	for (auto ele: _element)
	{
		temp = getCategory(ele);
		if (temp[0] == u"number")
		{
			return u"number";
		}
		else
		{
			return u"variable";
		}
		break;
	}
}

vector<u16string> wordSegment(const u16string& line)
{
	vector<word> res;
	u16string temp;
	vector<u16string> returnAssemblyCode;
	word block;
	vector<u16string> getWordAndType;
	u16string saveWord;
	u16string saveElementAttribute;
	for (auto element: line)
	{
		vector<u16string> savaParticiple = getCategory(element);
		if (saveWord.empty())
		{
			if (element == u' ' || element == u'\t')
			{
				continue;
			}
			else
			{
				saveWord += element;
				saveElementAttribute = savaParticiple[0];
			}
		}
		else
		{
			if (find(savaParticiple.begin(),
					savaParticiple.end(), saveElementAttribute)
				!= savaParticiple.end())
			{
				saveWord += element;
			}
			else
			{
				
				getWordAndType.push_back(saveWord);
				saveWord = u"";
				if (element == u' ' || element == u'\t')
				{
					continue;
				}
				else
				{
					saveWord += element;
					saveElementAttribute = savaParticiple[0];
				}
			}
		}
	}
	if (saveWord.size())
	{
		if (saveWord == u";")
		{
			getWordAndType.push_back(saveWord);
		}
		else
		{
			getWordAndType.push_back(saveWord);
			doradoError(300);
		}
	}
	if (getCategory((getWordAndType[0])[0])[0] == u"number")
	{
		doradoError(301);
	}
	for (auto element: getWordAndType)
	{
		 
		block._word = element;
		block._type = keywordTable(element);
		if (block._type.empty())
		{
			block._type = variableLabel(element);
			if (element == u"mem")
			{
				block._type = u"menSentence";
			}
		}
		res.push_back(block);
	}
	toJudge(res);
	returnAssemblyCode = creatAssemblyCode(res);
	return returnAssemblyCode;
}

vector<u16string> creatAssemblyCode(vector<word>& _code)
{
	vector<u16string> tempCode;
	if (_code[0]._word == u"mem")
	{
		tempCode.push_back(u"LI R1 "+_code[5]._word);
		tempCode.push_back(u"LI R0 "+_code[5]._word);
	}
	return tempCode;
}

//变量表
bool variableTable(u16string& var, int funtion)
{
	static int addr = 0xbf00;
	static map<u16string, int> varTable;
	if (funtion == 1)
	{
		auto search = varTable.find(var);
		if (search != varTable.end())
		{
			doradoError(302);
		}
		else
		{
			varTable.insert(pair<u16string, int>(var, addr++));
		}
	}
	else if(funtion == 2)
	{
		auto search = varTable.find(var);
		if (search != varTable.end())
		{
			return 0;
		}
		else
		{
			doradoError(302);
		}
	}
	return 0;
}

//判断定义语句
void judgeInt(vector<word>& word)
{
	if (word[1]._type == u"number")
	{
		doradoError(301);
	}
	for (auto element: word)
	{
		if (element._type == u"variable")
		{
			variableTable(element._word,1);
		}
	}
}

//判断赋值语句
void judgeVariable(vector<word>& word)
{
	if (word.size() == 4)
	{
		variableTable(word[1]._word,2);
	}
	else
	{
		doradoError(301);
	}
}


void toJudge(vector<word>& _word)
{
	for (auto element: _word)
	{
		if (element._type == u"type")
		{
			judgeInt(_word);
		}
		else if(element._type == u"variable")
		{
			judgeVariable(_word);
		}
	}
}



