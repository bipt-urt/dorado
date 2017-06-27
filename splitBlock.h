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
	return find(alphabet.begin(), alphabet.end(), _char)!=alphabet.end();
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
	for (auto line: _lines)
	{
		if (line.empty())
		{
			continue;
		}
		cout<<to_bytes(getFirstWord(line))<<endl;
	}
	return res;
}