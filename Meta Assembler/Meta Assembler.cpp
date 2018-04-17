// Meta Assembler.cpp : Defines the entry point for the console application.
//Meta assembler that detects DC and DS. Performs EQU statements. Performs ORG. Makes all trnslations. Reads from definition and instruction file.
//Creates symbol table at the end.

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

string InstructionChoice(string Instruction);
string ProduceOutput(string Instruction, string Data);
void Formatting(string TotalLines, string code);
void WriteEQU(string EQUName, string EQUNumber);
int FindLinesInFile();
string EQUReplace(string DataToReplace);




int main()
{
	string Inst, Data, InstCode, Output;
	string Textline, PreviousData;
	int TotalLines, LineCounter = 0;
	TotalLines = FindLinesInFile();
	int CurrentLineNumber = 0;


	ifstream Instructions("C:\\Users\\samal\\Documents\\Visual Studio 2015\\Projects\\Meta Assembler\\Meta Assembler\\Instructions.txt");
	if (!Instructions) cout << "Error could not open file";
	for (int i = 0; i < TotalLines; i++)
	{
		string EQUVariableName, EQUNumber;
		string DName, Dtype, DNumber;
		stringstream Convert;
		Convert << hex << CurrentLineNumber;
		string Show = Convert.str();
		//cout << Show << endl;

		getline(Instructions, Textline);

		//cout << Textline << endl;
		istringstream Split(Textline);

		if (Textline.find("EQU") != string::npos)
		{
			cout << "Found EQU" << endl;
			Split >> EQUVariableName;
			Split >> EQUNumber;
			Split >> EQUNumber;

			//Call function to write EQU to txt file
			WriteEQU(EQUVariableName, EQUNumber);


		}

		else if (Textline.find("DS.") != string::npos && Textline.find(":") != string::npos)
		{
			cout << "Found DS" << endl;
			Split >> DName;
			Split >> Dtype;
			Split >> DNumber;
			//cout << DName << " " << DNumber << " " << Dtype << endl;
			DName = DName.substr(0, DName.length() - 1);
			//cout << DName << endl;
			Dtype = Dtype.substr(Dtype.length() - 1, 1);
			//cout << Dtype << endl;

			if (Dtype == "b" || Dtype == "B")
			{
				cout << "Byte type" << endl;
			}
			else if (Dtype == "w" || Dtype == "W")
			{
				cout << "Word Type" << endl;
			}
			else if (Dtype == "l" || Dtype == "L")
			{
				cout << "Long type" << endl;
			}



		}

		else if (Textline.find("DC.") != string::npos && Textline.find(":") != string::npos)
		{
			cout << "Found DC" << endl;
			Split >> DName;
			Split >> Dtype;
			Split >> DNumber;
			//cout << DName << " " << DNumber << " " << Dtype << endl;
			DName = DName.substr(0, DName.length()-1);
			//cout << DName << endl;
			Dtype = Dtype.substr(Dtype.length() - 1, 1);
			//cout << Dtype << endl;

			if (Dtype == "b" || Dtype == "B")
			{
				cout << "Byte type" << endl;
			}
			else if (Dtype == "w" || Dtype == "W")
			{
				cout << "Word Type" << endl;
			}
			else if (Dtype == "l" || Dtype == "L")
			{
				cout << "Long type" << endl;
			}

		}
		
		else
		{
			Split >> Inst;
			Split >> Data;

			
			//cout << Inst << "*" << endl;
			//cout << Data << "!" << endl;

			if (Inst == "ORG")
			{
				cout << "Found ORG" << endl;
				CurrentLineNumber = atoi(Data.c_str());
				//stringstream Convertion;
				//Convertion << hex << CurrentLineNumber;
				//string Show = Convertion.str();
				//cout << Show << "!" << endl;
			}

			//if data = "EQU Label" then replace data with EQU LABel number, and then call the output functions
			ifstream EQULabels("EQUNames.txt");
			string EQUTextLine, EQUname, EQUnumber;

			while (getline(EQULabels, EQUTextLine))
			{
				istringstream Split(EQUTextLine);
				Split >> EQUname;
				Split >> EQUnumber;

				if (Data == EQUname)
				{
					Data = EQUnumber;
				}
			}
			


			if (Inst != "ORG")
			{

				InstCode = InstructionChoice(Inst);
				if (InstCode.length() != 4)
				{
					Output = ProduceOutput(InstCode, Data);
					Formatting(Show, Output);

				}
				else if (InstCode.length() == 4)
				{

					Data = "";
					Output = ProduceOutput(InstCode, Data);
					Formatting(Show, Output);
				}
				CurrentLineNumber++;
			}
		}
		
	}
	//MAKE ERROR CATCHER SO HEX CODE OUTPUT IS NEVER BIGGER THAN 4


	ifstream EQUFILE("EQUNames.txt");
	cout << "\nSymbol Table Information" << endl;
	cout << "------------------------" << endl;
	while (getline(EQUFILE, Textline))
	{
		istringstream Split(Textline);
		string EQUName, EQUNumber;
		Split >> EQUName;
		Split >> EQUNumber;

		cout << EQUName << "\t\t" << EQUNumber << endl;
	}
	EQUFILE.close();
	remove("EQUNames.txt");

	string Wait;
	cin >> Wait;

	return 0;
}


string InstructionChoice(string Instuction)
{
	string InstInFile, HexCodeInFile, InstLetter, Textline;
	bool InstCodeFound = false;
	ifstream Definitions("C:\\Users\\samal\\Documents\\Visual Studio 2015\\Projects\\Meta Assembler\\Meta Assembler\\Definitions.txt");
	if (!Definitions) cout << "Failed to open definitions file";
	while (InstCodeFound == false)
	{
		

		getline(Definitions, Textline);
		istringstream Split(Textline);
		Split >> InstInFile;
		Split >> HexCodeInFile;


		if (Instuction == InstInFile)
		{
			InstLetter = HexCodeInFile;
			InstCodeFound = true;
		}	
	}
	Definitions.close();
	return InstLetter;
}

string ProduceOutput(string Instruction, string Data)
{
	string Code = Instruction + Data;
	return Code;
}

void Formatting(string LineCount, string code)
{
	cout << LineCount << ": " << code <<endl; 
	ofstream toCdm("output.cdm", ios::app);
	toCdm << LineCount << ": " << code << "\n";
}

int FindLinesInFile()
{
	int Amount = 0;
	ifstream Lines("C:\\Users\\samal\\Documents\\Visual Studio 2015\\Projects\\Meta Assembler\\Meta Assembler\\Instructions.txt");
	if (!Lines) cout << "Error could not open file";
	string Textline;

	//get number of lines
	while (!Lines.eof())
	{
		getline(Lines, Textline);
		//cout << Textline << endl;
		Amount++;
		//cout << TotalLines;
	}
	Lines.close();
	return Amount;
}

string SplittingLine(string Textline)
{
	string Inst, Data;
	istringstream Split(Textline);

	Split >> Inst;
	Split >> Data;
	return Inst, Data;
}


void WriteEQU(string EQUName, string EQUNumber)
{
	ofstream EQUFILE("EQUNames.txt", ios::app);
	EQUFILE << EQUName << " " << EQUNumber << "\n";
	EQUFILE.close();
}

string EQUReplace(string DataToReplace)
{
	string VariableName, VariableContents, Textline;
	ifstream EQUFILE("EQUNames.txt");
	while (getline(EQUFILE, Textline))
	{
		istringstream Split(Textline);
		Split >> VariableName;
		

		if (VariableName == DataToReplace)
		{
			Split >> VariableContents;
		}
	}
	EQUFILE.close();

	return VariableContents;
}
//Notes
////////////////////////////////////////////////////////////////////
//(Optional Label): ds.(somelength) space to be allocated

//if textline contains ":" then different execution

//otherwise continue as normal


//lea load effective address
//load address register with adress of variable
//lea	textD2, a1			Load a1 with the address of textD2
//textD2 is used for para for subroutine (trap).
//trap, i will call a subroutine via interupt mechanism, because it puts me in supervisor mode
//which gives the subroutine access to all resources.



//number:number
//address:data


//label
//look at tutorial 1
//symbol table is translation table
//like the definition file
//symbol name					Value
//these relate only to this program

//so newline 1074, so when called expect 1074.


//label	number 
//every time you see the label, you replace it with a number //this is what EQU DOES

//bsr is relative branch

//absoloute is the actuall address
//relative is add the amount to the current line

//ds and dc, 
//ds leave space and remeber where it was
//equ Identifier with a number.


//label equ number
//simply not create anything in memory
//put entry in sybmbol table
//what ever number is, when the label is called


//dc reserves memory for a label


//label
//"test" is the label
//dc and ds create the label with a memory location
//equ assosiates it with a value



