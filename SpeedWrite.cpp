/**************************************
 * 
 * Typeing Simulator!
 * 
 * Made by John314 on June 18 2021
 * 
 * For the sole purpose of: Yes
 * 
 * Compile with : g++ SpeedWrite.cpp -lcaca -o SpeedWrite
 * 
****************************************/

//#define DEBUG_COUT 1

// Include Some Headers because we need them!
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <caca_conio.h>

// C++ syntax highlights
struct cpp_highlight {
	std::string compstr;
	uint8_t color;
};

std::vector<cpp_highlight> CPP_HighLights{
	{"#include", CACA_LIGHTMAGENTA},
	{"#define", CACA_LIGHTMAGENTA},
	{"#ifdef", CACA_LIGHTMAGENTA},
	{"#ifndef", CACA_LIGHTMAGENTA},
	{"#endif", CACA_LIGHTMAGENTA},
	{"#pragma", CACA_LIGHTMAGENTA},
	{"#undef", CACA_LIGHTMAGENTA},
	{"<",        CACA_LIGHTRED},
	{">",        CACA_LIGHTRED},
	{"!",        CACA_LIGHTRED},
	{"%",        CACA_LIGHTRED},
	{"^",        CACA_LIGHTRED},
	{"~",        CACA_LIGHTRED},
	{"&",        CACA_LIGHTRED},
	{"*",        CACA_LIGHTRED},
	{"+",        CACA_LIGHTRED},
	{"-",        CACA_LIGHTRED},
	{"->",       CACA_LIGHTRED},
	{".",        CACA_LIGHTRED},
	{"?",        CACA_LIGHTRED},

	{"++",        CACA_LIGHTRED},
	{"--",        CACA_LIGHTRED},

	{"=",        CACA_LIGHTRED},
	{"!=",       CACA_LIGHTRED},
	{"==",       CACA_LIGHTRED},
	{"/=",       CACA_LIGHTRED},
	{"*=",       CACA_LIGHTRED},
	{"+=",       CACA_LIGHTRED},
	{"-=",       CACA_LIGHTRED},
	{"&=",       CACA_LIGHTRED},
	{"|=",       CACA_LIGHTRED},
	{"^=",       CACA_LIGHTRED},
	{"%=",       CACA_LIGHTRED},
	{">=",       CACA_LIGHTRED},
	{"<=",       CACA_LIGHTRED},

	{":",        CACA_LIGHTRED},
	{"::",       CACA_GREEN},
	{"<<",       CACA_GREEN},
	{">>",       CACA_GREEN},
	{"(",        CACA_CYAN},
	{")",        CACA_CYAN},
	{"[",        CACA_CYAN},
	{"]",        CACA_CYAN},
	{"{",        CACA_CYAN},
	{"}",        CACA_CYAN},
	{"\'",       CACA_LIGHTBLUE},
	{"\"",       CACA_LIGHTBLUE},
	{"char",     CACA_LIGHTBLUE},
	{"uint8_t",  CACA_LIGHTBLUE},
	{"int8_t",   CACA_LIGHTBLUE},
	{"int",      CACA_LIGHTBLUE},
	{"uint16_t",  CACA_LIGHTBLUE},
	{"int16_t",   CACA_LIGHTBLUE},
	{"uint32_t",  CACA_LIGHTBLUE},
	{"int32_t",   CACA_LIGHTBLUE},
	{"float",     CACA_LIGHTBLUE},
	{"double",    CACA_LIGHTBLUE},
	{"bool",      CACA_LIGHTBLUE},
	{"case",      CACA_LIGHTBLUE},
	{"break",     CACA_LIGHTRED},
	{"continue",  CACA_LIGHTBLUE},
	{"struct",    CACA_LIGHTMAGENTA},
	{"class",     CACA_LIGHTMAGENTA},
	{"void",      CACA_LIGHTMAGENTA},
	{"return",    CACA_LIGHTMAGENTA},
	{"const",     CACA_LIGHTRED},
	{"for",      CACA_LIGHTMAGENTA},
	{"if",       CACA_LIGHTMAGENTA},
	{"while",    CACA_LIGHTMAGENTA},
	{"else",     CACA_LIGHTMAGENTA},
	{"goto",     CACA_LIGHTMAGENTA},

	// C++ Standerd Things
	{"std",      CACA_LIGHTRED},
	{"vector",   CACA_LIGHTMAGENTA},
	{"string",   CACA_LIGHTMAGENTA},
	{"cout",     CACA_LIGHTMAGENTA},
	{"cin",      CACA_LIGHTMAGENTA},
	{"endl",     CACA_LIGHTMAGENTA},
	{"flush",    CACA_LIGHTMAGENTA},

};

uint8_t GetParseColor(std::string test){
	for(cpp_highlight h : CPP_HighLights){
		if(h.compstr.compare(test)==0){
			return h.color;
		}
	}
	return -1;
};

std::vector<char> BreakCharsList {
	'(',
	')',
	'<',
	'>',
	' ',
	',',
	'\"',
	'\'',
	'[',
	']',
	'{',
	'}',
	';',
};


bool BreakChar(char ch){
	for(char cmp : BreakCharsList)
		if(ch == cmp)
			return true;
	return false;
};

std::vector<char> DoubleCharsList {
	':',
	'+',
	'-',
	'=',
	'|',
	'&',
};
bool DoubleTest(char ch){
	for(char cmp : DoubleCharsList)
		if(ch == cmp)
			return true;
	return false;
};

// Changes a string of numbers into an int
int str2int(std::string d){
	int v = 0;
	for(char ch : d){
		if(ch >= '0' && ch <= '9'){
			v *= 10;
			v += ch - '0';
		}
	}
	return v;
};

// List of switches
const int NumSwitchTypes = 4;

char switchTypes[NumSwitchTypes] = {
	'c',
	't',
	'w',
	'h',
};
const int st_codeFile = 0;
const int st_timeDelay = 1;
const int st_canvasWidth = 2;
const int st_canvasHeight = 3;

struct screen_line {
	std::vector<char> line;
	std::vector<uint32_t> attr;
};

// Main function that runs
int main(int argc,char *argv[]){
	// Return code for main()
	int errorCode = 0;

	// Time delay per typed char in milliseconds
	int mainTimeD = 20; // 20 ms

	// Caca Canvas Size
	int caca_CanvasWidth = 80;
	int caca_CanvasHeight = 24;

	// Push the arguments into a c++ vector
	std::vector<std::string> mainArgs;
	for(int i = 0; i < argc; i++)
		mainArgs.push_back(argv[i]);

	// Seperate vector of file names
	std::vector<std::string> fileNames;

	// Make sure there is a file to write out!
	if(mainArgs.size() < 2){
		std::cout << "Usage: SpeedWrite -c code_name.cpp [-t 20 -w 80 -h 40]" << std::endl;
		std::cout << "\t\t-" << static_cast<char>(st_codeFile) << " --> List of code files to write out." << std::endl;
		std::cout << "\t\t-" << static_cast<char>(st_timeDelay) << " --> Time between char output in milliseconds." << std::endl;
		std::cout << "\t\t-" << static_cast<char>(st_canvasWidth) << " --> Canvas width." << std::endl;
		std::cout << "\t\t-" << static_cast<char>(st_canvasHeight) << " --> Canvas height." << std::endl;

		return errorCode;
	}else{

		// Current switch
		int switchT = -1;
		// Test the switches
		for(std::string arg : mainArgs){
			bool pass = false;
			// Code file switch
			for(int t = 0; t < NumSwitchTypes; t++){
				if(arg.compare(std::string("-")+switchTypes[t])==0){
					switchT = t;
					pass = true;
					break;
				}
			}
			if(!pass){
				// Switch the switch
				switch(switchT){
					case st_codeFile:
						fileNames.push_back(arg);
					break;
					case st_timeDelay:
						// Set the time delay
						mainTimeD = str2int(arg);
					break;
					case st_canvasWidth:
						// Set the time delay
						caca_CanvasWidth = str2int(arg);
					break;
					case st_canvasHeight:
						// Set the time delay
						caca_CanvasHeight = str2int(arg);
					break;
				}
			}
		}
	}

	std::cout << "Opening ";
	for(std::string n : fileNames)
		std::cout << n << ", ";
	std::cout << std::endl;

	// Setup the caca screen
	caca_canvas_t *caca_Canvas; 
	caca_display_t * caca_Display;

	caca_Canvas = caca_create_canvas(caca_CanvasWidth, caca_CanvasHeight);
	caca_Display = caca_create_display(caca_Canvas);
	caca_set_display_title(caca_Display,"Speed Write v.1.0");
	caca_refresh_display(caca_Display);

	std::cout << "Created Caca window" << std::endl;

	// For every file
	for(std::string fname : fileNames){

		// Clear the screen
		caca_clear_canvas(caca_Canvas);

		// Refresh
		caca_refresh_display(caca_Display);

		// Open the file
		std::ifstream CodeFile(fname,std::ios::binary);

		// Make sure it opened
		if(!CodeFile.is_open()){
			std::cout << "Failed to open " << fname << std::endl;
			continue;
		}else{
			std::cout << "Opened " << fname << std::endl;
			// String that stores the code
			std::vector<char> mainCode;

			// Clear any data???
			mainCode.clear();

			// Read the data into the string
			bool code_inComment = false;
			while(!CodeFile.eof()){
#ifdef DEBUG_COUT
					std::cout << "Begin" << std::endl;
#endif
				std::string codeLine = "";
				while(!CodeFile.eof()){
					char byte;
					CodeFile.read(reinterpret_cast<char *>(&byte),sizeof(char));
					// Read the next byte if return char
					if(byte=='\r')
						CodeFile.read(reinterpret_cast<char *>(&byte),sizeof(char));
					if(byte=='\n')
						break;
					codeLine += byte;
					if(byte=='\n')
						break;
				}
				
				// Parse the line
				std::vector<std::string> codeParse;
				codeParse.push_back(""); // Push new string
				if(codeLine.length()>1){
					char lastCh = 0;
#ifdef DEBUG_COUT
					std::cout << "Parse " << codeLine << std::endl;
#endif
					for(char ch : codeLine){
						if(lastCh == '/'&&ch=='*'){
							code_inComment = true;
						}
						if(lastCh == '*'&&ch=='/'){
							code_inComment = false;
						}
						
						if(code_inComment){
							codeParse.back().push_back(ch);
							lastCh = ch;
							continue;
						}
						if(ch=='\n'||ch==0||ch == '\t'||ch=='\v'||ch==9){
							codeParse.push_back(""); // Push new string
							codeParse.back().push_back(ch);
							codeParse.push_back(""); // Push new string
							lastCh = ch;
							continue;
						}
						if(BreakChar(ch)){
							codeParse.push_back(""); // Push new string
							codeParse.back().push_back(ch);
							codeParse.push_back(""); // Push new string
							lastCh = ch;
							continue;
						}
						if(ch==lastCh&&DoubleTest(ch)){
							// Remove the last char
							codeParse.back().back() = 0;

							// Fix the string
							std::string tmpStr = codeParse.back();
							codeParse.back() = "";
							for(char c : tmpStr)
								if(c) codeParse.back() += c;

							// Push new string
							codeParse.push_back(""); 
							codeParse.back().push_back(lastCh);
							codeParse.back().push_back(ch);
							codeParse.push_back(""); // Push new string
							lastCh = ch;
							continue;
						}
						codeParse.back().push_back(ch);
						lastCh = ch;
					}
					std::string codeOutStr;
#ifdef DEBUG_COUT
					std::cout << "S:";
#endif
					for(std::string s : codeParse){
						uint8_t txt_col = GetParseColor(s);
#ifdef DEBUG_COUT
						std::cout << s << "|";
#endif
						codeOutStr += static_cast<char>(0x04);
						codeOutStr += static_cast<unsigned char>(txt_col);
						codeOutStr += s;
					}
#ifdef DEBUG_COUT
					std::cout << std::endl;
#endif
					for(char byte : codeOutStr)
						mainCode.push_back(byte);
					mainCode.push_back('\n');
#ifdef DEBUG_COUT
					std::cout << "Done" << std::endl;
#endif
				}else{
					// Add a new line
					mainCode.push_back('\n');
				}
			}
			// Position the char
			caca_gotoxy(caca_Canvas,0,0);

			int chx = 0, chy = 0;

			uint8_t CurCharCol = CACA_LIGHTGREEN;
			bool SetCol = false;
			int InComment = 0;
			char lastChar = 0;
			int codeOffset = 0;

			// Write out the code
			for(char str_ch : mainCode){
				codeOffset += 1;
				if(mainCode[codeOffset]=='/'){
					if(str_ch=='/'){
						InComment = 1;
					}
				}
				if(mainCode[codeOffset]=='*'){
					if(str_ch=='/'){
						InComment = 2;
					}
				}
				if(lastChar=='*'){
					if(str_ch=='/'){
						InComment = 0;
					}
				}
				lastChar = str_ch;

				if(SetCol){
					if(static_cast<unsigned char>(str_ch)==0xFF)
						CurCharCol = CACA_LIGHTGREEN;
					else
						CurCharCol = str_ch;
					SetCol = false;
					continue;
				}
				if(str_ch==0x04){
					// Set the color
					SetCol = true;
					continue;
				}
				if(InComment == 1){
					CurCharCol = CACA_YELLOW;
				}
				if(InComment == 2){
					CurCharCol = CACA_LIGHTGRAY;
				}

				// Set the char colour
				caca_set_color_ansi(caca_Canvas,CurCharCol,CACA_BLACK);

				// Fix the position
				if(str_ch=='\t'){
					chx += 3; // 3 not 4 because it increments later
				}

				if(chx++ >= caca_CanvasWidth || str_ch=='\n'){
					if(str_ch=='\n')
						if(InComment==1)
							InComment = 0;
					chy += 1;
					chx = 0;
				}
				if(chy >= caca_CanvasHeight){
					// Move entire screen up! :O
					std::vector<screen_line> scr_lines;
					scr_lines.resize(caca_CanvasHeight-1);
					for(int i = 0; i < caca_CanvasHeight-1; i ++){
						// Get the line
						for(int cx = 0; cx < caca_CanvasWidth; cx ++){							
							scr_lines[i].line.push_back( caca_get_char(caca_Canvas,cx,i+1));
							scr_lines[i].attr.push_back( caca_get_attr(caca_Canvas,cx,i+1));
						}
					}
					caca_clear_canvas(caca_Canvas);
					for(int i = 0; i < caca_CanvasHeight-1; i ++){
						// Print the line
						for(int cx = 0; cx < caca_CanvasWidth; cx ++){
							caca_put_char(caca_Canvas,cx,i,scr_lines[i].line[cx]);
							caca_put_attr(caca_Canvas,cx,i,scr_lines[i].attr[cx]);
						}
					}

					chy -= 1;
				}

				// Write the char
				caca_put_char(caca_Canvas,chx,chy,str_ch);
				caca_set_canvas_handle(caca_Canvas,0,chy);

				// Draw the new buffer
				caca_refresh_display(caca_Display);

				// Add some simulated delay
				usleep(mainTimeD*1000);
			}
		}
	}
	std::string cinv;
	std::cout << "\nPress Enter." << std::endl;
	std::cin >> cinv;

	// Remove the caca window
	caca_free_canvas(caca_Canvas);
	caca_free_display(caca_Display);

	return errorCode;
};
