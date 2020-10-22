#include <iostream>
#include "FileIO.h"

using namespace std;

// Performs a XOR based encryption/decryption on supplied text from a given password 
class Algorithm
{
	private:
	
	public:
	static string* Calculate(const char* text, const char* pass)
	{
		const int pass_length = strlen(pass);
		const int text_length = strlen(text);
		int crypt;
		int pass_idx = 0;
		
		string* s = new string();
		char c;

		// Iterate text characters and apply a rotating XOR per password value 
		for (int text_idx=0; text_idx < text_length - 1;text_idx++)
		{
			crypt = (int)pass[pass_idx];
			
			s->push_back(text[text_idx] ^ crypt);
			
			pass_idx++;
			if (pass_idx >= pass_length - 1)
				pass_idx = 0;
		}

		return s;
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

	const string file_text_in = FileIO::fileRead(argv[1]);
	
	if (file_text_in.empty())
	{
		cout << "File was empty!" << endl;
		return -1;
	}

	cout << "Processing Text ..." << endl;

	string* file_text_out = Algorithm::Calculate(file_text_in.data(), "p1ssword2");

	cout << "Writing to file " << argv[2] << endl;
	
	if (!FileIO::fileWrite(argv[2], file_text_out, false))
	{
		cout << "Operation Failed!" << endl;
		return -1;
	}
	
	cout << "Operation Completed." << endl;
	return 0;
}

