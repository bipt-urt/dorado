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
	switch (_errorNumber)
	{
		case 100:
			doradoError(u"文件不存在。通过Dorado编译器接收到的参数编译文件并试图打开，但文件不存在。");
			break;
		case 101:
			doradoError(u"错误的命令行参数指令。用户向Dorado编译器发出的命令行参数中的指令无效，Dorado编译器无法解释它。");
			break;
		case 102:
			doradoError(u"错误的命令行参数值。用户向Dorado编译器发出的命令行参数中的值无效，Dorado编译器无法解释它。");
			break;
		case 103:
			doradoError(u"权限不足。在Dorado编译器试图打开、写入、修改文件时，需要权限不足的问题，通过使用超级用户（sudo或su）来重新执行操作可能会解决该问题。");
			break;
		case 104:
			doradoError(u"内存不足。在Dorado编译器的执行期间，由于外部因素导致Dorado编译器无法分配所需的内存。尝试重新启动计算机或者更改程序可能的内存空间大小可能会有助于解决该问题。");
			break;
		case 105:
			doradoError(u"内部错误。Dorado编译器发生了内部错误。");
			break;
		case 200:
			doradoError(u"#include指令存在递归。当#include的文件拓扑具有A→A或者A→B→A的形式时，Dorado编译器无法将其展开。");
			break;
		case 201:
			cerr<<to_bytes(u"#include指令递归超过缺省阈值。#include的包含层数可能过多，Dorado编译器在展开#include时可能耗费大量时间。")<<endl;
			break;
		case 202:
			doradoError(u"#include指令包含文件出现错误。Dorado在处理由#include指令包含的文件时发生了错误。如果该问题持续发生，请检查所包含的文件。");
			break;
		case 203:
			doradoError(u"#include文件不存在。由#include指定的文件无法由Dorado编译器找到。");
			break;
		case 204:
			doradoError(u"#include文件权限不足。由#include指定的文件因为权限不足，无法由Dorado编译器展开，通过使用超级用户（sudo或su）来重新执行操作可能会解决该问题。");
			break;
		case 205:
			doradoError(u"#define重复定义。两个或者更多#define语句重复定义了一个预编译期变量。");
			break;
		case 206:
			cerr<<to_bytes(u"#undef删除了不存在的预编译期变量。")<<endl;
			break;
		case 207:
			doradoError(u"#ifdef和/或#ifndef和#endif无法配对。这通常是由于笔误引起的。试着检查条件编译指示符的配对情况。");
			break;
		case 208:
			cerr<<to_bytes(u"条件编译互斥。由一条或多条#ifdef和/或#ifndef带来的条件编译指示符逻辑上产生了互斥，相关代码将不会被编译。");
			break;
		case 300:
			cerr<<to_bytes(u"该行缺少分号。这可能是由于换行引起的。Dorado编译器建议将语句写于一行中。")<<endl;
			break;
		case 301:
			doradoError(u"错误的变量格式。Dorado编译器无法接受变量或字面量由数字开头，紧邻字母的形式。");
			break;
		case 302:
			doradoError(u"重复的变量定义。Dorado编译器检测到代码中有重复的变量定义，这是不被允许的。");
			break;
		case 400:
			cerr<<to_bytes(u"汇编数据精度不足。在汇编语句数字字面量中，由于位数不足，将以0补足。")<<endl;
			break;
		case 401:
			cerr<<to_bytes(u"汇编数据精度丢失。在汇编语句数字字面量中，由于位数太多，多余的位数将会舍去，这可能会导致丢失精度。")<<endl;
			break;
		case 402:
			doradoError(u"未知的寄存器名。Dorado汇编器遇到了未知的寄存器名，Dorado汇编器无法解释它。");
			break;
		case 403:
			doradoError(u"未知的指令。Dorado汇编器遇到了未知的指令名，Dorado汇编器无法解释它。");
			break;
		default:
			doradoError(u"未知错误。");
			break;
	}
	return;
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
