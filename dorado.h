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