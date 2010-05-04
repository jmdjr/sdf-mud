/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*** buffer.h ****/

#define BUFFER_DEBUG

typedef struct buffer_type BUFFER;

struct buffer_type
{
   char *data; /* The data */

   size_t len; /* The current len of the buffer */
   size_t size;   /* The allocated size of data */

   bool overflowed;  /* Is the buffer overflowed? */
};

#ifdef BUFFER_DEBUG  /* Debugged version */

#define buffer_new(size)           __buffer_new (size, __FILE__, __LINE__)
#define buffer_strcat(buffer,text) __buffer_strcat (buffer, text, __FILE__, __LINE__)

BUFFER *__buffer_new( int size, const char *file, unsigned line );
void __buffer_strcat( BUFFER * buffer, const char *text, const char *file, unsigned line );

#else /* not debugged version */

#define buffer_new(size)           __buffer_new (size)
#define buffer_strcat(buffer,text) __buffer_strcat (buffer,text)

BUFFER *__buffer_new( int size );
void __buffer_strcat( BUFFER * buffer, const char *text );

#endif

void buffer_free( BUFFER * buffer );
void buffer_clear( BUFFER * buffer );
int find_mem_size( int min_size );
int bprintf( BUFFER * buffer, char *fmt, ... );
