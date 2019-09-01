#include <stdio.h>
#include <string.h>

/* Small bit of code to extract cross-references from 'iauc8000.txt',
putting them in the format used in 'galnovae.txt'.  */

int main( const int argc, const char **argv)
{
   char buff[300];
   FILE *ifile = fopen( "iauc8000.txt", "rb");

   while( fgets( buff, sizeof( buff), ifile))
      if( strstr( buff, argv[1]))
         {
         buff[6] = '\0';
         printf( "  %s", buff + 2);
         }
   printf( "\n");
   return( 0);
}
