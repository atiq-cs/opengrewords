#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>			// for getline
using namespace std;

char buff[50]="";

int getLineType(string& line) {
	static int numval = 1;
	size_t found;
	string wordbegin;

	_itoa_s(numval, buff, 50, 10);		//convert id, base = 10		// ms secured function
	wordbegin = buff;
	wordbegin += ". ";
	found=line.find(wordbegin);
	// not returned null
	if (found!=string::npos) {
		line = line.substr(found+wordbegin.length());
		numval++;
		return 1;
	}
		
	wordbegin = "* ";
	found=line.find(wordbegin);
	if (found!=string::npos) {
		line = line.substr(found+wordbegin.length());
		return 3;
	}
	if (line.empty())
		return 0;

	return 2;
}

void formatWordForQuery(string& str) {
	size_t found = 0;
	while (true) {
		// replace all ' with \'
		found = str.find('\'', found);
		if (found != string::npos) {
			//cout<<"found in position "<<found<<" where line "<<str<<endl;
			str.replace(found, 1, "\\\'");
			found+=2;
		}
		else
			break;
	}
	found = 0;
	while (true) {
		// replace all ' with \'
		found = str.find('’', found);
		if (found != string::npos) {
			//cout<<"found in position "<<found<<" where line "<<str<<endl;
			str.replace(found, 1, "\\\'");
			found+=2;
		}
		else
			break;
	}
	found = 0;
	while (true) {
		// replace all ' with \'
		found = str.find('"', found);
		if (found != string::npos) {
			//cout<<"found in position "<<found<<" where line "<<str<<endl;
			str.replace(found, 1, "\\\"");
			found+=2;
		}
		else
			break;
	}
}

int main () {
	ifstream rtffile;
	ofstream sqlfile;

	string line;

	rtffile.open ("Master Wordlist.txt", ios::in);

	if (!rtffile.is_open()) {
		cout<<"Cannot open rtf file!"<<endl;
		exit(EXIT_FAILURE);
	}

	sqlfile.open ("testquery.sql", ios::out);
	if (!sqlfile.is_open()) {
		cout<<"Cannot open sql file!"<<endl;
		exit(EXIT_FAILURE);
	}

	//sqlfile << "Writing this to the file.\n";

	string sql_query;

	//size_t found;

	//char buff[50];
	int writeStatus = 0;
	int lineType, lineNumber = 0;
	bool isFirstMeaning = true;
	bool isFirstSentence = true;

	while (rtffile.good()) {
		getline(rtffile, line);
		lineNumber++;
		lineType = getLineType(line);
		//sqlfile<<"Type = "<<lineType<<" for "<<line<<endl;

		switch(lineType) {
		case 0:
			// write query
			if (sql_query.empty()) {
				cout<<"wrong sequence. shouldn't write"<<endl;
			}
			else {
				isFirstMeaning = true;
				if (isFirstSentence)
					sql_query += ", NULL";
				else
					isFirstSentence = true;
				sql_query += ", 1, 1);";
				/*insert into wordlist
					(word, Meaning, wdtype)
					values ('Abase', 'lower; degrade; humiliate; make humble; make oneself lose self-respect', 1);*/

				sqlfile<<sql_query<<endl;
				sql_query = "";
			}
			break;
		case 1:
			// get word
			formatWordForQuery(line);
			sql_query = "insert into wordlist\n\t(id, word, Meaning, Example, wdtype, isprinted)\n\tvalues ("+string(buff)+ ", \'" + line + "\'";
			break;
		case 2:
			// get meaning
			//cin.get();
			if (sql_query.empty()) {
				cout<<"wrong sequence before meaning in line "<<lineNumber<<endl;
				exit(EXIT_FAILURE);
			}
			formatWordForQuery(line);
			// handle multiple meaning
			if (isFirstMeaning) {
				sql_query += ", \'" + line + "\'";
				isFirstMeaning = false;
			}
			else
				sql_query += " \'<br>" + line + "\'";
			break;
		case 3:		// get sentence
			if (sql_query.empty()) {
				cout<<"wrong sequence before sentence"<<endl;
				exit(EXIT_FAILURE);
			}
			formatWordForQuery(line);
			// get sentences
			// handle multiple sentences
			if (isFirstSentence) {
				sql_query += ", \'" + line + "\'";
				isFirstSentence = false;
			}
			else
				sql_query += " \'<br>" + line + "\'";
			//sql_query += ", \'" + line + "\'";
			break;

		default:
			cout<<"Tentalizing conundrum!!"<<endl;
			cout<<"got strange line type "<<lineType<<endl;
			exit(EXIT_FAILURE);
			break;
		}
		/*
		// Get the word and get into query
		if (writeStatus == 0) {
			// find whether line has pattern of word starting tags
			//sqlfile<<"checking for word"<<endl;
			numval++;
			_itoa_s(numval, buff, 50, 10);		//convert id, base = 10		// ms secured function
			wordbegin = buff;
			wordbegin += ". ";
			found=line.find(wordbegin);
			// not returned null
			if (found!=string::npos) {
				//sqlfile << "occurrence found at: " << int(found) << endl;
				// discard initial tags
				word = line.substr(found);
				//sqlfile << "word is: " << word << endl;
				// see if the word is not null
				sql_query = "insert into wordlist\n\t(word, Meaning, wdtype)\n\tvalues (";
				// Add word
				sql_query += word;

				// add priority
				sql_query += "\', 1, \'";

				writeStatus = 1;
			}
		}
		// Get the meanings
		else if (writeStatus == 1) {
			sql_query += line;
			writeStatus = 2;
		}
		// Get the sentences
		else if (writeStatus == 2) {
			//sqlfile<<"checking for sentence"<<endl;
			// find whether line has pattern of meaning starting tags
			found=line.find("* ");
			//sqlfile<<"line "<<line<<endl;
			//sqlfile<<"search pattern "<<MeaningBegin<<endl;
			if (found!=string::npos) {
				//sqlfile << "occurrence found at: " << int(found) << endl;
				// discard initial tags
				word = line.substr(found+offset_MeaningBegin);
				sqlfile<<"sentence length "<<word.length()<<endl;

				if (word.length()==0)		// Take the next line as word
					getline(rtffile, word);

				//sqlfile<<"meaning "<<word<<endl;

				// see if the word is not null
				if (word.length() > 0) {
					/* discard second brace
					found=word.find('}');
					if (found!=string::npos)
						word.resize(found);

					// discard space
					found=word.find(' ');
					if (found!=string::npos)
						word.resize(found);

					// discard second brace

					found=word.find('.');
					if (found!=string::npos)
						word.resize(found-1);

					while(found==string::npos) {
						getline(rtffile, line);
						word += line;
						found += line.length();
						found=line.find('.');
					}


					// see if word remains after removing brace and space
					if (word.length() > 0) {
						// Add id
						sql_query = "insert into sentences\n\tvalues (";
						sql_query += buff;				// append
						sql_query += ", \'";

						// Add word
						sql_query += word;

						// add whether sentence has star
						sql_query += "\', 0);";
						//cout<<"got query: "<<sql_query<<endl;
						sqlfile<<sql_query<<endl;

						writeStatus = 0;
					}
				}
			}
		}
		else {
			cout<<"Tentalizing conundrum!!"<<endl;
			break;
		}*/
	}
	cout<<"list processed"<<endl;

	sqlfile.close();
	rtffile.close();

	return 0;
}
