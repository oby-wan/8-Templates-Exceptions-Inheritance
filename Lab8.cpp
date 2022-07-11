#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <class T>
class PGM
{
protected:
	T** board;
	T numrows;
	T numcols;
	T max;
	string magic;
	string comment;

	void readInput(ifstream& infile) {
		cout << "Read input called" << endl;
		string line;
		char header[3];
		infile >> header;
		getline(infile, line);
		getline(infile, line);
		infile >> numcols >> numrows >> max;
		board = new T* [numcols];
		for (int i = 0; i < numcols; i++)
			board[i] = new T[numrows];
		for (int i = 0; i < numrows; i++) {
			for (int j = 0; j < numcols; j++)
				infile >> board[j][i];
		}
		if (numcols <= 0 || numrows <= 0 || max < 0 || max > 255) {
			cout << numcols << numrows << max << endl;
			throw exceptionClass("Bad values");
		}
	}
public:
	// for file exceptions
	class exceptionClass {
	private:
		const char* message;
	public:
		exceptionClass(const char* colourExcept) : message(colourExcept) {
			message = colourExcept;
		}
		string returnError() {
			return message;
		}
	};
	// default constructor
	PGM() : numrows(0), numcols(0), max(255), board(nullptr), magic(""), comment("") {
		cout << "default constructor called" << endl;
	}
	// one-arg constructor
	PGM(ifstream& infile) : numrows(0), numcols(0), max(255), board(nullptr), magic(""), comment("") {
		cout << "PGM One arg constructor called" << endl;
		readInput(infile);
	}
	// destructor
	~PGM() {
		cout << "Destroying PGM Object" << endl;
		for (int i = 0; i < numcols; i++) 
			delete[] board[i];
		delete[] board;
		board = nullptr;	
	}
	// overloaded addition operator
	PGM operator+(const PGM<T>& p) {
		for (int i = 0; i < numrows; i++) {
			for (int j = 0; j < numcols; i++)
				board[i][j] += p.board[i][j];
		}
		return *this;
	}
	// copy constructor
	PGM(const PGM<T>& p) : numrows(0), numcols(0), max(255), board(nullptr), magic(""), comment("") {
		cout << "> Copy constructor called" << endl;
		numrows = p.numrows;
		numcols = p.numcols;
		max = p.max;
		board = new T*[numrows];
		for (int i = 0; i < numrows; i++) {
			for (int j = 0; j < numcols; j++)
				board[i][j] = p.board[i][j];
		}
	}
	// move constructor
	PGM(PGM<T>&& p) : numrows(0), numcols(0), max(255), board(nullptr), magic(""), comment("") {
		cout << "move constructor called" << endl;
		for (int i = 0; i < numrows; i++) {
			for (int j = 0; j < numcols; j++) {
				board[i][j] = p.board[i][j];
				p.board[i] = nullptr;
				p.board = nullptr;
			}
		}
		numcols = p.numcols;
		p.numcols = nullptr;
		numrows = p.numrows;
		p.numrows = nullptr;
		magic = p.magic;
		p.magic = nullptr;
		comment = p.comment;
		p.comment = nullptr;
	}
	// overloaded assignment operator
	PGM<T>& operator=(const PGM<T>& p) {
		numcols = p.cols;
		numrows = p.numrows;
		board = new T*[numrows];
		if (this != &p) {
			if (magic != nullptr)
				delete[] magic;
			if (comment != nullptr)
				delete[] comment;
			
			magic = new char[strlen(p.magic) + 1];
			strcpy(magic, p.magic);
			comment = new char[strlen(p.comment) + 1];
			strcpy(comment, p.comment);
		}
		return *this;
	}
	// move assignment operator
	PGM<T>& operator=(PGM<T>&& p) {
		numcols = p.cols;
		p.numcols = nullptr;
		numrows = p.numrows;
		p.numrows = nullptr;

		board = new T**[numrows];
			if (this != &p) {
				if (magic != nullptr)
					delete[] magic;
				if (comment != nullptr)
					delete[] comment;

				magic = new char[strlen(p.magic) + 1];
				strcpy(magic, p.magic);
				comment = new char[strlen(p.comment) + 1];
				strcpy(comment, p.comment);
				p.magic = nullptr;
				p.comment = nullptr;
			}
		return *this;
	}
	// template friend function for saving PGM object
	// file
	template <class T>
	friend void saveToPGM(const PGM<T>& p , const char* c) {
		ofstream out(c);
		out << "P2\n" << "#Created by GIMP version 2.10.28 PNM plug-in\n";
		out << p.numcols << " " << p.numrows << "\n" << p.max << "\n";
		for (int i = 0; i < p.numrows; i++) {
			for (int j = 0; j < p.numcols; j++) 
				out << p.board[j][i] << " ";
			out << endl;
		}
	}
};

int main()
{
	ifstream file1("secret.pgm", ios::in);
	ifstream file2("key.pgm", ios::in);
	PGM<short>* ptr1 = nullptr;
	PGM<short>* ptr2 = nullptr;
	PGM<short>* ptr3 = nullptr;
	try
	{
		ptr1 = new PGM<short>(file1);
		ptr2 = new PGM<short>(file2);
		ptr3 = new PGM<short>(*ptr1 + *ptr2);
	}
	catch (PGM<short>::exceptionClass e)
	{
		cout << e.returnError() << endl;
		return 0;
	}
	saveToPGM(*ptr3, "result.pgm");
	delete ptr1;
	ptr1 = nullptr;
	delete ptr2;
	ptr2 = nullptr;
	delete ptr3;
	ptr3 = nullptr;
	return 0;
}