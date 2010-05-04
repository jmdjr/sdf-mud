/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#include "mud.h"

/* Reads the actual time file from disk - Samson 1-21-99 */
void fread_timedata(FILE *fp)
{
  char *word = NULL;
  bool fMatch = FALSE;

  for( ; ; )
  {
    word = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;

   switch(UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
      break;
      case 'E':
        if(!str_cmp(word, "End"))
          return;
      break;
      case 'M':
        KEY("Mhour", time_info.hour, fread_number(fp));
        KEY("Mday", time_info.day, fread_number(fp));
        KEY("Mmonth", time_info.month, fread_number(fp));
        KEY("Myear", time_info.year, fread_number(fp));
      break;
    }

    if(!fMatch)
    {
      bug("Fread_timedata: no match: %s", word);
      fread_to_eol(fp);
    }
  }
return;
}

/* Load time information from saved file - Samson 1-21-99 */
bool load_timedata(void)
{
  char filename[MAX_INPUT_LENGTH];
  FILE *fp;
  bool found;

  found = FALSE;
  sprintf(filename, "%sbday.dat", SYSTEM_DIR);
  if((fp = fopen(filename, "r")) != NULL)
  {
    found = TRUE;
    for( ; ; )
    {
      char letter = '\0';
      char *word = NULL;

      letter = fread_letter(fp);
      if(letter == '*')
      {
        fread_to_eol(fp);
        continue;
      }

      if(letter != '#')
      {
        bug("%s", "Load_timedata: # not found.");
        break;
      }

      word = fread_word(fp);
      if(!str_cmp(word, "TIME"))
      {
        fread_timedata(fp);
        break;
      }
      else if(!str_cmp(word, "END"))
        break;
      else
      {
        bug("Load_timedata: bad section - %s.", word);
        break;
      }
    }
    fclose(fp);
  }
  return found;
}

/* Saves the current game world time to disk - Samson 1-21-99 */
void save_timedata(void)
{
  FILE *fp;
  char filename[MAX_INPUT_LENGTH];

  sprintf(filename, "%sbday.dat", SYSTEM_DIR);

  if((fp = fopen(filename, "w")) == NULL)
  {
    bug("%s", "save_timedata: fopen");
    perror(filename);
  }
  else
  {
    fprintf(fp, "%s", "#TIME\n");
    fprintf(fp, "Mhour	%d\n", time_info.hour);
    fprintf(fp, "Mday	%d\n", time_info.day);
    fprintf(fp, "Mmonth	%d\n", time_info.month);
    fprintf(fp, "Myear	%d\n", time_info.year);
    fprintf(fp, "%s", "End\n\n");
    fprintf(fp, "%s", "#END\n");
  }
  fclose(fp);
return;
}
