#pragma once
class menu
{
public:
	menu();
	~menu();
	bool file_read_line(std::fstream & file, int tab[], int size);
	int ShowMenu();
};

