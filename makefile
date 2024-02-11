all: galnovae xrefs galnovae.zip galnovae.csv

galnovae.csv: galnovae.txt cvt2csv
	./cvt2csv >  galnovae.csv

cvt2csv: cvt2csv.c
	gcc -Wall -Wextra -pedantic -O3 -o cvt2csv cvt2csv.c

galnovae: galnovae.cpp
	g++ -Wall -Wextra -pedantic -O3 -o galnovae galnovae.cpp

xrefs: xrefs.c
	gcc -Wall -Wextra -pedantic -O3 -o xrefs xrefs.c

galnovae.zip: galnovae.txt galnovae.err galnovae.tdf galnovae.idf
	zip -j galnovae.zip galnovae.txt galnovae.err galnovae.tdf galnovae.idf

clean:
	-rm galnovae.zip
	-rm galnovae
	-rm xrefs
	-rm cvt2csv
	-rm galnovae.csv
