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
#include <math.h>

using namespace std;

typedef wstring str_t;

double gaussianRandom(void);
vector<vector<int>> make_arduino_pattern(vector<vector<int>> pattern, int mode);
vector<vector<int>> vector_sort(vector<vector<int>> long_note, int index);
int key_mode(vector<string> osu_file, int i);
int CopyTextToClipboard(const char* ap_string);
char* CopyClipboardToText();
vector<str_t> get_files_in_folder(str_t folder, str_t file_type);
int first_excute();
string bit2string(int num);
string num2bi(vector<int> pattern, int mode);

enum {
	K4_MODE = 4,
	K5_MODE = 5,
	K6_MODE = 6,
	K7_MODE = 7
};

int main()
{
	int mode = 0;
	char quit = 'y';

	string PATH;
	if (first_excute())
	{
		cout << "ó�� ���� �ϼ̱���! ���� ��θ� �Է����ּ��� : ";
		getline(cin, PATH, '\n');
		ofstream s(L"arduino_code_path.txt");
		s << PATH << "/";
		s.close();
	}

	ifstream w(L"arduino_code_path.txt");
	string tmp;
	if (w.is_open())
	{
		while (!w.eof())
		{
			getline(w, tmp, '\n');
		}
		w.close();
	}


	while (quit == 'y')
	{
		wstring path;
		path.assign(tmp.begin(), tmp.end());
		wstring song_folder;
		wstring hardness;
		wstring file_name;
		string tmp_for_path;

		cout << "�뷡������ ��� 4�����̻� �����ּ��� : ";
		//cin.ignore();
		getline(cin, tmp_for_path, '\n');
		song_folder.assign(tmp_for_path.begin(), tmp_for_path.end());
		vector<str_t> all_file = get_files_in_folder(path, L"*" + song_folder + L"*");

		if (all_file.size() == 1)
		{
			wcout << all_file[0] << endl;
			song_folder = all_file[0] + L"/";
		}

		else if (all_file.size() == 0)
		{
			cout << "������ �����ϴ�..." << endl;
			continue;
		}
		else if (all_file.size() > 1)
		{
			cout << "���������� ���� �մϴ�. ��� ���� �����Ͻðڽ��ϱ�?" << endl;
			for (int i = 0; i < all_file.size(); i++)
			{
				cout << i + 1 << " ";
				wcout << all_file[i] << endl;
			}
			int index = 0;
			cout << "���°? : ";
			cin >> index;

			if (index > all_file.size())
				index = all_file.size();
			else if (index <= 0)
				index = 1;

			wcout << all_file[index - 1] << endl;
			song_folder = all_file[index - 1] + L"/";
		}

		cout << "���̵�(���ӿ� ǥ�õǾ� �ִ� ���̵� ����) : ";
		cin.ignore();
		getline(cin, tmp_for_path, '\n');
		hardness.assign(tmp_for_path.begin(), tmp_for_path.end());
		all_file = get_files_in_folder(path + song_folder, L"*" + hardness + L"*" + L".osu");

		if (all_file.size() == 1)
		{
			wcout << all_file[0] << endl;
			file_name = all_file[0];
		}

		else if (all_file.size() == 0)
		{
			cout << "���̵������� �����ϴ�..." << endl;
			continue;
		}
		else if (all_file.size() > 1)
		{
			cout << "���������� ���� �մϴ�. ��� ���� �����Ͻðڽ��ϱ�?" << endl;
			for (int i = 0; i < all_file.size(); i++)
			{
				cout << i + 1 << " ";
				wcout << all_file[i] << endl;
			}
			int index = 0;
			cout << "���°? : ";
			cin >> index;

			if (index > all_file.size())
				index = all_file.size();
			else if (index <= 0)
				index = 1;

			wcout << all_file[index - 1] << endl;
			file_name = all_file[index - 1];
		}

		string code_arduino;
		ifstream p(path + song_folder + file_name);
		string tmp;
		stringstream JUST_PASTE_IT;
		vector<string> osu_file;
		vector<vector<int>> pattern;
		int line = 0;

		srand(GetTickCount());

		if (p.is_open())
		{
			while (!p.eof())
			{
				getline(p, tmp, '\n');
				osu_file.push_back(tmp);
			}
			p.close();
		}

		int tag[3] = { 0 };

		for (int i = 0; i < osu_file.size() - 1; i++)
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
				mode = key_mode(osu_file, i + 1);
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

				switch (mode)
				{
				case K4_MODE:
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
					break;
				case K5_MODE:
					switch (tmp_int[0])
					{
					case 51:
						tmp_int[0] = 0;
						break;
					case 153:
						tmp_int[0] = 1;
						break;
					case 256:
						tmp_int[0] = 2;
						break;
					case 358:
						tmp_int[0] = 3;
						break;
					case 460:
						tmp_int[0] = 4;
						break;
					default:
						break;
					}
					break;
				case K6_MODE:
					switch (tmp_int[0])
					{
					case 42:
						tmp_int[0] = 0;
						break;
					case 128:
						tmp_int[0] = 1;
						break;
					case 213:
						tmp_int[0] = 2;
						break;
					case 298:
						tmp_int[0] = 3;
						break;
					case 384:
						tmp_int[0] = 4;
						break;
					case 469:
						tmp_int[0] = 5;
						break;
					default:
						break;
					}
					break;
				case K7_MODE:
					switch (tmp_int[0])
					{
					case 36:
						tmp_int[0] = 0;
						break;
					case 109:
						tmp_int[0] = 1;
						break;
					case 182:
						tmp_int[0] = 2;
						break;
					case 256:
						tmp_int[0] = 3;
						break;
					case 329:
						tmp_int[0] = 4;
						break;
					case 402:
						tmp_int[0] = 5;
						break;
					case 475:
						tmp_int[0] = 6;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}

				tmp_int.erase(tmp_int.begin() + 1);
				tmp_int.erase(tmp_int.begin() + 3);

				pattern.push_back(tmp_int);
			}
		}
		//�������� pattern ���� 0:Ű ��ġ 1:���۽ð� 2:�ճ�Ʈ����? 3:���� �ճ�Ʈ��� ������ �ð�  �� ��� �ִ�.
		pattern = make_arduino_pattern(pattern, mode);


		//�Ƶ��̳� �ڵ� �����ϴ� �ڵ�
		JUST_PASTE_IT << "#include <avr/pgmspace.h>" << endl 
			<< "#include \"Keyboard.h\"" << endl 
			<< "#include \"Mouse.h\"" << endl 
			<< endl 
			<< "//current map is ";
		string file_name_a;
		file_name_a.assign(file_name.begin(), file_name.end());
		JUST_PASTE_IT << file_name_a << endl << "const PROGMEM unsigned short pattern[][1] = " << endl 
			<< "{";

		for (int i = 0; i < pattern.size(); i++)
		{
			JUST_PASTE_IT << "0x" << num2bi(pattern[i],mode);

			if (i != pattern.size() - 1)
				JUST_PASTE_IT << ",";
			JUST_PASTE_IT << "  //" << pattern[i][mode] << endl;
		}
		JUST_PASTE_IT << "};" << endl << endl << "const PROGMEM unsigned long timing[] =" << endl 
			<< "{";

		for (int i = 0; i < pattern.size(); i++)
		{
			JUST_PASTE_IT << pattern[i][mode];
			if (i != pattern.size() - 1)
				JUST_PASTE_IT << "," << endl;
		}
		JUST_PASTE_IT << "};" << endl;

		JUST_PASTE_IT << "int END = " << pattern.size() << ";" << endl 
			<< "//" << "file size is " << pattern.size() * (4 + mode) / 1024 << "KB" << endl;
		std::cout << "file size is " << pattern.size() * (4 + mode) / 1024 << "KB" << endl;

		switch (mode)
		{
		case K4_MODE:
			p.open("arduino_osu_code/arduino_code_4K_ver2.0.cpp");
			break;
		case K5_MODE:
			p.open("arduino_osu_code/arduino_code_5K_ver2.0.cpp");
			break;
		case K6_MODE:
			p.open("arduino_osu_code/arduino_code_6K_ver2.0.cpp");
			break;
		case K7_MODE:
			p.open("arduino_osu_code/arduino_code_7K_ver2.0.cpp");
			break;
		default:
			break;
		}

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

		JUST_PASTE_IT << code_arduino;

		string good = JUST_PASTE_IT.str();
		CopyTextToClipboard(good.c_str());

		cout << "�ٸ� �ʵ� ������(y/n) : ";
		cin >> quit;
		cin.ignore();
	}

	return 0;
}

vector<vector<int>> vector_sort(vector<vector<int>> long_note, int index)
{
	int i, j;
	vector<int> t;
	for (i = 0; i < long_note.size() - 1; i++)
	{
		for (j = 1; j < long_note.size(); j++)
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

vector<vector<int>> make_arduino_pattern(vector<vector<int>> pattern, int mode)
{
	int timing = pattern[0][1];
	vector<vector<int>> arduino;
	vector<vector<int>> long_note;
	vector<vector<int>> tmp_long;
	vector<int> tmp(mode + 1, 0);
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
			tmp[mode] = timing;
			arduino.push_back(tmp);
			tmp.assign(mode + 1, 0);
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

	/*for (int i = 0; i < long_note.size(); i++)
		cout << i << " : " << long_note[i][0] << " " << long_note[i][1] << endl;*/

	int j = 0;

	for (int i = 0; i < arduino.size(); i++)
	{
		if (j < long_note.size())
		{
			if (long_note[j][1] == arduino[i][mode])
			{
				arduino[i][long_note[j][0]] = 3;
				j++;
				i--;
			}
			else if (long_note[j][1] < arduino[i][mode])
			{
				vector<int> tmp1(mode + 1, 0);
				tmp1[mode] = long_note[j][1];
				tmp1[long_note[j][0]] = 3;
				arduino.insert(arduino.begin() + i, tmp1);
				j++;
				i--;
			}
		}
	}

	bool last = arduino[arduino.size() - 1][0] == 2 || arduino[arduino.size() - 1][1] == 2 || arduino[arduino.size() - 1][2] == 2;
	if (mode >= 4)
		last = last || arduino[arduino.size() - 1][3] == 2;
	if (mode >= 5)
		last = last || arduino[arduino.size() - 1][4] == 2;
	if (mode >= 6)
		last = last || arduino[arduino.size() - 1][5] == 2;
	if (mode >= 7)
		last = last || arduino[arduino.size() - 1][6] == 2;

	if (last)
	{
		tmp.assign(mode + 1, 0);

		for (; j < long_note.size(); j++)
		{
			tmp[mode] = long_note[j][1];
			tmp[long_note[j][0]] = 3;
		}
		arduino.push_back(tmp);
	}

	return arduino;
}

int key_mode(vector<string> osu_file, int c)
{
	string tmp;

	for (int i = c; i < osu_file.size(); i++)
	{
		stringstream ss(osu_file[i]);
		vector<int> tmp_int;
		while (ss.good())
		{
			getline(ss, tmp, ',');

			tmp_int.push_back(atoi(tmp.c_str()));
		}

		if (tmp_int[0] == 64)
		{
			return K4_MODE;
		}
		if (tmp_int[0] == 51)
		{
			return K5_MODE;
		}
		if (tmp_int[0] == 42)
		{
			return K6_MODE;
		}
		if (tmp_int[0] == 36)
		{
			return K7_MODE;
		}
	}
}

int CopyTextToClipboard(const char* ap_string)
{
	int string_len = strlen(ap_string) + 1;
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_len);
	char* p_data = (char*)::GlobalLock(h_data);

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

double gaussianRandom(void)
{
	double v1, v2, s;

	do {
		v1 = 2 * ((double)rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 ������ ��
		v2 = 2 * ((double)rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 ������ ��
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);

	s = sqrt((-2 * log(s)) / s);

	return v1 * s * 50;
}

vector<str_t> get_files_in_folder(str_t folder, str_t file_type)
{
	vector<str_t> names;
	wchar_t search_path[200];
	wsprintf(search_path, L"%s%s", folder.c_str(), file_type.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  //if ������ 1�̸� �����̰� 0�̸� ���丮
			names.push_back(fd.cFileName);
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

int first_excute()
{
	ifstream p(L"arduino_code_path.txt");
	string tmp;
	if (p.is_open())
	{
		while (!p.eof())
		{
			getline(p, tmp, '\n');
		}
		p.close();
	}

	if (tmp.size() == 0)
		return 1;
	else
		return 0;
}

string bit2string(int num)
{
	if (num == 0)
		return string("0");
	if (num == 1)
		return string("1");
	if (num == 2)
		return string("2");
	if (num == 3)
		return string("3");
	if (num == 4)
		return string("4");
	if (num == 5)
		return string("5");
	if (num == 6)
		return string("6");
	if (num == 7)
		return string("7");
	if (num == 8)
		return string("8");
	if (num == 9)
		return string("9");
	if (num == 10)
		return string("a");
	if (num == 11)
		return string("b");
	if (num == 12)
		return string("c");
	if (num == 13)
		return string("d");
	if (num == 14)
		return string("e");
	if (num == 15)
		return string("f");
}

string num2bi(vector<int> pattern, int mode)
{
	string tmp[4];
	if (mode == 4)
	{
		tmp[0] = bit2string((pattern[1] << 2) + pattern[0]);
		tmp[1] = bit2string((pattern[3] << 2) + pattern[2]);
		return (string("00") + tmp[1] + tmp[0]);
	}
	else if (mode == 5)
	{
		tmp[0] = bit2string((pattern[1] << 2) + pattern[0]);
		tmp[1] = bit2string((pattern[3] << 2) + pattern[2]);
		tmp[2] = bit2string(pattern[4]);
		return string("0") + (tmp[2] + tmp[1] + tmp[0]);
	}
	else if (mode == 6)
	{
		tmp[0] = bit2string((pattern[1] << 2) + pattern[0]);
		tmp[1] = bit2string((pattern[3] << 2) + pattern[2]);
		tmp[2] = bit2string((pattern[5] << 2) + pattern[4]);
		return string("0") + (tmp[2] + tmp[1] + tmp[0]);
	}
	else if (mode == 7)
	{
		tmp[0] = bit2string((pattern[1] << 2) + pattern[0]);
		tmp[1] = bit2string((pattern[3] << 2) + pattern[2]);
		tmp[2] = bit2string((pattern[5] << 2) + pattern[4]);
		tmp[3] = bit2string(pattern[6]);
		return (tmp[3] + tmp[2] + tmp[1] + tmp[0]);
	}
}