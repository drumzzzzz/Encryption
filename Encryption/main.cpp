#include <iostream>
#include "FileIO.h"

using namespace std;

// Strategy abstract polymorphic base
class Encryption
{
public:
	virtual vector<string> Calculate(vector<string>* text, const char* pass, bool isEncrypt) = 0;

	static void DisplayInputValues(vector<string>* values)
	{
		cout << "Input:" << endl;
		for (string s : *values)
			cout << s << endl;
	}

	static void DisplayOutputValues(vector<string>* values)
	{
		cout << "Output:" << endl;
		for (string s : *values)
			cout << s << endl;
	}
};


// Performs an ascii roll based encryption/decryption on supplied text from a given password 
class RollShift : public Encryption
{
	private:
	const int MIN = 32;
	const int MAX = 126;
	
	char AsciiRoll(int ascii_code, char c, int dir)
	{
		int value = (int)c;
		
		if (ascii_code < MIN || ascii_code > MAX)
			return c;

		for (int i=0;i<ascii_code;i++)
		{
			value+=dir;
			//if (value > MAX)
			//	value = MIN;
		}
		return (char)value;
	}
	
	public:
	vector<string> Calculate(vector<string>* text, const char* pass, bool isEncrypt)
	{
		vector<string> crypt_text;
		const unsigned int pass_length = strlen(pass) - 1;
		unsigned int text_length;

		unsigned int pass_idx = 0;
		int ascii_code;
		int idx = 0;
		char c;

		DisplayInputValues(text);

		for (string ts : *text)
		{
			text_length = ts.size();
			crypt_text.emplace_back("");
			//pass_idx = 0;
			// Iterate text characters and apply a +- rotate per password value 
			for (unsigned int text_idx = 0; text_idx < text_length; text_idx++)
			{
				ascii_code = (int)pass[pass_idx];
				c = AsciiRoll(ascii_code, ts[text_idx], (isEncrypt) ? 1 : -1);
			    crypt_text[idx].push_back(c);
				
				pass_idx++;
				if (pass_idx > pass_length)
					pass_idx = 0;
			}
			idx++;
		}

		DisplayOutputValues(&crypt_text);
		return crypt_text;
	}
};

// Strategy pattern context methods
class Context : public FileIO
{
private:
	Encryption* encryption;

public:
	Context()
	{}

	Context(Encryption* new_encryption)
	{
		encryption = new_encryption;
	}

	void changeEncryption(Encryption* new_encryption)
	{
		encryption = new_encryption;
	}

	vector<string> executeStrategy(vector<string>* values, const char* pass, bool isEncrypt)
	{
		return encryption->Calculate(values,pass, isEncrypt);
	}

	vector<string>* ReadFile(string filename)
	{
		cout << "Reading file ..." << endl;
		return fileRead(filename);
	}

	bool WriteFile(string filename, vector<string>* data)
	{
		cout << "Writing file ..." << endl;
		return fileWrite(filename, data, false);
	}
};

int main(int argc, char* argv[])
{
	const int PASS_LENGTH_MIN = 8;
	const int PASS_LENGTH_MAX = 20;
	
	if (argc < 6)
	{
		cerr << "Usage: " << endl <<
			argv[0] << "<-e/-d> <-encryption> 'text file in' 'text file out' password" << endl
			<< "Example: -e -rollshift myfile1.txt myfile2.txt p@ssword1" << endl;
		return -1;
	}
	
	if (strlen(argv[3]) < PASS_LENGTH_MIN || strlen(argv[3]) > PASS_LENGTH_MAX)
	{
		cout << "Invalid password length (" << strlen(argv[3]) << "). Must be between "
		<< PASS_LENGTH_MIN << " and " << PASS_LENGTH_MAX << " characters." << endl;
		return -1;
	}

	if (strlen(argv[3]) == 0 || strlen(argv[4]) == 0)
	{
		cout << "Invalid file names." << endl;
		return -1;
	}

	Context* context = new Context();
	RollShift rollshift;

	vector<string>* file_text_in = context->ReadFile(argv[3]);

	if (file_text_in == nullptr || file_text_in->empty())
	{
		cout << "File was empty!" << endl;
		return -1;
	}

	bool isEncrypt;

	if (strcmp(argv[1], "-e") == 0)
		isEncrypt = true;
	else if (strcmp(argv[1], "-d") == 0)
		isEncrypt = false;
	else
	{
		cout << "Missing -e -d parameter" << endl;
		return -1;
	}

	if (strcmp(argv[2],"-rollshift") == 0)
	{
		context->changeEncryption(&rollshift);
		vector<string> result = context->executeStrategy(file_text_in, argv[5], isEncrypt);
		context->WriteFile(argv[4], &result);
	}
	else
	{
		cout << "Missing encryption type parameter" << endl;
		return -1;
	}

	return 0;
}

