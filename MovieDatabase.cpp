#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Movie
{
	string title, director, relDate;
	double grossTotal = 0.0;
	int runTime = 0;
};

int numberOfLines(ifstream&);
void populateMovieFromFile(ifstream&, Movie&);
Movie* createDatabase(int&);

void findMovie(Movie*, int);
bool caseInsensitiveCmp(string, string);
void displayMovie(const Movie&);
void saveToFile(const Movie&);
bool promptToContinue();
void displayFavorites();



Movie* createDatabase(int& linesNum)
{
	Movie mov;
	string fileName;
	ifstream inFile;
	int i;

	cout << "Please enter the name of the file: ";
	cin >> fileName;

	inFile.open(fileName);

	if (inFile.fail())
	{
		while (inFile.fail())
		{
			cout << "File cannot be opened." << endl;
			cout << "\nPlease enter the name of the file: ";
			cin >> fileName;

			if (inFile.is_open())
				break;
		}
	}

	linesNum = numberOfLines(inFile);

	Movie* ptrMovies = new Movie[linesNum];


	inFile.clear();
	inFile.seekg(0, ios::beg);


	for (i = 0; i < linesNum; i++)
	{
		populateMovieFromFile(inFile, mov);

		ptrMovies[i].title = mov.title; 
		ptrMovies[i].grossTotal = mov.grossTotal; 
		ptrMovies[i].director = mov.director;
		ptrMovies[i].relDate = mov.relDate; 
		ptrMovies[i].runTime = mov.runTime; 
	}

	inFile.close();

	return ptrMovies;
}



int main()
{
	Movie* moviesPtr = NULL;
	bool toContinue = true;
	int numLines;
	ifstream inFile;
	numLines = 0;

	moviesPtr = createDatabase(numLines);

	while (toContinue)
	{
		findMovie(moviesPtr, numLines);
		toContinue = promptToContinue();

		if (toContinue == false)
			break;
	}

	displayFavorites();

	delete[] moviesPtr;
	return 0;
}



int numberOfLines(ifstream& inFile)
{
	string temp;
	int size = 0; 


	while (getline(inFile, temp))
	{
		size++;
	}

	return size;
}





void populateMovieFromFile(ifstream& inFile, Movie& movieI)
{
	getline(inFile, movieI.title, ',');

	inFile.clear();
	inFile.ignore();
	inFile >> movieI.grossTotal;

	inFile.clear();
	inFile.ignore();
	getline(inFile, movieI.director, ',');

	inFile.clear();
	inFile.ignore();
	getline(inFile, movieI.relDate, ',');

	inFile.clear();
	inFile >> movieI.runTime;

	inFile.ignore(1000, '\n');

}

void findMovie(Movie* ptrMovies, int numOfLines)
{
	string userMovie, listMovie;
	int numLoops = 0;
	bool movieFound = false;
	char yN;

	cout << "Enter a movie title: ";

	cin.clear();
	cin.ignore();
	getline(cin, userMovie);

	for (int i = 0; i < numOfLines; i++)
	{
		
		movieFound = caseInsensitiveCmp(userMovie, ptrMovies[i].title);

		if (movieFound)
		{
			displayMovie(ptrMovies[i]);
			cout << "\nWould you like to save the above movie? (Y/N): ";
			cin >> yN;

			yN = toupper(yN);

			while (yN != 'Y' && yN != 'N')
			{
				cout << "Please enter Y or N: ";
				cin >> yN;

				if (yN == 'Y' || yN == 'N')
					break;
			}

			if (yN == 'Y') {
				saveToFile(ptrMovies[i]);
				cout << "Successfully saved to favorites.txt!" << endl;
			}

		}

		else
		{
			numLoops++;
		}
		
	}

	if (numLoops == 25)
	{
		cout << "Error: no such movie in database";
	}
}

bool caseInsensitiveCmp(string userInp, string listMov)
{
	int i;

	for (i = 0; i < userInp.size(); i++)
	{
		userInp[i] = tolower(userInp[i]);
	}

	for (i = 0; i < listMov.size(); i++)
	{
		listMov[i] = tolower(listMov[i]);
	}


	if (userInp == listMov)
	{
		return true;
	}

	else
		return false;
}

void displayMovie(const Movie& savedMovie)
{
	cout << "\n\tTitle: " << savedMovie.title << endl;
	cout << "  Gross Total: " << savedMovie.grossTotal << " billion" << endl;
	cout << "     Director:" << savedMovie.director << endl;
	cout << " Release Date: " << savedMovie.relDate << endl;
	cout << "     Run Time: " << savedMovie.runTime << " minutes" << endl;
}


void saveToFile(const Movie& favMovie)
{
	static int numTimes = 0;

	if (numTimes == 0)
	{
		ofstream outFile("favorites.txt");
		outFile << "Title: " << favMovie.title << endl;
		outFile << "Gross Total: " << favMovie.grossTotal << endl;
		outFile << "Director: " << favMovie.director << endl;
		outFile << "Release Date: " << favMovie.relDate << endl;
		outFile << "Run Time: " << favMovie.runTime << " minutes" << endl;
		outFile.close();
	}
	else
	{
		ofstream outFile;
		outFile.open("favorites.txt", ios::app);
		outFile << endl;
		outFile << "Title: " << favMovie.title << endl;
		outFile << "Gross Total: " << favMovie.grossTotal << endl;
		outFile << "Director: " << favMovie.director << endl;
		outFile << "Release Date: " << favMovie.relDate << endl;
		outFile << "Run Time: " << favMovie.runTime << " minutes" << endl;
		outFile.close();
	}

	numTimes++;

}

bool promptToContinue()
{
	char yesNo;
	cout << "Would you like to exit? (Y/N): ";
	cin >> yesNo;

	yesNo = toupper(yesNo);

	while ((yesNo != 'Y') && (yesNo != 'N'))
	{
		cout << "Please enter Y or N: ";
		cin >> yesNo;

		if (yesNo == 'Y' || yesNo == 'N')
			break;
	}

	if (yesNo == 'N') {
		return true;
	}
	else
		return false;
}


void displayFavorites()
{
	ifstream readFile;
	string info;
	readFile.open("favorites.txt");
	

	if (getline(readFile, info))
	{
		cout << '\n' << info << endl;

		while (getline(readFile, info, '\n'))
		{
			cout << info << endl;
		}

	}
	
	else
	{
		cout << "Error: You have not saved any movies. " << endl;
	}
	readFile.close();
}