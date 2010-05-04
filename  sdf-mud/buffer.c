/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#include <stdarg.h>
#include <string.h>
#include "mud.h"

/*
 Implementation of a dynamically expanding buffer.
 
 Inspired by Russ Taylor's <rtaylor@efn.org> buffers in ROM 2.4b2.
 
 The buffer is primarily used for null-terminated character strings.
 
 A buffer is allocated with buffer_new, written to using buffer_strcat,
 cleared (if needed) using buffer_clear and free'ed using buffer_free.
 
 If BUFFER_DEBUG is defined, the buffer_strcat call is defined as having
 2 extra parameters, __LINE__ and __FILE__. These are then saved
 to the bug file if an overflow occurs.
 
 Erwin S. Andreasen <erwin@pip.dknet.dk>

*/
void bugf( char *fmt, ... ) __attribute__ ( ( format( printf, 1, 2 ) ) );
#if 0
#define EMEM_SIZE -1 /* find_mem_size returns this when block is too large */
#endif
#define NUL '\0'
#if 0
#define MAX_MEM_LIST 11
const int rgSizeList[MAX_MEM_LIST] = {
   16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768 - 64
};

/* Find in rgSizeList a memory size at least this long */
int find_mem_size( int min_size )
{
   int i;

   for( i = 0; i < MAX_MEM_LIST; i++ )
      if( rgSizeList[i] >= min_size )
         return rgSizeList[i];

   /*
    * min_size is bigger than biggest allowable size! 
    */

   return EMEM_SIZE;
}
#endif
/* Create a new buffer, of at least size bytes */

#ifndef BUFFER_DEBUG /* no debugging */
BUFFER *__buffer_new( int min_size )
#else /* debugging - expect filename and line */
BUFFER *__buffer_new( int min_size, const char *file, unsigned line )
#endif
{
   int size;
   BUFFER *buffer;
   /*
    * char buf[200];
    *//*
    * for the bug line 
    */
/*
    size = find_mem_size (min_size);
    */
   size = min_size;
#if 0
   if( size == EMEM_SIZE )
   {
#ifdef BUFFER_DEBUG
      sprintf( buf, "Buffer size too big: %d bytes (%s:%u).", min_size, file, line );
#else
      sprintf( buf, "Buffer size too big: %d bytes.", min_size );
#endif

      bug( buf, 0 );
      abort(  );
   }
#endif
   CREATE( buffer, BUFFER, 1 );
   buffer->size = size;
   CREATE( buffer->data, char, size );
   buffer->overflowed = FALSE;

   buffer->len = 0;

   return buffer;
}  /* __buf_new */

/* Add a string to a buffer. Expand if necessary */

#ifndef BUFFER_DEBUG /* no debugging */
void __buffer_strcat( BUFFER * buffer, const char *text )
#else /* debugging - expect filename and line */
void __buffer_strcat( BUFFER * buffer, const char *text, const char *file, unsigned line )
#endif
{
   int new_size;
   int text_len;
   char *new_data;
/*    char buf[200]; */

   if( buffer->overflowed )   /* Do not attempt to add anymore if buffer is already overflowed */
      return;

   if( !text ) /* Adding NULL string ? */
      return;

   text_len = strlen( text );

   if( text_len == 0 )  /* Adding empty string ? */
      return;

   /*
    * Will the combined len of the added text and the current text exceed our buffer? 
    */

   if( ( text_len + buffer->len + 1 ) > buffer->size )   /* expand? */
   {
      // new_size = find_mem_size (buffer->size + text_len + 1);
      new_size = buffer->size + text_len + 1;
#if 0
      if( new_size == EMEM_SIZE )   /* New size too big ? */
      {
#ifdef BUFFER_DEBUG
         sprintf( buf, "Buffer overflow, wanted %d bytes (%s:%u).", text_len + buffer->len, file, line );
#else
         sprintf( buf, "Buffer overflow, wanted %d bytes.", text_len + buffer->len );
#endif
         bug( buf, 0 );
         buffer->overflowed = TRUE;
         return;
      }
#endif
      /*
       * Allocate the new buffer 
       */
      CREATE( new_data, char, new_size );
      /*
       * Copy the current buffer to the new buffer 
       */

      memcpy( new_data, buffer->data, buffer->len );
      DISPOSE( buffer->data );
      buffer->data = new_data;
      buffer->size = new_size;

   }  /* if */

   memcpy( buffer->data + buffer->len, text, text_len ); /* Start copying */
   buffer->len += text_len;   /* Adjust length */
   buffer->data[buffer->len] = NUL; /* Null-terminate at new end */

}  /* __buf_strcat */


/* Free a buffer */
void buffer_free( BUFFER * buffer )
{
   /*
    * Free data 
    */
   DISPOSE( buffer->data );

   /*
    * Free buffer 
    */
   DISPOSE( buffer );
}

/*
 * Clear a buffer's contents...
 * It now clears it completely and resets the buffer to 200
 */
void buffer_clear( BUFFER * buffer )
{
   buffer->overflowed = FALSE;
   DISPOSE( buffer->data );
   CREATE( buffer->data, char, 200 );
   buffer->size = 200;
   buffer->len = 0;
}

/* print stuff, append to buffer. safe. */
int bprintf( BUFFER * buffer, char *fmt, ... )
{
   char buf[MAX_STRING_LENGTH];
   va_list va;
   int res;

   va_start( va, fmt );
   res = vsnprintf( buf, MAX_STRING_LENGTH, fmt, va );
   va_end( va );

   if( res >= MAX_STRING_LENGTH - 1 )
   {
      buf[0] = NUL;
      bugf( "Overflow when printing string %s", fmt );
   }
   else
      buffer_strcat( buffer, buf );

   return res;
}

void bugf( char *fmt, ... )
{
   char buf[MAX_STRING_LENGTH];
   va_list args;
   va_start( args, fmt );
   vsnprintf( buf, MAX_STRING_LENGTH, fmt, args );
   va_end( args );

   bug( buf, 0 );
}
