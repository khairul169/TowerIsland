#include "main.h"

char* Utils::getFileString(char *fname)
{
	char *string = NULL;
	FILE *f = fopen(fname, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		string = (char*)malloc(fsize + 1);
		if (string)
			fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = 0;
	}

	return string;
}