#ifndef FILEIOUTIL_HPP_
#define FILEIOUTIL_HPP_

#include <string>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <dirent.h>
#include <list>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

template<typename T>
class FileIOUtil
{
private:
	FileIOUtil();
public:
	static string& getInputFolderPath();
	static string& getOutputFolderPath();

	static void readData(string, string, T*&, unsigned long&);

	static void ensureDirectory(string);

	static void writeData(string, string, T*, unsigned long);

	static list<string> listFiles(string, string);
};

template<typename T>
void FileIOUtil<T>::readData(string filePath, string fileName, T*& data, unsigned long& total)
{
	FILE* input = fopen(((filePath + fileName).c_str()), "rb");

	if (input)
	{
		total = 0;

		char* buffer = nullptr;
		long fileLength = 0;
		fseek(input, 0, SEEK_END);
		fileLength = ftell(input);

		fseek(input, 0, SEEK_SET);
		buffer = (char*) malloc(fileLength * sizeof(char));
		fread(buffer, sizeof(char), fileLength, input);
		fclose(input);

		for (unsigned long i = 0; i <= fileLength * sizeof(char); i++)
		{
			if (buffer[i] == '\n' || buffer[i] == '\0' || i == (fileLength * sizeof(char)))
			{
				total++;
			}
		}

		istringstream processor(buffer);
		free(buffer);

		data = new T[total];

		for (unsigned long i = 0; i < total; i++)
		{
			T d;
			processor >> d;
			data[i] = d;
		}
	}
}

template<typename T>
void FileIOUtil<T>::ensureDirectory(string path)
{
	mkdir(path.c_str(), 0777);
}

template<typename T>
void FileIOUtil<T>::writeData(string filePath, string fileName, T* data, unsigned long total)
{
	if (data == nullptr)
	{
		return;
	}

	ostringstream buffer;

	for (unsigned long i = 0; i < total; i++)
	{
		buffer << to_string(data[i]);

		if (i + 1 < total)
		{
			buffer << '\n';
		}
		else
		{
			buffer << '\0';
		}
	}

	mkdir(filePath.c_str(), 0777);
	FILE* output = fopen((filePath + fileName).c_str(), "wb");
	fwrite(buffer.str().c_str(), 1, ((long)buffer.tellp()) - 1, output);
	fclose(output);
}

template<typename T>
list<string> FileIOUtil<T>::listFiles(string directoryPath, string extension)
{
	list<string> expanded;

	if (extension.size() < 2)
	{
		extension = ".txt";
	}

	if (extension.at(0) != '.')
	{
		extension = '.' + extension;
	}

	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(directoryPath.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			unsigned long len = strlen(ent->d_name);

			if (ent->d_type == DT_REG && len > extension.length() && strcmp(ent->d_name + len - extension.length(), extension.c_str()) == 0)
			{
				expanded.emplace_back(ent->d_name);
			}
		}

		closedir(dir);
	}

	return expanded;
}

#endif /* FILEIOUTIL_HPP_ */
