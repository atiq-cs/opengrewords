create database GREdict;

Data Definitions
----------------
1. Number represents primary key
2. word itself
3. word meaning: variable size
4. Example sentence
// extra fields
6. antonyms
7. parts of speech
5. wordtype
8. moderation status
9. reserved field 1, int type

wordtype
--------
1 = Master Wordlist
2 = GRE Wordlist
3 = Uncategorized Normal Wordlist, all words except type 1 & 2
4 = Movie Disposal, how to group word for movies?
5 = Magazine Disposal

create table movielist
	(id TINYINT NOT NULL AUTO_INCREMENT,
	title varchar(200) NOT NULL,
	watchdate DATE,
	PRIMARY KEY (id));

create table wordlist
	(id MEDIUMINT NOT NULL AUTO_INCREMENT,
	word varchar(50) NOT NULL,
	Meaning varchar(1000) NOT NULL,
	Example varchar(2000),
	Antonym varchar(300),
	partsOfSpeech TINYINT(1),
	wdtype TINYINT NOT NULL,
	movieid TINYINT,
	isprinted TINYINT(1),
	PRIMARY KEY (id));

create table synonyms
	(id MEDIUMINT NOT NULL,
	syn varchar(100) NOT NULL,
	isSynStar TINYINT(1),
	FOREIGN KEY (id) REFERENCES wordlist(id));

create table sentences
	(id MEDIUMINT NOT NULL,
	sent varchar(500) NOT NULL,
	isSentStar TINYINT(1),
	FOREIGN KEY (id) REFERENCES wordlist(id));

add column
ALTER TABLE wordlist ADD COLUMN isMeaningStar TINYINT(1);
	
Renamed the table
alter table wordlist change meaning varchar(300);

add not null to syn attribute
alter table synonyms change syn syn varchar(300) NOT NULL;

add column
ALTER TABLE synonyms ADD COLUMN isSynStar TINYINT(1);

add column
ALTER TABLE sentences ADD COLUMN isSentStar TINYINT(1);


insert into wordlist
	(word, Meaning, wdtype)
	values ('Abase', 'lower; degrade; humiliate; make humble; make oneself lose self-respect', 1);

insert into wordlist
	(word, Meaning, wdtype)
	values ('Aberrant', 1);

insert into synonyms
	values (1, 'reduce');
	
insert into synonyms
	values (1, 'lessen');

insert into synonyms
	values (1, 'lower');

insert into synonyms
	values (2, 'abnormal');
	
insert into synonyms
	values (2, 'anomalous');

insert into sentences
	values (1, 'Each confession brought her into an attitude of abasement', 0);

insert into wordlist
	(id, word, priority, Meaning, isMeaningStar)
	values (xx, 'Abeyance', 1, '', 0);
