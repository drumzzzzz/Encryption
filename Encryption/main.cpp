#include <iostream>
#include "FileIO.h"

using namespace std;

// Performs a XOR based encryption/decryption on supplied text from a given password 
class Algorithm
{
	private:
	
	public:
	static vector<string>* Calculate(vector<string>* text, const char* pass)
	{
		vector<string>* crypt_text = new vector<string>();
		const unsigned int pass_length = strlen(pass) + 1;
		int text_length;
		// const unsigned int text_length = strlen(text) + 1;
		unsigned int crypt;
		unsigned int pass_idx = 0;
		char c;

		for (string ts : *text)
		{
			text_length = ts.size();
			string* s = new string();
			pass_idx = 0;
			// Iterate text characters and apply a rotating XOR per password value 
			for (unsigned int text_idx = 0; text_idx < text_length; text_idx++)
			{
				crypt = (int)pass[pass_idx];
				c = (int)pass[pass_idx];
				s->push_back(ts[text_idx] ^ c);
				pass_idx++;
				if (pass_idx > pass_length)
					pass_idx = 0;
			}
			//s->erase(remove(s->begin(), s->end(), '\n'), s->end());
			crypt_text->push_back(s->data());
		}
		return crypt_text;
	}
};


int main(int argc, char* argv[])
{
	const int PASS_LENGTH_MIN = 8;
	const int PASS_LENGTH_MAX = 20;
	
	if (argc < 4)
	{
		cerr << "Usage: " << endl <<
			argv[0] << "'text file in' 'text file out' password" << endl
			<< "Example: myfile1.txt myfile2.txt p@ssword1" << endl
			<< "Example: myfile2.txt myfile3.txt p@ssword1" << endl;
		return -1;
	}
	
	if (strlen(argv[3]) < PASS_LENGTH_MIN || strlen(argv[3]) > PASS_LENGTH_MAX)
	{
		cout << "Invalid password length (" << strlen(argv[3]) << "). Must be between "
		<< PASS_LENGTH_MIN << " and " << PASS_LENGTH_MAX << " characters." << endl;
		return -1;
	}

	if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0)
	{
		cout << "Invalid file names." << endl;
		return -1;
	}

	cout << "Reading " << argv[1] << endl;

	vector<string>* file_text_in = FileIO::fileRead(argv[1]);
	
	if (file_text_in == nullptr)
	{
		cout << "File was empty!" << endl;
		return -1;
	}

	cout << "Processing Text ..." << endl;

	vector<string>* file_text_out = Algorithm::Calculate(file_text_in, argv[3]);

	cout << "Writing to file " << argv[2] << endl;
	
	if (!FileIO::fileWrite(argv[2], file_text_out, false))
	{
		cout << "Operation Failed!" << endl;
		return -1;
	}
	
	cout << "Operation Completed." << endl;

	vector<string>* test_text_out = Algorithm::Calculate(file_text_out, argv[3]);
	cout << "Input:" << endl;
	for (string ts : *file_text_in)
	{
		cout << ts << endl;
	}

	cout << "Encrypted:" << endl;
	for (string ts : *file_text_out)
	{
		cout << ts << endl;
	}

	cout << "Decrypted:" << endl;
	for (string ts : *test_text_out)
	{
		cout << ts << endl;
	}
	
	return 0;
}

