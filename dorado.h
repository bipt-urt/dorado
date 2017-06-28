void doradoError(const u16string& _information = u"")
{
	if (_information.size())
	{
		cerr<<to_bytes(_information)<<endl;
	}
	const int exitFailure = 1;
	exit(exitFailure);
}

void doradoError(const int& _errorNumber)
{
	
}

void cmdProcess(int argc, char* argv[])
{
	
}

void showCode(const vector<u16string>& _lines)
{
	int rowNumber = 0;
	for (auto line: _lines)
	{
		cout<<"["<<rowNumber++<<"]\t"<<to_bytes(line)<<endl;
	}
	return;
}

u16string intToHexString(const int& _number)
{
	stringstream cvt;
	cvt<<hex<<_number;
	string res;
	u16string res16;
	cvt>>res;
	res16 = from_bytes(res);
	return res16;
}

u16string intToDecString(const int& _number)
{
	stringstream cvt;
	cvt<<_number;
	string res;
	u16string res16;
	cvt>>res;
	res16 = from_bytes(res);
	return res16;
}

int stringToInt(const u16string& _str)
{
	int res;
	stringstream cvt;
	cvt<<to_bytes(_str);
	cvt>>res;
	return res;
}
