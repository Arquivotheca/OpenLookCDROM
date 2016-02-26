/*                                                         HTFWriter:  File Writer for libwww
                                      C FILE WRITER
                                             
   It is useful to have both FWriter and Writer for environments in which fdopen() doesn't
   exist for example.
   
   Part of libwww . Implemented by HTFWriter.c
   
 */
#ifndef HTFWRITE_H
#define HTFWRITE_H

#include "HTStream.h"
#include <stdio.h>
#include "HTFormat.h"

#ifdef SHORT_NAMES
#define HTFWriter_new   HTFWnew
#endif

/*

Stream given file

 */
extern HTStream * HTFWriter_new PARAMS((FILE *  fp,
                                             BOOL       leave_open));
/*

   OLD: (HENRIK 08/02-94) extern HTStream * HTFWriter_new PARAMS((FILE * fp));
   
   Converters
   
   HTSaveAndCallBack will save to a cache file and call the request->callback function
   with the filename as parameter.
   
 */
extern HTConverter HTSaveAndExecute, HTSaveLocally, HTSaveAndCallBack,
HTThroughLine;

/*

The cache

   The cache contains details of temporary disk files which contain the contents of remote
   documents.   There is also a list of cache items for each URL in its anchor object.
   
  AN ITEM IN THE CACHE
  
 */
typedef struct _HTCacheItem {
    HTParentAnchor *    anchor;
    HTFormat            format;         /* May have many formats per anchor */
    char *              filename;
    time_t              load_time;
    time_t              load_delay;
    int                 reference_count;
} HTCacheItem;

/*

  THE CACHE ITSELF
  
   There is one global cache. No, it's not reentrant code then.  If we find a use for >1
   cache then we can change it. Currently the obvious advantage is when all things which
   could gain from caching use the same one.
   
 */
extern HTList * HTCache;        /* made up of HTCacheItem items */

/*

  THE CACHE LIMIT
  
   The cache limit is the number of files which are kept. Yes, I know, the amount of disk
   space wouldbe more relevant. So this may change. Currentlyit is present to 100 but may
   be changed by the application by writing into this variable.
   
 */
extern int      HTCacheLimit;

/*

  CLEAR A CACHE
  
 */
extern void HTCacheClear PARAMS((HTList * cache));

/*

  TO REMOVE ALL CACHE FILES KNOWN TO THIS SESSION
  
 */

extern void HTCacheDeleteAll NOPARAMS;
/*

  STREAM FOR WRITING TO CACHE
  
   Note that HTSaveAndCallBack will also generate a cache file.
   
 */
extern HTConverter HTCacheWriter;

#endif
/*

   end  */
