#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>			// for getline
using namespace std;

int main () {
	ifstream rtffile;
	ofstream sqlfile;

	string line;

	rtffile.open ("Master Wordlist.rtf", ios::in);

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

	string wordbegin("{\\rtlch\\fcs1 \\af31507 \\ltrch\\fcs0 \\b\\f40\\fs18\\insrsid12785112\\charrsid7550108 ");
	string MeaningBegin("{\\rtlch\\fcs1 \\af31507 \\ltrch\\fcs0 \\f40\\fs18\\insrsid12785112\\charrsid7550108 ");
	string word;
	string sql_query;

	size_t found;
	size_t offset_wordbegin;
	size_t offset_MeaningBegin;
	
	int idvalue = 3;
	char buff[50];
	int writeStatus = 0;

	offset_wordbegin = wordbegin.length();
	offset_MeaningBegin = MeaningBegin.length();
	

	while (rtffile.good()) {
		getline(rtffile, line);
		
		// Get the word and get into query
		if (writeStatus == 0) {
			// find whether line has pattern of word starting tags
			//sqlfile<<"checking for word"<<endl;
			found=line.find(wordbegin);
			if (found!=string::npos) {
				//sqlfile << "occurrence found at: " << int(found) << endl;
				// discard initial tags
				word = line.substr(found+offset_wordbegin);
				//sqlfile << "word is: " << word << endl;
				// see if the word is not null
				if (word.length() > 0) {
					// discard second brace
					found=word.find('}');
					if (found!=string::npos)
						word.resize(found);

					// discard space
					found=word.find(' ');
					if (found!=string::npos)
						word.resize(found);

					// see if word remains after removing brace and space
					if (word.length() > 0) {
						// Add id
						_itoa_s(idvalue, buff, 50, 10);		//convert id		// ms secured function
						sql_query = "insert into wordlist\n\t(id, word, priority, Meaning, isMeaningStar)\n\tvalues (";
						sql_query += buff;				// append
						sql_query += ", \'";

						// Add word
						sql_query += word;

						// add priority
						sql_query += "\', 1, \'";
						idvalue++;

						writeStatus = 1;
					}
				}
			}
		}
		// Get the meanings, synonyms and add into query
		else if (writeStatus == 1) {
			//sqlfile<<"checking for meaning"<<endl;
			// find whether line has pattern of meaning starting tags
			found=line.find(MeaningBegin);
			//sqlfile<<"line "<<line<<endl;
			//sqlfile<<"search pattern "<<MeaningBegin<<endl;
			if (found!=string::npos) {
				//sqlfile << "occurrence found at: " << int(found) << endl;
				// discard initial tags
				//word = line.substr(found+offset_MeaningBegin);

				// Take the next line as word
				getline(rtffile, word);
				//sqlfile<<"meaning "<<word<<endl;

				// see if the word is not null
				if (word.length() > 0) {
					/*// discard second brace
					found=word.find('}');
					if (found!=string::npos)
						word.resize(found);

					// discard space
					found=word.find(' ');
					if (found!=string::npos)
						word.resize(found);*/

					// discard second brace
					found=word.find('\n');
					if (found!=string::npos)
						word.resize(found);

					// see if word remains after removing brace and space
					if (word.length() > 0) {
						// Add Meaning
						sql_query += word;

						// add whether meaning has star
						sql_query += "\', 0);";
						//cout<<"got query: "<<sql_query<<endl;
						sqlfile<<sql_query<<endl;

						writeStatus = 2;
					}
				}
			}
		}
		// Get the sentences and add into query
		else if (writeStatus == 2) {
			//sqlfile<<"checking for sentence"<<endl;
			// find whether line has pattern of meaning starting tags
			found=line.find(MeaningBegin);
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
					/*// discard second brace
					found=word.find('}');
					if (found!=string::npos)
						word.resize(found);

					// discard space
					found=word.find(' ');
					if (found!=string::npos)
						word.resize(found);*/

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
		}
	}
	cout<<"list processed"<<endl;

	sqlfile.close();
	rtffile.close();

	return 0;
}
