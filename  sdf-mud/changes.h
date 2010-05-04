/*************************************************************************** *  Shadow Wars: the Revenge of Kalos                                      * --------------------------------------------------------------------------- *  You are allowd to change and modify this code, you may NOT take out    * *  the helpfile 'CREATORS', and you must leave in the Greeting,           * *  Shadow Wars: the Revenge of Kalos created by Xkilla and Sillviant      * *  (c)2001 Xkilla 'Ryan Waliany' (ryan@scv.net)                           * *  Sillviant 'Matt Burnett' (sillviant@charter.net)                       * --------------------------------------------------------------------------- *  Changes snippet by: Xkilla. This is a snippet, you may modify it, but  * *  you must leave in Xkilla's credit.                                     * ***************************************************************************/
     typedef struct changes_data CHANGE_DATA;
void load_changes( void );
void save_changes( void );
void delete_change( int num );
struct changes_data 
{
   char *change;
   char *coder;
   char *date;
   time_t mudtime;
};
extern struct changes_data *changes_table;
