delete from wordlist where id > 0;

\. H:\Sourcecodes\Web\GREWords\Parser\testquery.sql

drop table wordlist;

insert into wordlist
	(word, Meaning, Example, wdtype, isprinted)
	values ('Toll', '(of a bell) Sound with a slow, uniform succession of strokes, as a signal or announcement', 'Tolling like that must be for a governor at least.', 4, 0);
