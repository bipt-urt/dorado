void doradoError(const u16string& _information = u"")
{
	if (_information.size())
	{
		cerr<<to_bytes(_information)<<endl;
	}
	const int exitFailure = 1;
	exit(exitFailure);
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