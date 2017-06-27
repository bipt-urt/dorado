struct blockSegment
{
	public:
		vector<u16string> lines;
		u16string type;
};

bool isAlpha(const char16_t& _char)
{
	vector<char16_t> alphabet =
	{
		u'a', u'b', u'c', u'd', u'e',
		u'f', u'g', u'h', u'i', u'j',
		u'k', u'l', u'm', u'n', u'o',
		u'p', u'q', u'r', u's', u't',
		u'u', u'v', u'w', u'x', u'y',
		u'z', u'A', u'B', u'C', u'D',
		u'E', u'F', u'G', u'H', u'I',
		u'J', u'K', u'L', u'M', u'N',
		u'O', u'P', u'Q', u'R', u'S',
		u'T', u'U', u'V', u'W', u'X',
		u'Y', u'Z'
	};
	return find(alphabet.begin(), alphabet.end(), _char) != alphabet.end();
}

// 查找该行最近的分隔符, 特别地, 为了区分函数的声明和定义, 函数的声明将返回(, 函数的定义将返回)
char16_t findSplitSymbol(const vector<u16string>& _lines, const int& rowId = 0)
{
	bool isFunction = false;
	for (auto i=_lines.begin()+rowId; i!=_lines.end(); i++)
	{
		for (auto letter: *i)
		{
			if (letter == u'{' || letter == u';')
			{
				if (!isFunction)
				{
					return letter;
				}
				else
				{
					if (letter == u'{')
					{
						return u')';
					}
					else
					{
						return u'(';
					}
				}
			}
			else if (letter == u'(')
			{
				isFunction = true;
			}
		}
	}
	return u'\0';
}

u16string getFirstWord(const u16string& _str)
{
	u16string res;
	for (auto letter: _str)
	{
		if (letter == u' ' || !isAlpha(letter))
		{
			break;
		}
		else
		{
			res += letter;
		}
	}
	return res;
}

vector<blockSegment> splitBlock(const vector<u16string>& _lines)
{
	vector<blockSegment> res;
	vector<int> splitLineNumber;
	int blockLevel = 0;
	int rowNumber = -1;
	for (auto line: _lines)
	{
		rowNumber++;
		if (line.empty())
		{
			continue;
		}
		if (line[0] == u'{')
		{
			blockLevel++;
		}
		else if(line[0] == u'}')
		{
			blockLevel--;
			continue;
		}
		if (blockLevel)
		{
			continue;
		}
		splitLineNumber.push_back(rowNumber);
	}
	for (auto element: splitLineNumber)
	{
		cout<<to_bytes(_lines[element])<<endl;
	}
	return res;
}