all: galnovae xrefs galnovae.zip

galnovae: galnovae.cpp
	g++ -Wall -O3 -o galnovae galnovae.cpp

xrefs: xrefs.c
	gcc -Wall -O3 -o xrefs xrefs.c

galnovae.zip: galnovae.txt galnovae.err galnovae.tdf galnovae.idf
	zip -j galnovae.zip galnovae.txt galnovae.err galnovae.tdf galnovae.idf

clean:
	rm galnovae.zip
	rm galnovae
	rm xrefs
