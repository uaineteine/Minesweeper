// Minesweeper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h> 
#include <iostream>
#include <iterator>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

bool inPlay = false;
int mapSize = 10;
int mineCount = 20;
vector<vector<int>> map;
vector<vector<string>> drawMap;
vector<vector<int>> markedList;
int point[2];
int AlgorthimType;
int ConsoleSize = 80; //default value
string fileToSave = "autosave.txt"; //default
//I think you can guess what this is.
string deathMap[7] = 
{
	"  `/osss+`   -s`    /o       oo:    ++    -+sss+-    `mo",
	" /Ny:``-yM+  +M.    hN       NmM+   dd  `dm/.`.+Nh`  `Ms",
	"`Nh      hM` +M:....hN       Ny:Mo  dd  yM.     /M+  `Mo",
	"-Mo      sM. +MyssssmN       Ny :Ns dd  dN      -Mo  `Mo",
	" Nd     `mm  +M.    hN       Ny  -Nydd  sM-     sM-   y:",
	" -dmo++smh.  +M.    hN       Ny   .mMd  `sNy++odm:   .do",
	"   `:::-`    `:     .-       -.    `-.     -:::.      -." 
};
string winMap[10] =
{
	"     .`        .` `:.                                            ",
	".MN.      yM+ /ms            .s/                                 ",
	" +Mh     :Mh  `:.    `://-  -oMh::.   `://:`    .:`./:`-:`    `:.",
	"  hM+   `NN.  /Ms  `yMyoohm oyMmss: .hNyooyMd.  oMdmsy:oMy    hM/",
	"  `NM.  yM/   /Ms  yM+       :My    mM-    /Md  oMy     hM/  +Ms ",
	"   :Md /My    /Ms  mM.       :My   `MM     .MN  oM+     `NN..Nm  ",
	"    sMyNm`    /Ms  sMy   `/  :Mh    hMo   `yMo  oM+      -MddN.  ",
	"    `dMN-     /Mo   odNmmms   yNmm+ `odNddmh/   oM/       +MM/   ",
	"                       `                `                 +My    ",
	"                                                         .md`    "
};
//adjaceny
string Algo1[5] =
{
	"[0] [1] [2] [2] [1]",
	"[0] [1] [*] [*] [1]",
	"[0] [1] [2] [2] [1]",
	"[1] [1] [0] [0] [0]",
	"[*] [1] [0] [0] [0]"
};
//distance
string Algo2[5] =
{
	"[2] [2] [2] [2] [2]",
	"[2] [1] [1] [1] [2]",
	"[2] [1] [*] [1] [2]",
	"[2] [1] [1] [1] [2]",
	"[2] [2] [2] [2] [2]"
};

void GenerateMap()
{
	//Generate null map and overwrite
	map.clear();
	for (int i = 0; i < mapSize; i++)
	{
		vector<int> row;
		for (int j = 0; j < mapSize; j++)
			row.push_back(0);
		map.push_back(row);
	}
	//Generate n mines
	//algo maps the mine as 1 since it only 
	//draws distance away per reveal
	//algo one is the offical game one
	for (int i = 0; i < mineCount; i++)
	{
		int x = rand() % mapSize;
		int y = rand() % mapSize;
		//Ensure it isn't already a mine
		if (map[x][y] != 1)
		{
			map[x][y] = 1;
		}
		else
		{
			//it is, repeat
			i--;
		}

	}
	//Generate Draw Map and overwrite
	drawMap.clear();
	for (int i = 0; i < mapSize; i++)
	{
		vector<string> row;
		for (int j = 0; j < mapSize; j++)
			row.push_back("?");
		drawMap.push_back(row);
	}
}
//check if x or y is within map bounds
bool intInBounds(int i)
{
	if (i > -1)
		if (i < mapSize)
			return true;
	return false;
}
//algo2 Get vector of co-ordinates of a square n distance from centre
vector<vector<int>> GetSquare(int x, int y, int distance)
{
	vector<vector<int>> output;
	vector<int> coordinates = { x - distance , y - distance };
	output.push_back(coordinates);
	for (int i = 0; i < 2 * distance; i++)
	{
		coordinates = { output[output.size() - 1][0] + 1, output[output.size() - 1][1] };
		output.push_back(coordinates);
	}
	for (int i = 0; i < 2 * distance; i++)
	{
		coordinates = { output[output.size() - 1][0], output[output.size() - 1][1] + 1 };
		output.push_back(coordinates);
	}
	for (int i = 0; i < 2 * distance; i++)
	{
		coordinates = { output[output.size() - 1][0] - 1, output[output.size() - 1][1] };
		output.push_back(coordinates);
	}
	for (int i = 0; i < (2 * distance) - 1; i++)
	{
		coordinates = { output[output.size() - 1][0] , output[output.size() - 1][1] - 1 };
		output.push_back(coordinates);
	}
	return output;
}
//Get the current adjacency count of bombs
//this is gametype/algotype 1
int GetAdjacencyValue(int x, int y)
{
	int output = 0;
	//get co-ordinates of square 1 distance away
	vector<vector<int>> currSquare = GetSquare(x, y, 1);
	//check all co-ordinates
	for (int k = 0; k < currSquare.size(); k++)
	{
		int xcheck = currSquare[k][0];
		int ycheck = currSquare[k][1];
		if (intInBounds(xcheck) == true)
			if (intInBounds(ycheck) == true)
				if (map[xcheck][ycheck] == 1)
					output += 1;
	}
	return output;
}
//Get distance by checking squares until found a bomb is found
//this is gametype/algotype 2
int GetProximityValue(int x, int y)
{
	int output;
	bool found = false;
	for (int i = 0; found == false; i++)
	{
		vector<vector<int>> currSquare = GetSquare(x, y, i);
		for (int k = 0; k < currSquare.size(); k++)
		{
			int xcheck = currSquare[k][0];
			int ycheck = currSquare[k][1];
			if (intInBounds(xcheck) == true)
				if (intInBounds(ycheck) == true)
					if (map[xcheck][ycheck] == 1)
						found = true;
		}
		output = i;
	}
	return output;
}
//returns the current attributes of window handle-used for size changing
WORD GetConsoleTextAttribute(HANDLE hCon)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hCon, &con_info);
	return con_info.wAttributes;
}
void PrintMap()
{
	string line = "__|   ";
	for (int x = 0; x < mapSize; x++)
	{
		line += std::to_string(x + 1);
		line += "  |  ";
	}
	std::cout << line << std::endl;
	for (int y = 0; y < mapSize; y++)
	{
		line = std::to_string(y + 1);
		if (y < 9)
			line += " ";
		line += "|";
		for (int x = 0; x < mapSize; x++)
		{
			if (x > 8)
				line += " ";
			line +=" [ ";
			line += drawMap[x][y];
			line += " ]";
		}
		std::cout << line << std::endl;
	}
}
void SelectMapPoint()
{
	//Select map point
	//x
	bool repeat = true;
	string xstring;
	int x;
	while (repeat == true)
	{
		std::cout << "Select x co-ord:";
		std::cin >> xstring;
		x = atoi(xstring.c_str()) - 1;
		if (x != -1)
		{
			//check within bounds
			if (intInBounds(x) == true)
			{
				repeat = false;
			}
			else
			{
				std::cout << "Ordinate: " << x << " is out of map bounds. Please try again" << std::endl;
			}
		}
		else
		{
			system("cls");
			PrintMap();
			std::cout << "Not a valid integer, please try again" << std::endl;
		}
	}
	std::cout << "Selected: " + xstring << std::endl;

	//y
	repeat = true;
	string ystring;
	int y;
	while (repeat == true)
	{
		std::cout << "Select y co-ord:";
		std::cin >> ystring;

		y = atoi(ystring.c_str()) - 1;
		if (y != -1)
		{
			//check within bounds
			if (intInBounds(y) == true)
			{
				repeat = false;
			}
			else
			{
				std::cout << "Ordinate: " << y << " is out of map bounds. Please try again" << std::endl;
			}
		}
		else
		{
			std::cout << "Not a valid integer, please try again" << std::endl;
		}
	}

	std::cout << "Selected: " + ystring << std::endl;

	//create point vector
	point[0] = x;
	point[1] = y;
}
void ChangeWindowSize(int MapSize, int lines)
{
	int columns = 80;
	//adjust coloumn size for map beyond size 10;
	if (MapSize > 10)
		columns += (MapSize - 10) * 6;

	//create mode command in cmd to change size from integers
	string col = to_string(columns);
	string lin = to_string(lines);
	string command = "mode ";
	command += col;
	command += ",";
	command += lin;
	const char * c = command.c_str();
	system(c);

	//adjust window size, while keeping it in the original postion
	HWND consoleHndl = GetConsoleWindow();
	RECT rect;
	GetWindowRect(consoleHndl, &rect);
	MoveWindow(consoleHndl, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}
void SaveGame(string name)
{
	//save file
	ofstream savefile;
	savefile.open(name);
	if (savefile.is_open())
	{
		savefile << AlgorthimType << "\n";
		savefile << mapSize << "\n";
		savefile << mineCount << "\n";
		for (int x = 0; x < mapSize; x++)
			for (int y = 0; y < mapSize; y++)
			{
				savefile << map[x][y] << "\n";
			}
		for (int x = 0; x < mapSize; x++)
			for (int y = 0; y < mapSize; y++)
			{
				savefile << drawMap[x][y] << "\n";
			}
		savefile.close();
	}
	//if an error occured
	else 
		std::cout << "Unable to save game :'(" << std::endl;
}
void LoadGame(string name)
{
	//load game
	ifstream openfile;
	openfile.open(name);
	//if can open
	if (openfile.is_open())
	{
		string line;
		getline(openfile, line);
		AlgorthimType = atoi(line.c_str());
		getline(openfile, line);
		mapSize = atoi(line.c_str());
		ChangeWindowSize(mapSize, 30);
		getline(openfile, line);
		mineCount = atoi(line.c_str());
		for (int x = 0; x < mapSize; x++)
			for (int y = 0; y < mapSize; y++)
			{
				getline(openfile, line);
				map[x][y] = atoi(line.c_str());
			}
		for (int x = 0; x < mapSize; x++)
			for (int y = 0; y < mapSize; y++)
			{
				getline(openfile, drawMap[x][y]);
			}
		openfile.close();
	}
	//the error handling is in the main void
}
void centerstring(char* input)
{
	int l = strlen(input);
	int pos = (int)((ConsoleSize - l) / 2);
	for (int i = 0; i<pos; i++)
		cout << " ";

	cout << input << endl;
}
void RevealSelected()
{
	//if it is currently 'marked'
	if (drawMap[point[0]][point[1]] == "*")
	{
		//remove point from marked list
		for (int i = 0; i < markedList.size(); i++)
			if (markedList[i][0] == point[0] & markedList[i][1] == point[1])
			{
				//remove
				markedList.erase(markedList.begin() + i);
			}
	}
	if (AlgorthimType == 1)
	{
		//get adjacency count
		drawMap[point[0]][point[1]] = std::to_string(GetAdjacencyValue(point[0], point[1]));
	}
	else
	{
		//get the proximity value/distance away
		drawMap[point[0]][point[1]] = std::to_string(GetProximityValue(point[0], point[1]));
	}

	//check for bang though
	if (map[point[0]][point[1]] == 1)
	{
		system("cls");
		//change colour-red
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		//draw death strings-centered
		for (int i = 0; i < (sizeof(deathMap) / sizeof(*deathMap)); i++)
		{
			string lineToCentre;
			lineToCentre.append(deathMap[i]);
			char *ToPrint = new char[lineToCentre.length() + 1];
			std::strcpy(ToPrint, lineToCentre.c_str());
			centerstring(ToPrint);
		}
		//return normal colour to console
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		inPlay = false;
	}
	else
	{
		//no bang-continue
		system("cls");
		SaveGame("autosave.txt");
		PrintMap();
	}
}
void MarkSelected()
{
	drawMap[point[0]][point[1]] = "*";
	vector<int> pVec(std::begin(point), std::end(point));
	markedList.push_back(pVec);
	//autosave
	SaveGame("autosave.txt");
	system("cls");
	PrintMap();
}
bool VictoryCheck()
{
	//needs to be as many as bombs
	if (markedList.size() == mineCount)
	{
		//check all marked
		bool MisMatched = false;
		for (int i = 0; i < markedList.size(); i++)
		{
			int x = markedList[i][0];
			int y = markedList[i][1];
			if (map[x][y] != 1)
				return false;
		}
		if (MisMatched == false)
		{
			//none mismatched inidcating all mines are mapped correctly
			return true;
		}
	}
	return false;
}
int _tmain(int argc, _TCHAR* argv[])
{
	std::cout<<"Hello there!" << std::endl;
	std::cout << "" << std::endl;

	//The whole game loop
	bool theAllMightyLoop = true;
	while (theAllMightyLoop == true)
	{
		bool GetPlayOrLoad = true;
		//Play Game or Load-intro
		while (GetPlayOrLoad == true)
		{
			std::cout << "Would you like to play a new game (n) or load one (l)?" << std::endl;
			string read;
			std::cin >> read;
			if (read == "l")
			{
				//GenerateTheVectors
				GenerateMap();

				//get files
				bool GoodFile = false;
				string fileName = "autosave.txt";
				while (GoodFile == false)
				{
					std::cout << "Please specifiy filename:" << std::endl;
					std::cin >> read;
					//check if not default
					//check if it exists
					ifstream f(read.c_str());
					GoodFile = f.good();
					//bad file
					if (GoodFile == false)
					{
						system("cls");
						std::cout << "File does not exist, please try again" << std::endl;
					}
				}
				///load the selected file
				fileName = read;
				LoadGame(fileName);
				fileToSave = fileName;
				//Start the Game
				system("cls");
				PrintMap();
				inPlay = true;
				GetPlayOrLoad = false;
			}
			else
			{
				//new game
				if (read == "n")
				{
					system("cls");

					//algorithm type
					bool getAlgoType = true;
					std::cout << "What game style/algorthim would you like? (1) or (2)" << std::endl;
					std::cout << std::endl;
					//Display types
					for (int i = 0; i < (sizeof(Algo1) / sizeof(*Algo1)); i++)
					{
						string lineToCentre;
						lineToCentre.append(Algo1[i]);
						lineToCentre.append("         ");
						lineToCentre.append(Algo2[i]);
						char *ToPrint = new char[lineToCentre.length() + 1];
						std::strcpy(ToPrint, lineToCentre.c_str());
						centerstring(ToPrint);
					}
					cout << endl;
					centerstring("1                           2");
					cout << endl;
					centerstring("Offical proximity game        Unoffical distance game");
					//get the gametype
					while (getAlgoType == true)
					{
						std::cin >> read;
						AlgorthimType = atoi(read.c_str());
						if (AlgorthimType != -1)
						{
							switch (AlgorthimType) {
							case 1:
								getAlgoType = false;
								break;
							case 2:
								getAlgoType = false;
								break;
							default: std::cout << "The input must be either 1 or 2" << std::endl;
							}
						}
						else
						{
							std::cout << "That is an invalid integer, please try again" << std::endl;
						}
					}

					//mapsize
					bool getMapSize = true;
					std::cout << "What map size would you like? Between 3-20?" << std::endl;
					while (getMapSize == true)
					{
						std::cin >> read;
						int size = atoi(read.c_str());
						if (size != -1)
						{
							if (size > 2)
							{
								if (size < 21)
								{
									//all is good
									mapSize = size;
									getMapSize = false;
								}
								else
								{
									std::cout << "The integer: " << size << " is too large, please try again" << std::endl;
								}
							}
							else
							{
								std::cout << "The integer: " << size << " is too small, please try again" << std::endl;
							}
						}
						else
						{
							std::cout << "That is an invalid integer, please try again" << std::endl;
						}
					}
					int cells = pow(mapSize, 2);

					//get bomb count
					bool getBombCount = true;
					std::cout << "How many mines would you like? Between 1-" << cells << "?" << std::endl;
					while (getBombCount == true)
					{
						std::cin >> read;
						int count = atoi(read.c_str());
						if (count != -1)
						{
							if (count > 0)
							{
								if (count < cells)
								{
									//all good
									mineCount = count;
									getBombCount = false;
								}
								else
								{
									std::cout << "The integer: " << count << " is too large. It must be less than the cell counts of the map: " << cells << ", please try again" << std::endl;
								}
							}
							else
							{
								std::cout << "The integer: " << count << " is must be 0 or above, please try again" << std::endl;
							}
						}
						else
						{ 
							std::cout << "The integer: " << count << " is invalid, please try again" << std::endl;
						}
					}

					//Get savefile name
					fileToSave = "autosave.txt";
					bool gettingFileName = true;
					//can remove while loop
					while (gettingFileName == true)
					{
						std::cout << "Please specifiy a filename to save to:" << std::endl;
						std::cin >> read;
						fileToSave = read;
						//can remove while loop
						gettingFileName = false;
					}

					//Generate map
					GenerateMap();

					//Print initial map and adjust window size
					system("cls");
					ChangeWindowSize(mapSize, 30);

					//Start the game
					PrintMap();
					inPlay = true;
					GetPlayOrLoad = false;
					SaveGame(fileToSave);
				}
				else
				{
					std::cout << "Sorry please try again... 'n' or 'l'?" << std::endl;
				}
			}
		}
		//in play
		while (inPlay == true)
		{
			std::cout << "" << std::endl;

			//select point or save
			bool SelectOrSave = true;
			while (SelectOrSave == true)
			{
				std::cout << "Select point (p) or save game (s)?";
				string read;
				cin >> read;
				if (read == "p")
				{
					SelectMapPoint();
					SelectOrSave = false;
				}
				else if (read == "s")
				{
					SaveGame(fileToSave);
					system("cls");
					PrintMap();
				}
				else
					cout << "Invalid input, please give either 'p' to select a point or 's' for saving a game" << endl;
			}


			bool getRevealOrMark = true;
			while (getRevealOrMark == true)
			{
				int xprint = point[0] + 1;
				int yprint = point[1] + 1;
				std::cout << "Reveal (r) (" << xprint << ", " << yprint << "), mark (m) or cancel (c)";

				string read;
				std::cin >> read;

				if (read == "r")
				{
					//reveal it
					getRevealOrMark = false;
					RevealSelected();
				}
				else if (read == "m")
				{
					//mark it
					getRevealOrMark = false;
					MarkSelected();
				}
				else if (read == "c")
				{
					//cancel, do nothing
					system("cls");
					PrintMap();
					getRevealOrMark = false;
				}
				else
				{
					std::cout << "Sorry please try again... 'n' or 'l'?" << std::endl;
				}
			}

			//check wining
			if (VictoryCheck() == true)
			{
				//yay we won
				inPlay = false;
				//change colour to red
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

				//Print out the victory
				for (int i = 0; i < (sizeof(winMap) / sizeof(*winMap)); i++)
				{
					string lineToCentre;
					lineToCentre.append(winMap[i]);
					char *ToPrint = new char[lineToCentre.length() + 1];
					std::strcpy(ToPrint, lineToCentre.c_str());
					centerstring(ToPrint);
				}

				//change colour back to normal white
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				
				system("pause");
				system("cls");
			}
		}
	}
	system("pause");
	return 0;
}