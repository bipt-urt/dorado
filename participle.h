struct word
{
	u16string _word;
	u16string _type;
};

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
		{u';', {u"endl"}}
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

vector<word> wordSegment(const u16string& line)
{
	vector<word> res;
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
				== savaParticiple.end())
			{
				saveWord += element;
			}
			else
			{
				getWordAndType.push_back(saveWord);
				saveWord = u" ";
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
		getWordAndType.push_back(saveWord);
	}
	for (auto element: getWordAndType)
	{
		cout<<to_bytes(element)<<"@";
	}
	return res;
}