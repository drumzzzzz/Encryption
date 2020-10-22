#include <iostream>
using namespace std;

class Algorithm
{
	private:
	
	public:
	static string Calculate(const char* text, const char* pass)
	{
		char ch;
		int pass_idx = 0;
		const int pass_length = strlen(pass);
		const int text_length = strlen(text);
		unsigned int crypt;

		// cout << "Text:" << text << " Length:" << text_length << endl;
		// cout << "Pass:" << pass << " Length:" << pass_length << endl;
		
		string s;
		char c;

		for (int text_idx=0; text_idx < text_length;text_idx++)
		{
			crypt = (int)pass[pass_idx];
			s.push_back(text[text_idx] ^ crypt);
			
			pass_idx++;
			if (pass_idx >= pass_length)
				pass_idx = 0;
		}

		return s.data();
	}
};


int main()
{
	const string encrypted = Algorithm::Calculate("Hello World! Yes it is working!", "password");
	cout << "Encrypted:";
	cout << string(encrypted) << endl;
	const string decrypted = Algorithm::Calculate(encrypted.c_str(), "password");
	cout << "Decrypted:";
	cout << string(decrypted) << endl;
}

