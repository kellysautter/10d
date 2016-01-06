/* qmap2def, 13.03.1997/HH
 * call: qmap2def dfbfile mapfile > deffile
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LLL 256
int strcmpx(const char *cX, const char *cY);
void out (char *cL);
int main (int argc, char* argv[])
{
   char cLine [LLL];
   FILE *fMap;
   FILE *fDfb;
    int bPrint = 0;
   unsigned int i = 0;

   if (argc <=2 )
      return( -1 )

    fDfb = fopen(argv[1],"r");
    if (fDfb == NULL) {
      printf ("%s\n", strerror(errno));
      return( -1 )
   }
    fMap = fopen(argv[2],"r");
    if (fMap == NULL) {
      printf ("%s\n", strerror(errno));
      return -2;
   }

   while (fgets(cLine,LLL,fDfb) != NULL) {
      printf("%s",cLine);
   }
   fclose(fDfb);

   while (fgets(cLine,LLL,fMap) != NULL) {

        /* Convert Tab to Blank */
      for (i=0; i < strlen(cLine); i++) {
         if (cLine[i] == '\t')
            cLine[i] == ' ';
      }
      i = strspn(cLine+0," ");   /* Blank-Elimination */

      /* Bypass Blankline */
      if (cLine[i] == '\0' || cLine[i] == '\n' )
         continue;

      if ( strcmpx(cLine+i, "Address") != 0 )
      {
         if (bPrint)
            out (cLine);
         continue;
      }

      /* "Address" found */
        i += sizeof("Address")-1;
      i += strspn(cLine+i," ");   /* Blank-Elimination */

      /* End Processing if "Address Publics" */
      if ( strcmpx( cLine+i, "Publics" ) == 0 )
         break;

      if ( strcmpx( cLine+i, "Export" ) != 0 )
         continue;

      /* "Address Export" found */
      i += sizeof( "Export" ) - 1;
      i += strspn(cLine+i," ");   /* Blank-Elimination */

      if (strcmpx (cLine+i, "Alias") == 0) {
         bPrint =1;
      }
   }
   fclose(fMap);

}
int strcmpx(const char *cX, const char *cY)
{
    for (;;) {
       if (*cY == '\0')
          return( 0 )
       if (*cX != *cY)
          break;
       cX++;
       cY++;
    }
    return( 1 )
}
void out (char *cL)
{
   char *pc, *pc1;
   char cA [LLL];

   pc = cL;
   while (*pc == ' ')
      pc++;
   pc = strchr (pc, ' ');
   if (!pc)
      return;
   while (*pc == ' ')
      pc++;
   pc1 = pc;
   pc = strchr(pc, ' ');
   if (!pc)
      return;
   *pc = '\0';

   strcpy (cA,pc1);
   for (pc=cA; *pc; pc++)
      *pc = toupper(*pc);
    printf ("%s=%s\n",cA,pc1);
}
