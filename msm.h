// THCO-MIPS二进制编译器
// 王万霖 dgideas@outlook.com
// 2017年6月
// g++ 1.cpp -o 1 --std=c++11
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cctype>
#include <cstdlib>

using namespace std;

void errorExit(const string& _reason = "")
{
	if (!_reason.empty())
	{
		cout<<"错误: "<<_reason;
	}
	else
	{
		cout<<"发生了错误";
	}
	cout<<endl;
	exit(0);
	return;
}

string strToLower(const string& _raw)
{
	string res = "";
	for (const auto& letter: _raw)
	{
		res += tolower(letter);
	}
	return res;
}

string hexToBin(string _hex, const int& _length)
{
	string res = "";
	map<char, string> binTransfer = {
		{'0', "0000"}, {'1', "0001"},
		{'2', "0010"}, {'3', "0011"},
		{'4', "0100"}, {'5', "0101"},
		{'6', "0110"}, {'7', "0111"},
		{'8', "1000"}, {'9', "1001"},
		{'a', "1010"}, {'b', "1011"},
		{'c', "1100"}, {'d', "1101"},
		{'e', "1110"}, {'f', "1111"}
	};
	int bitCount = _length/4 + (_length%4 ? 1 : 0);
	int count = 0;
	if (_hex.substr(0, 2) == "0x")
	{
		_hex = _hex.substr(2);
	}
	else if (_hex.substr(0,1) == "x")
	{
		_hex = _hex.substr(1);
	}
	
	for (string::const_reverse_iterator i = _hex.crbegin();
		i != _hex.crend();
		i++)
	{
		if (count++ == bitCount)
		{
			break;
		}
		res = binTransfer[*i] + res;
	}
	if (res.size() != _length)
	{
		if (_length > res.size())
		{
			string tmp(_length-res.size(), '0');
			res = tmp + res;
			cout<<"(2001)数据精度不足警告: 在数据"<<_hex<<"中, 由于位数不足"
				<<_length<<"位, 将以0补足."<<endl;
		}
		else
		{
			res = res.substr(res.size()-_length);
			cout<<"(2002)数据长度警告: 在数据"<<_hex<<"中, 由于位数大于"
				<<_length<<"位,可能会丢失精度."<<endl;
		}
	}
	else if(_hex.size()*4 > _length)
	{
		cout<<"(2002)数据长度警告: 在数据"<<_hex<<"中, 由于位数大于"
			<<_length<<"位,可能会丢失精度."<<endl;
	}
	return res;
}

char segToBin(const string& _seg)
{
	char ch = 0;
	int weight = _seg.size()-1;
	for (const auto& bit: _seg)
	{
		if (bit == '1')
		{
			ch += 1<<weight;
		}
		weight--;
	}
	return ch;
}

string toBin(const string& _bin)
{
	string res = "";
	bool bigEnd = false;
	char highByte = segToBin(_bin.substr(0, 8));
	char lowByte = segToBin(_bin.substr(8));
	if (bigEnd)
	{
		res += highByte;
		res += lowByte;
	}
	else
	{
		res += lowByte;
		res += highByte;
	}
	return res;
}

string rx(const string& _regFileNumber)
{
	if (_regFileNumber == "r0")
	{
		return "000";
	}
	else if(_regFileNumber == "r1")
	{
		return "001";
	}
	else if(_regFileNumber == "r2")
	{
		return "010";
	}
	else if(_regFileNumber == "r3")
	{
		return "011";
	}
	else if(_regFileNumber == "r4")
	{
		 return "100";
	}
	else if(_regFileNumber == "r5")
	{
		return "101";
	}
	else if(_regFileNumber == "r6")
	{
		return "110";
	}
	else if(_regFileNumber == "r7")
	{
		return "111";
	}
	else
	{
		errorExit("(1002)寄存器错误: 未知的寄存器名" + _regFileNumber);
	}
}

string imm(const string& _imm, const int length = 8)
{
	return hexToBin(_imm, length);
}

string translate(const vector<string>& _ins)
{
	string res = "";
	string instruction = _ins[0];
	if (instruction == "addiu")
	{
		res = "01001" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "addiu3")
	{
		res = "01000" + rx(_ins[1]) + rx(_ins[2]) + "0" + imm(_ins[3], 4);
	}
	else if(instruction == "addsp3")
	{
		res = "00000" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "addsp")
	{
		res = "01100011" + imm(_ins[1]);
	}
	else if(instruction == "addu")
	{
		res = "11100" + rx(_ins[1]) + rx(_ins[2]) + rx(_ins[3]) + "01";
	}
	else if(instruction == "add")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "01100";
	}
	else if(instruction == "b")
	{
		res = "00010" + imm(_ins[1], 11);
	}
	else if(instruction == "beqz")
	{
		res = "00100" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "bnez")
	{
		res = "00101" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "bteqz")
	{
		res = "01100000" + imm(_ins[1]);
	}
	else if(instruction == "btnez")
	{
		res = "01100001" + imm(_ins[1]);
	}
	else if(instruction == "cmp")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "01010";
	}
	else if(instruction == "cmpi")
	{
		res = "01110" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "int")
	{
		res = "111110000000" + imm(_ins[1], 4);
	}
	else if(instruction == "jalr")
	{
		res = "11101" + rx(_ins[1]) + "11000000";
	}
	else if(instruction == "jr")
	{
		res = "11101" + rx(_ins[1]) + "00000000";
	}
	else if(instruction == "jrra")
	{
		res = "1110100000100000";
	}
	else if(instruction == "li")
	{
		res = "01101" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "lw")
	{
		res = "10011" + rx(_ins[1]) + rx(_ins[2]) + imm(_ins[3], 4);
	}
	else if(instruction == "lw_sp")
	{
		res = "10010" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "mfih")
	{
		res = "11110" + rx(_ins[1]) + "00000000";
	}
	else if(instruction == "mfpc")
	{
		res = "11101" + rx(_ins[1]) + "01000000";
	}
	else if(instruction == "move")
	{
		res = "01111" + rx(_ins[1]) + rx(_ins[2]) + "00000";
	}
	else if(instruction == "mtih")
	{
		res = "11110" + rx(_ins[1]) + "00000001";
	}
	else if(instruction == "mtsp")
	{
		res = "01100100" + rx(_ins[1]) + "00000";
	}
	else if(instruction == "neg")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "01011";
	}
	else if(instruction == "not")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "01111";
	}
	else if(instruction == "nop")
	{
		res = "0000100000000000";
	}
	else if(instruction == "or")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "01101";
	}
	else if(instruction == "sll")
	{
		res = "00110" + rx(_ins[1]) + rx(_ins[2]) + imm(_ins[3], 3) + "00";
	}
	else if(instruction == "sllv")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "00100";
	}
	else if(instruction == "slt")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "00010";
	}
	else if(instruction == "slti")
	{
		res = "01010" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "sltu")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "00011";
	}
	else if(instruction == "sltui")
	{
		res = "01011" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "sra")
	{
		res = "00110" + rx(_ins[1]) + rx(_ins[2]) + imm(_ins[3], 3) + "11";
	}
	else if(instruction == "srav")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "00111";
	}
	else if(instruction == "srl")
	{
		res = "00110" + rx(_ins[1]) + rx(_ins[2]) + imm(_ins[3], 3) + "10";
	}
	else if(instruction == "srlv")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "00110";
	}
	else if(instruction == "subu")
	{
		res = "11100" + rx(_ins[1]) + rx(_ins[2]) + rx(_ins[3]) + "11";
	}
	else if(instruction == "sw")
	{
		res = "11011" + rx(_ins[1]) + rx(_ins[2]) + imm(_ins[3], 5);
	}
	else if(instruction == "sw_rs")
	{
		res = "01100010" + imm(_ins[1]);
	}
	else if(instruction == "sw_sp")
	{
		res = "11010" + rx(_ins[1]) + imm(_ins[2]);
	}
	else if(instruction == "xor")
	{
		res = "11101" + rx(_ins[1]) + rx(_ins[2]) + "01110";
	}
	else
	{
		errorExit("(1001)指令错误: 未知的指令" + _ins[0]);
	}
	return res;
}

vector<string> split(const string& _res)
{
	vector<char> blank = {'\t', ' ', '\r', '\n'};
	vector<char> avoidPreffix = {'.', ';', '#'};
	vector<string> res;
	string cache = "";
	for (const auto& letter: _res)
	{
		if (find(blank.begin(), blank.end(), letter) == blank.end())
		{
			cache += letter;
		}
		else
		{
			if (!cache.empty() && (find(avoidPreffix.begin(),
				avoidPreffix.end(), cache.at(0)) == avoidPreffix.end()))
			{
				res.push_back(cache);
				cache = "";
			}
		}
	}
	if (!cache.empty())
	{
		res.push_back(cache);
	}
	return res;
}

void showVersion()
{
	cout<<"THCO-MIPS 编译器"<<endl;
	cout<<"王万霖 dgideas@outlook.com"<<endl;
	return;
}

void showUsage(const string& _appName = "")
{
	cout<<"用法: ";
	if (!_appName.empty())
	{
		cout<<_appName;
	}
	else
	{
		cout<<"msm";
	}
	cout<<" <mipsfilename> [outputfilename]"<<endl;
	cout<<endl<<"其中，可以接受汇编预编译选项 (以.起始)"<<endl;
	cout<<"\t.target: 设置汇编目标"<<endl;
	cout<<"\t.start:  起始内存地址, 用于计算B指令跳转的内存位置"<<endl;
	return;
}
/*
int main(int argc, char* argv[])
{
	bool DEBUG = false;
	if (argc < 2)
	{
		showUsage(string(argv[0]));
	}
	else
	{
		string outputFilename = "out.bin";
		if (argc == 3)
		{
			outputFilename = argv[2];
		}
		fstream mipsFile(argv[1], std::ios::in);
		fstream resFile(outputFilename, std::ios::out);
		string line;
		while (getline(mipsFile, line))
		{
			vector<string> splitedInstruction = split(strToLower(line));
			if (splitedInstruction.size())
			{
				string res = translate(splitedInstruction);
				if (DEBUG)
				{
					cout<<line<<"\t"<<res<<endl;
				}
				resFile<<toBin(res);
			}
		}
	}
	return 0;
}
*/