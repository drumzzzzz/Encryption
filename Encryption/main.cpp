#include <iostream>
#include "FileIO.h"

using namespace std;

// Strategy abstract polymorphic base
class Encryption
{
public:
	virtual vector<string> Calculate(vector<int>* values) = 0;
};


// Performs a XOR based encryption/decryption on supplied text from a given password 
class RollShift : Encryption
{
	private:
	
	public:
	static string Calculate(const char* text, const char* pass)
	{
		string crypt_text = "";
		const unsigned int pass_length = strlen(pass) - 1;
		const unsigned int text_length = strlen(text) - 1;

		unsigned int crypt;
		unsigned int pass_idx = 0;
		char c;
		int idx = 0;
	
		// Iterate text characters and apply a rotating XOR per password value 
		for (unsigned int text_idx = 0; text_idx < text_length; text_idx++)
		{
			crypt = (int)pass[pass_idx];
			crypt_text.push_back((char)text[text_idx] ^ crypt);
			c = (char)(text[text_idx] ^ crypt);
			cout << "(" << text[text_idx] << ":" << crypt << ", " << c << ") " << endl;
			pass_idx++;
			if (pass_idx > pass_length)
				pass_idx = 0;
		}
		//int pos;
		//if ((pos = crypt_text.find('')) != string::npos)
		//	crypt_text.erase(pos);
	
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

	string* file_text_in = FileIO::fileRead(argv[1]);
	
	if (file_text_in->empty())
	{
		cout << "File was empty!" << endl;
		return -1;
	}

	cout << "Processing Text ..." << endl;

	string encrypted = Algorithm::Calculate(file_text_in->data(), argv[3]);

	cout << "Encrypted:" << endl;
	cout << encrypted;

	cout << "Writing to file " << argv[2] << endl;
	
	if (!FileIO::fileWrite_Unicode(argv[2], &encrypted))
	{
		cout << "Operation Failed!" << endl;
		return -1;
	}
	
	cout << "Operation Completed." << endl;

	string decrypted = Algorithm::Calculate(encrypted.data(), argv[3]);
	cout << "Decrypted:" << endl;
	cout << decrypted;
	
	return 0;
}

