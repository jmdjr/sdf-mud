/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 * mccp.c - support functions for mccp (the Mud Client Compression Protocol)
 *
 * see http://homepages.ihug.co.nz/~icecube/compress/ and README.Rom24-mccp
 *
 * Copyright (c) 1999, Oliver Jowett <icecube@ihug.co.nz>.
 *
 * This code may be freely distributed and used if this copyright notice is
 * retained intact.
 */

#ifdef MCCP

#include <string.h>
#include <errno.h>
#include <arpa/telnet.h>
#include "mud.h"

char compress_start[] = { IAC, SB, TELOPT_COMPRESS, WILL, SE, '\0' };

bool write_to_descriptor( DESCRIPTOR_DATA * d, char *txt, int length );

/*
 * Memory management - zlib uses these hooks to allocate and free the
 * memory it needs
 */
void *zlib_alloc( void *opaque, unsigned int items, unsigned int size )
{
   return calloc( items, size );
}

void zlib_free( void *opaque, void *address )
{
   DISPOSE( address );
}

/* Try to send any pending compressed-but-not-sent data in 'd' */
bool processCompressed( DESCRIPTOR_DATA * d )
{
   int iStart = 0, nBlock, nWrite, len;

   if( !d->out_compress )
      return TRUE;

   /*
    * Try to write out some data.. 
    */
   len = d->out_compress->next_out - d->out_compress_buf;

   if( len > 0 )
   {
      /*
       * we have some data to write 
       */

      for( iStart = 0; iStart < len; iStart += nWrite )
      {
         nBlock = UMIN( len - iStart, 4096 );
         if( ( nWrite = write( d->descriptor, d->out_compress_buf + iStart, nBlock ) ) < 0 )
         {
            if( errno == EAGAIN || errno == ENOSR )
               break;

            return FALSE;  /* write error */
         }

         if( nWrite <= 0 )
            break;
      }

      if( iStart )
      {
         /*
          * We wrote "iStart" bytes 
          */
         if( iStart < len )
            memmove( d->out_compress_buf, d->out_compress_buf + iStart, len - iStart );
         d->out_compress->next_out = d->out_compress_buf + len - iStart;
      }
   }

   return TRUE;
}

/*
 * Begin compressing data on 'd'
 */
bool compressStart( DESCRIPTOR_DATA * d )
{
   z_stream *s;

   if( d->out_compress )
      return TRUE;

   CREATE( s, z_stream, 1 );
   CREATE( d->out_compress_buf, unsigned char, COMPRESS_BUF_SIZE );

   s->next_in = NULL;
   s->avail_in = 0;

   s->next_out = d->out_compress_buf;
   s->avail_out = COMPRESS_BUF_SIZE;

   s->zalloc = zlib_alloc;
   s->zfree = zlib_free;
   s->opaque = NULL;

   if( deflateInit( s, 9 ) != Z_OK )
   {
      DISPOSE( d->out_compress_buf );
      DISPOSE( s );
      return FALSE;
   }

   write_to_descriptor( d, compress_start, strlen( compress_start ) );
   d->out_compress = s;

   return TRUE;
}

/* Cleanly shut down compression on 'd' */
bool compressEnd( DESCRIPTOR_DATA * d )
{
   unsigned char dummy[1];

   if( !d->out_compress )
      return TRUE;

   d->out_compress->avail_in = 0;
   d->out_compress->next_in = dummy;

   if( deflate( d->out_compress, Z_FINISH ) != Z_STREAM_END )
      return FALSE;

   if( !processCompressed( d ) ) /* try to send any residual data */
      return FALSE;

   deflateEnd( d->out_compress );
   DISPOSE( d->out_compress_buf );
   DISPOSE( d->out_compress );

   return TRUE;
}

/* write compressed data to user */
bool writeCompressed( DESCRIPTOR_DATA * d, char *txt, int length )
{
   z_stream *s = d->out_compress;

   s->next_in = ( unsigned char * )txt;

   if( length <= 0 )
      length = strlen( txt );

   s->avail_in = length;

   while( s->avail_in )
   {
      s->avail_out = COMPRESS_BUF_SIZE - ( s->next_out - d->out_compress_buf );

      if( s->avail_out )
      {
         int status = deflate( s, Z_SYNC_FLUSH );

         if( status != Z_OK )
            return FALSE;
      }

      if( !processCompressed( d ) )
         return FALSE;
/*
        len = d->out_compress->next_out - d->out_compress_buf;
        if (len > 0)
        {
            for (iStart = 0; iStart < len; iStart += nWrite)
            {
                nBlock = UMIN (len - iStart, 4096);
                if ((nWrite = write(d->descriptor, d->out_compress_buf + iStart, nBlock)) < 0)
                {
                    perror( "Write_to_descriptor: compressed" );
                    return FALSE;
                }

                if (!nWrite)
                    break;
            }
            if (!iStart)
                break;

            if (iStart < len)
                memmove(d->out_compress_buf, d->out_compress_buf+iStart, len - iStart);

            d->out_compress->next_out = d->out_compress_buf + len - iStart;
        } */
   }
   return TRUE;
}

void do_compress( CHAR_DATA * ch, char *argument )
{
   if( !ch->desc )
   {
      send_to_char( "What descriptor?!\n", ch );
      return;
   }

   if( !ch->desc->out_compress )
   {
      if( !compressStart( ch->desc ) )
      {
         send_to_char( "Failed.\n", ch );
         return;
      }
      send_to_char( "Ok, compression enabled.\n", ch );
   }
   else
   {
      if( !compressEnd( ch->desc ) )
      {
         send_to_char( "Failed.\n", ch );
         return;
      }
      send_to_char( "Ok, compression disabled.\n", ch );
   }
}

#endif
