/* Code to read in 'galnovae.txt' and write 'galnovae.csv' to stdout.
Compiles with

gcc -Wall -Wextra -pedantic -o cvt2csv cvt2csv.c

and can be run as,  e.g.,  './cvt2csv > galnovae.csv'.  Add a command line
argument,  and it outputs the format for galnovae.txt,  as reconstructed
from the column_size[] array and the names for the columns given in the
tags[] array.        */

#include <stdio.h>
#include <string.h>
#include <assert.h>

static const int column_sizes[] = {
        14, 5, 3, 9, 13, 15,        /* name,  yyyy, mm, dd.dd,  GCVS name, RA */
        14, 1, 5, 4,                /* dec,  mag. < or >, disc. mag,  photo band */
        5, 3, 1, 4,                 /* max mag, max mag band,  min mag < or >, min mag */
        1, 3, 2,                    /* min mag ? or uncertain,  min mag,  band */
        8, 8, 22, 41,               /* T3,  spect class, obscure xid,  disc name */
        7, 7, 7 };                  /* references 1, 2, 3... */

static int extract_field( char *obuff, const char *buff, const int field_no)
{
   int start, end;
   int column_starts[50];
   const int n_fields = (int)( sizeof( column_sizes) / sizeof( column_sizes[0]));
   const int ilen = (int)strlen( buff);

   column_starts[0] = 1;
   for( int i = 1; i < n_fields; i++)
      column_starts[i] = column_starts[i - 1] + column_sizes[i - 1];
   if( field_no < n_fields - 1)
      {
      start = column_starts[field_no];
      end = column_starts[field_no + 1];
      }
   else           /* all subsequent reference fields are seven bytes each */
      {
      start = column_starts[n_fields - 1] + 7 * (field_no + 1 - n_fields);
      end = start + 7;
      }
   *obuff = '\0';
   start--;             /* columns are numbered starting at one.  Let's */
   end--;               /* switch to zero-based addressing */
   if( start > ilen)                /* don't have this field */
      return( -1);
   else
      {
      if( end > ilen)
         end = ilen;
      while( start < end && buff[start] == ' ') /* skip leading spaces */
         start++;
      while( start < end && buff[end - 1] == ' ') /* and trailing spaces */
         end--;
      memcpy( obuff, buff + start, end - start);
      obuff[end - start] = '\0';
      }
   return( 0);
}

const char *tags[] = {  "Nova_name", "year_disc", "month_disc", "day_disc", "GCVS_ID",
         "RA", "dec", "disc_mag_indicator", "disc_mag", "disc_mag_band", "max_mag",
         "max_mag_band", "min_mag_indicator", "min_mag", "min_mag_flag",
         "min_mag_band", "T3_indicator", "T3", "GCVS_class", "obscure_xid",
         "discoverers", NULL };

#define INTENTIONALLY_UNUSED_PARAMETER( param) (void)(param)

int main( const int argc, const char **argv)
{
   char buff[1000];
   FILE *ifile = fopen( "galnovae.txt", "rb");
   int i;

   INTENTIONALLY_UNUSED_PARAMETER( argv);
   if( argc == 2)       /* just show column sizes/starts/labels */
      {
      int col = 1, ref_size;

      for( i = 0; tags[i]; col += column_sizes[i], i++)
         printf( "%4d %2d %s\n", col, column_sizes[i], tags[i]);
      printf( "%4d  7 ref1\n", col);
      ref_size = column_sizes[i];
      printf( "Subsequent refs at %d, %d, %d...\n", col + ref_size,
                  col + 2 * ref_size,  col + 3 * ref_size);
      return( 0);
      }
   assert( ifile);
   printf( "\"%s\"", tags[0]);
   for( i = 1; tags[i]; i++)
      printf( ",\"%s\"", tags[i]);
   for( i = 0; i < 90; i++)
      printf( ",\"ref_%d\"", i);
   printf( "\n");

   while( fgets( buff, sizeof( buff), ifile))
      if( memcmp( buff, "NOVA ", 5))
         {
         char field[80];

         buff[strlen( buff) - 1] = '\0';           /* remove trailing line feed */
         for( i = 0; !extract_field( field, buff, i); i++)
            printf( "%s\"%s\"", (i ? "," : ""), field);
         printf( "\n");
         }
   fclose( ifile);
   return( 0);
}
