#include <iostream>
#include <string.h>
#include "FileIO.h"

using namespace std;

// Enryption strategy abstract polymorphic base
class Encryption
{
public:
	virtual ~Encryption() = default;
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


// Performs an ascii roll based encryption/decryption on supplied text from a given password ascii char
class RollShift : public Encryption
{
	private:
	const float MIN = 1;
	const float MAX = 40;

	// Convert given ascii code into a nRange(min->max) and add or subtract from char
	char AsciiRoll(int ascii_code, char c, int dir)
	{
		int value = (int)c;
		const int range = static_cast<int>((MAX - MIN) * ((float)ascii_code / 255 ));

		for (int i=0;i<range;i++)
		{
			value += dir;
		}
		return (char)value;
	}
	
	public:
	// Encrypts or decrypts supplied text array values via a character increment/decrement per rotating password value 
	vector<string> Calculate(vector<string>* text, const char* pass, bool isEncrypt) override
	{
		vector<string> crypt_text;
		const unsigned int pass_length = strlen(pass) - 1;
		unsigned int text_length;

		unsigned int pass_idx = 0;
		int ascii_code;
		int idx = 0;
		char c;

		DisplayInputValues(text);

		// Iterate array strings
		for (string ts : *text)
		{
			text_length = ts.size();
			crypt_text.emplace_back("");
			
			// Iterate the text characters and apply a +- rotate per password value 
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
	Encryption* encryption{};

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

	static vector<string>* ReadFile(string filename)
	{
		cout << "Reading file ..." << endl;
		return fileRead(filename);
	}

	static bool WriteFile(string filename, vector<string>* data)
	{
		cout << "Writing file ..." << endl;
		return fileWrite(filename, data, false);
	}
};

const int PASS_LENGTH_MIN = 8;
const int PASS_LENGTH_MAX = 20;
bool isEncrypt;

// Validate the passed arguments
bool CheckArguments(int argc, char* argv[])
{
	if (argc < 6)
	{
		cerr << "Usage: " << endl <<
			argv[0] << "<-e/-d> <-encryption> 'text file in' 'text file out' password" << endl
			<< "Example: -e -rollshift myfile1.txt myfile2.txt p@ssword1" << endl;
		return false;
	}

	if (strlen(argv[3]) < PASS_LENGTH_MIN || strlen(argv[3]) > PASS_LENGTH_MAX)
	{
		cerr << "Invalid alphanumeric password length (" << strlen(argv[3]) << "). Must be between "
			<< PASS_LENGTH_MIN << " and " << PASS_LENGTH_MAX << " characters." << endl;
		return false;
	}

	if (strlen(argv[3]) == 0 || strlen(argv[4]) == 0)
	{
		cerr << "Invalid file names." << endl;
		return false;
	}

	if (strcmp(argv[1], "-e") == 0)
	{
		isEncrypt = true;
		return true;
	}
	if (strcmp(argv[1], "-d") == 0)
	{
		isEncrypt = false;
		return true;
	}

	cerr << "Missing -e -d parameter" << endl;
	return false;
}

// Applies a selected encryption strategy
int main(int argc, char* argv[])
{
	if (!CheckArguments(argc, argv))
		return -1;
	
	Context* context = new Context();
	RollShift rollshift;

	vector<string>* file_text_in = Context::ReadFile(argv[3]);

	if (file_text_in == nullptr || file_text_in->empty())
	{
		cerr << "File was empty!" << endl;
		return -1;
	}

	// Roll shift encrypt/decrypt
	if (strcmp(argv[2],"-rollshift") == 0)
	{
		context->changeEncryption(&rollshift);
		vector<string> result = context->executeStrategy(file_text_in, argv[5], isEncrypt);
		Context::WriteFile(argv[4], &result);
	}
	else
	{
		cerr << "Missing encryption type parameter" << endl;
		return -1;
	}

	delete(context);
	return 0;
}

