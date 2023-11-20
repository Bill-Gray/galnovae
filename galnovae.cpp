#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Code used in 2013 to merge the IAU list of galactic novae,

http://www.cbat.eps.harvard.edu/nova_list.html

(which ends in 2010) with the list I had at the time.  That caught
some errors,  added some objects,  and added exact times for some
objects.  I don't think the code has any current use.   */

#define MAX_IAU 10000

static void fix_line_using_iau_data( char *iline, const char *iau_data)
{
   memcpy( iline + 14, iau_data + 15, 17);  /* copy in YYYY MM DD.ddddd */
   if( strlen( iau_data) > 74 && iau_data[72] != ' ')
      {       /*  yes,  there's a five-character ref in bytes 72-76 */
      char ref[6];
      size_t ilen = strlen( iline);
      const size_t ref_loc = 175;

      memcpy( ref, iau_data + 72, 5);
      ref[5] = ' ';
      if( ref[0] == 'I' && ref[1] >= '0' && ref[1] <= '9')
         ref[0] = ' '; /* IAUC refs are stored in our file without the 'I' */
      if( ilen < ref_loc)         /* no ref in incoming line yet */
         {
         ilen--;        /* back over terminating LF char */
         memset( iline + ilen, ' ', ref_loc - ilen);
         memcpy( iline + ref_loc, ref, 5);
         iline[ref_loc + 5] = '\n';
         iline[ref_loc + 6] = '\0';
         }
      }
}

#define INTENTIONALLY_UNUSED_PARAMETER( param) (void)(param)

int main( const int argc, const char **argv)
{
   FILE *ifile = fopen( "iau_list.txt", "rb");
   unsigned n = 0, i;
   char **iau = (char **)calloc( MAX_IAU, sizeof( char *));
   char buff[500];
   const bool report_missed_hits = (argc > 1);

   INTENTIONALLY_UNUSED_PARAMETER( argc);
   INTENTIONALLY_UNUSED_PARAMETER( argv);
   assert( iau);
   assert( ifile);
   while( fgets( buff, sizeof( buff), ifile))
      {
      iau[n] = (char *)malloc( strlen( buff) + 1);
      strcpy( iau[n], buff);
      n++;
      }
   fclose( ifile);
   ifile = fopen( "galnovae.dat", "rb");
   while( fgets( buff, sizeof( buff), ifile) &&
               memcmp( buff, "NOVA          YEAR MM DD.ddddd", 30))
      printf( "%s", buff);
   printf( "%s", buff);
   while( fgets( buff, sizeof( buff), ifile))
      {
      char desig[80], constell[80];
      unsigned start = 31, n_hits = 0;

      if( !memcmp( buff + 31, "NSV", 3))
         start = 34;
      if( buff[start] != ' ' && sscanf( buff + start, "%s %s", desig, constell) == 2)
         {
         assert( strlen( constell) == 3);
         strcat( desig, "           ");
         strcpy( desig + 6, constell);
         for( i = 0; i < n; i++)
            if( !memcmp( desig, iau[i], 9))
               {
               fix_line_using_iau_data( buff, iau[i]);
               iau[i][0] = '\0';
               n_hits++;
               }
         }
      if( !n_hits)        /* Ten objects have different names in the     */
         {                /* two lists;  the following fixes that issue. */
         unsigned idx;
         const char *matches[21] = { "Car SDORN1843      ", "eta Car   ",
                                     "Oph      1938      ", "N Oph 1938",
                                     "Car N    1971 02 18", "N Car 1971",
                                     "Car N    1972 03 22", "N Car 1972",
                                     "Sct N?  N1981 01 18", "N Sct 1981",
                                     "Sgr      1986 10 28", "N Sgr 1986",
                                     "Lib UG   1955 04 20", "N Lib 1955",
                                     "Boo N?  N1962 12 03", "N Boo 1962",
                                     "Lac N?  N1986 11 22", "N Lac 1986",
                                     "Vel XR   1990 02 22", "N Vel 1990",
                                     NULL };

         for( i = 0; matches[i]; i += 2)
            if( !memcmp( buff + 31, matches[i + 1], 10))
               for( idx = 0; idx < n; idx++)
                  if( !memcmp( iau[idx] + 6, matches[i], 19))
                     {
                     fix_line_using_iau_data( buff, iau[idx]);
                     iau[idx][0] = '\0';
                     n_hits++;
                     }
         }

      printf( "%s", buff);
      if( report_missed_hits && n_hits != 1 && atoi( buff + 14) < 2011)
         printf( "%d hits here\n", n_hits);
      }
   for( i = 0; i < n; i++)
      if( iau[i][0])
         printf( "Leftover: %s", iau[i]);
   fclose( ifile);
   return( 0);
}
