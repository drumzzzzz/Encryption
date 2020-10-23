/// FileIO.h: file reading and writing header
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
using namespace std;

class FileIO
{
public:
	
    // Reads a supplied filename contents and returns a string array
    static string* fileRead(string filename)
    {
        ifstream file;
        file.open(filename);

        if (!file.is_open())
        {
            cout << "Error opening " << filename << endl;
            return nullptr;
        }

        stringstream strStream;
        strStream << file.rdbuf(); //read the file

        string* file_text = new string();
        
        file_text->append(strStream.str()); //str holds the content of the file

        file.close();
    	
        return file_text;
    }
   

    // Writes a file with a supplied string value as append or overwrite
    static bool fileWrite(string filename, string* file_text, bool isAppend)
    {
        ofstream file;

        if (isAppend)
            file.open(filename, ios_base::app);
        else
            file.open(filename);

        if (!file.is_open())
        {
            cout << "Error opening " << filename << endl;
            return false;
        }

        file << file_text->data() << endl;
        file.close();
        return true;
    }

    static bool fileWrite_Unicode(string myFile, string* ws)
	{
        ofstream outFile(myFile, ios::out | ios::binary);
        outFile.write((char*)ws->c_str(), ws->length() * sizeof(wchar_t));
        outFile.close();
        return true;
    }
};


