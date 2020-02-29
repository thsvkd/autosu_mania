#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <Windows.h>	
#include <io.h>
#include <ctype.h>

using namespace std;

vector<vector<int>> make_arduino_pattern(vector<vector<int>> pattern);
vector<vector<int>> vector_sort(vector<vector<int>> long_note, int index);
int CopyTextToClipboard(const char *ap_string);
char* CopyClipboardToText();

int main()
{
	string path("C:/Users/thsxo/AppData/Local/osu!/Songs/");
	string song_folder("354157 m108 - _ Crow Solace _/");
	string file_name("m108 -  Crow Solace  (Critical_Star) [4K Extra].osu");
	string code_arduino;
	ifstream p(path + song_folder + file_name);
	string tmp;
	stringstream JUST_PASTE_IT;
	vector<string> osu_file;
	vector<vector<int>> pattern;
	int line = 0;

	if (p.is_open())
	{
		while (!p.eof())
		{
			getline(p, tmp, '\n');
			osu_file.push_back(tmp);
		}
		p.close();
	}

	p.open("arduino_code.txt");

	if (p.is_open())
	{
		while (!p.eof())
		{
			getline(p, tmp);
			code_arduino += tmp;
			code_arduino += "\n";
		}

		p.close();
	}

	int tag[3] = { 0 };

	for (int i = 0; i<osu_file.size() - 1; i++)
	{
		if (osu_file[i].compare("[TimingPoints]") == 0)
		{
			stringstream ss(osu_file[i + 1]);
			vector<string> tmp_string;
			double timing;

			while (ss.good())
			{
				getline(ss, tmp, ',');

				tmp_string.push_back(tmp);
			}
			timing = atof(tmp_string[1].c_str());
		}

		if (osu_file[i].compare("[HitObjects]") == 0)
		{
			tag[1] = 1;
			continue;
		}

		if (tag[1] == 1)
		{
			line++;
			stringstream ss(osu_file[i]);
			vector<int> tmp_int;

			while (ss.good())
			{
				getline(ss, tmp, ',');

				tmp_int.push_back(atoi(tmp.c_str()));
			}

			switch (tmp_int[0])
			{
			case 64:
				tmp_int[0] = 0;
				break;
			case 192:
				tmp_int[0] = 1;
				break;
			case 320:
				tmp_int[0] = 2;
				break;
			case 448:
				tmp_int[0] = 3;
				break;
			default:
				break;
			}

			tmp_int.erase(tmp_int.begin() + 1);
			tmp_int.erase(tmp_int.begin() + 3);

			pattern.push_back(tmp_int);
		}
	}

	pattern = make_arduino_pattern(pattern);

	JUST_PASTE_IT << "#include <avr/pgmspace.h>" << endl << "#include \"Keyboard.h\"" << endl << "#include \"Mouse.h\"" << endl << endl;
	JUST_PASTE_IT << "//current map is " << file_name << endl;
	JUST_PASTE_IT << "const PROGMEM unsigned char pattern[][4] = " << endl << "{";

	for (int i = 0; i < pattern.size(); i++)
	{
		JUST_PASTE_IT << "{"
			<< pattern[i][0] << ","
			<< pattern[i][1] << ","
			<< pattern[i][2] << ","
			<< pattern[i][3]
			<< "}";
		if (i != pattern.size() - 1)
			JUST_PASTE_IT << ",";
		JUST_PASTE_IT << "  //" << pattern[i][4] << endl;
	}
	JUST_PASTE_IT << "};" << endl << endl;
	JUST_PASTE_IT << "const PROGMEM unsigned long timing[] =" << endl << "{";

	for (int i = 0; i < pattern.size(); i++)
	{
		JUST_PASTE_IT << pattern[i][4];
		if (i != pattern.size() - 1)
			JUST_PASTE_IT << "," << endl;
	}
	JUST_PASTE_IT << "};" << endl;

	JUST_PASTE_IT << "int END = " << pattern.size() << ";" << endl;
	JUST_PASTE_IT << "//" << "file size is " << pattern.size() * 8 / 1024 << "KB" << endl;
	cout << "//" << "file size is " << pattern.size() * 8 / 1024 << "KB" << endl;
	JUST_PASTE_IT << code_arduino;

	string good = JUST_PASTE_IT.str();
	CopyTextToClipboard(good.c_str());

	return 0;
}

vector<vector<int>> vector_sort(vector<vector<int>> long_note, int index)
{
	int i, j;
	vector<int> t;
	for (i = 0; i<long_note.size() - 1; i++)
	{
		for (j = 1; j<long_note.size(); j++)
		{
			if (long_note[j - 1][index] > long_note[j][index])
			{
				t = long_note[j - 1];
				long_note[j - 1] = long_note[j];
				long_note[j] = t;
			}
		}
	}

	return long_note;
}

vector<vector<int>> make_arduino_pattern(vector<vector<int>> pattern)
{
	int timing = pattern[0][1];
	vector<vector<int>> arduino;
	vector<vector<int>> long_note;
	vector<vector<int>> tmp_long;
	vector<int> tmp(5, 0);
	if (pattern[0][2] != 128)
		tmp[pattern[0][0]] = 1;
	else
	{
		vector<int> long_tmp2(2, 0);
		tmp[pattern[0][0]] = 2;
		long_tmp2[0] = pattern[0][0];
		long_tmp2[1] = pattern[0][3];
		long_note.push_back(long_tmp2);
	}
	pattern.push_back(tmp);

	for (int i = 1; i < pattern.size(); i++)
	{
		if (timing != pattern[i][1])
		{
			tmp[4] = timing;
			arduino.push_back(tmp);
			tmp.assign(5, 0);
			timing = pattern[i][1];
		}

		if (pattern[i][2] != 128)
		{
			tmp[pattern[i][0]] = 1;
		}
		else if (pattern[i][2] == 128)
		{
			tmp[pattern[i][0]] = 2;
			vector<int> long_tmp(2, 0);
			long_tmp[0] = pattern[i][0];
			long_tmp[1] = pattern[i][3];
			long_note.push_back(long_tmp);
		}
	}

	long_note = vector_sort(long_note, 1);

	for (int i = 0; i < long_note.size(); i++)
		cout << i << " : " << long_note[i][0] << " " << long_note[i][1] << endl;

	int j = 0;

	for (int i = 0; i < arduino.size(); i++)
	{
		if (j < long_note.size())
		{
			if (long_note[j][1] == arduino[i][4])
			{
				arduino[i][long_note[j][0]] = 3;
				j++;
				i--;
			}
			else if (long_note[j][1] < arduino[i][4])
			{
				vector<int> tmp1(5, 0);
				tmp1[4] = long_note[j][1];
				tmp1[long_note[j][0]] = 3;
				arduino.insert(arduino.begin() + i, tmp1);
				j++;
				i--;
			}
		}
	}

	if (arduino[arduino.size() - 1][0] == 2 || arduino[arduino.size() - 1][1] == 2 || arduino[arduino.size() - 1][2] == 2 || arduino[arduino.size() - 1][3] == 2)
	{
		tmp.assign(5, 0);

		for (; j < long_note.size(); j++)
		{
			tmp[4] = long_note[j][1];
			tmp[long_note[j][0]] = 3;
		}
		arduino.push_back(tmp);
	}

	return arduino;
}

int CopyTextToClipboard(const char *ap_string)
{
	int string_len = strlen(ap_string) + 1;
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_len);
	char *p_data = (char*)::GlobalLock(h_data);

	if (NULL != p_data)
	{
		memcpy(p_data, ap_string, string_len);

		::GlobalUnlock(h_data);
		if (::OpenClipboard(NULL))
		{
			::EmptyClipboard();
			::SetClipboardData(CF_TEXT, h_data);
			::CloseClipboard();
		}
	}

	return 0;
}

char* CopyClipboardToText()
{
	unsigned int priority_list = CF_TEXT;
	char* p_string = NULL;

	if (::GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		if (::OpenClipboard(NULL))
		{
			HANDLE h_clipboard_data = ::GetClipboardData(CF_TEXT);
			if (h_clipboard_data != NULL)
			{
				char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
				int string_len = strlen(p_clipboard_data) + 1;
				p_string = new char[string_len];

				memcpy(p_string, p_clipboard_data, string_len);
				::GlobalUnlock(h_clipboard_data);
			}
			::CloseClipboard();
		}
	}

	return p_string;
}