/*
 * defines for the dmalloc library
 *
 * Copyright 1995 by Gray Watson
 *
 * This file is part of the dmalloc package.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * NON-COMMERCIAL purpose and without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies, and that the name of Gray Watson not be used in
 * advertising or publicity pertaining to distribution of the document
 * or software without specific, written prior permission.
 *
 * Gray Watson makes no representations about the suitability of the
 * software described herein for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * The author may be contacted via http://www.letters.com/~gray/
 *
 * $Id: dmalloc.h.1,v 1.9 1998/10/26 14:24:31 gray Exp $
 */

#ifndef __DMALLOC_H__
#define __DMALLOC_H__

/* this is dmalloc.h */
/* produced by configure, inserted into dmalloc.h */

/* the const operator */
/* const is available */

/*
 * the definition of DMALLOC_SIZE
 *
 * NOTE: some architectures have malloc, realloc, etc.
 * using unsigned instead of unsigned long.  You may
 * have to edit this by hand to fix any compilation
 * warnings or errors.
 */
#include <sys/types.h>
#define DMALLOC_SIZE size_t

/* NOTE: start of $Id: dmalloc.h.3,v 1.25 1998/11/09 18:11:55 gray Exp $ */

/* this defines what type the standard void memory-pointer is */
#if defined(__STDC__) && __STDC__ == 1
#define DMALLOC_PNT		void *
#define DMALLOC_FREE_RET	void
#else
#define DMALLOC_PNT		char *
#define DMALLOC_FREE_RET	int
#endif

/*
 * Malloc function return codes
 */
#define CALLOC_ERROR		0L		/* error from calloc */
#define MALLOC_ERROR		0L		/* error from malloc */
#define REALLOC_ERROR		0L		/* error from realloc */

/* NOTE: this if for non- __STDC__ systems only */
#define FREE_ERROR		0		/* error from free */
#define FREE_NOERROR		1		/* no error from free */

#define DMALLOC_VERIFY_ERROR	0		/* checks failed, error */
#define DMALLOC_VERIFY_NOERROR	1		/* checks passed, no error */

#ifdef __cplusplus
extern "C" {
#endif

/* logfile for dumping dmalloc info, DMALLOC_LOGFILE env var overrides this */
extern	char		*dmalloc_logpath;

/* internal dmalloc error number for reference purposes only */
extern	int		dmalloc_errno;

/* address to look for.  when discovered call dmalloc_error() */
extern	DMALLOC_PNT	dmalloc_address;

/*
 * argument to dmalloc_address, if 0 then never call dmalloc_error()
 * else call it after seeing dmalloc_address for this many times.
 */
extern	int		dmalloc_address_count;

/*
 * shutdown memory-allocation module, provide statistics if necessary
 */
extern	void	dmalloc_shutdown(void);

/*
 * allocate and return a SIZE block of bytes.  returns 0L on error.
 */
extern	DMALLOC_PNT	malloc(DMALLOC_SIZE size);

/*
 * allocate and return a block of _zeroed_ bytes able to hold
 * NUM_ELEMENTS, each element contains SIZE bytes.  returns 0L on
 * error.
 */
extern	DMALLOC_PNT	calloc(DMALLOC_SIZE num_elements, DMALLOC_SIZE size);

/*
 * Resizes OLD_PNT to NEW_SIZE bytes and return the new space after
 * either copying all of OLD_PNT to the new area or truncating.  If
 * OLD_PNT is 0L then it will do the equivalent of malloc(NEW_SIZE).
 * If NEW_SIZE is 0 and OLD_PNT is not 0L then it will do the
 * equivalent of free(OLD_PNT) and will return 0L.  Returns 0L on
 * error.
 */
extern	DMALLOC_PNT	realloc(DMALLOC_PNT old_pnt, DMALLOC_SIZE new_size);

/*
 * Resizes OLD_PNT to NEW_SIZE bytes and return the new space after
 * either copying all of OLD_PNT to the new area or truncating.  If
 * OLD_PNT is 0L then it will do the equivalent of malloc(NEW_SIZE).
 * If NEW_SIZE is 0 and OLD_PNT is not 0L then it will do the
 * equivalent of free(OLD_PNT) and will return 0L.  Any extended
 * memory space will be zeroed like calloc.  Returns 0L on error.
 */
extern	DMALLOC_PNT	recalloc(DMALLOC_PNT old_pnt, DMALLOC_SIZE new_size);

/*
 * Allocate and return a SIZE block of bytes that has been aligned to
 * ALIGNMENT bytes.  ALIGNMENT must be a power of two and must be less
 * than or equal to the block-size.  Returns 0L on error.
 */
extern	DMALLOC_PNT	memalign(DMALLOC_SIZE alignment, DMALLOC_SIZE size);

/*
 * Allocate and return a SIZE block of bytes that starts on a page
 * boundary.  Returns 0L on error.
 */
extern	DMALLOC_PNT	valloc(DMALLOC_SIZE size);

/*
 * release PNT in the heap, returning FREE_ERROR, FREE_NOERROR or void
 * depending on whether STDC is defined by your compiler.
 */
extern	DMALLOC_FREE_RET	free(DMALLOC_PNT pnt);

/*
 * same as free(PNT)
 */
extern	DMALLOC_FREE_RET	cfree(DMALLOC_PNT pnt);

/*
 * log the heap structure plus information on the blocks if necessary.
 */
extern	void	dmalloc_log_heap_map(void);

/*
 * dump dmalloc statistics to logfile
 */
extern	void	dmalloc_log_stats(void);

/*
 * dump unfreed-memory info to logfile
 */
extern	void	dmalloc_log_unfreed(void);

/*
 * verify pointer PNT, if PNT is 0 then check the entire heap.
 * returns MALLOC_VERIFY_ERROR or MALLOC_VERIFY_NOERROR
 */
extern	int	dmalloc_verify(const DMALLOC_PNT pnt);

/*
 * same as dmalloc_verify
 */
extern	int	malloc_verify(const DMALLOC_PNT pnt);

/*
 * set the global debug functionality FLAGS to debug (0 to disable all
 * debugging).  NOTE: after this module has started up, you cannot set
 * certain flags such as fence-post or free-space checking.
 */
extern	void	dmalloc_debug(const int flags);

/*
 * returns the current debug functionality flags.  this allows you to
 * save a dmalloc library state to be restored later.
 */
extern	int	dmalloc_debug_current(void);

/*
 * examine pointer PNT and returns SIZE, and FILE / LINE info on it,
 * or return-address RET_ADDR if any of the pointers are not 0L.
 * if FILE returns 0L then RET_ATTR may have a value and vice versa.
 * returns NOERROR or ERROR depending on whether PNT is good or not
 */
extern	int	dmalloc_examine(const DMALLOC_PNT pnt, DMALLOC_SIZE * size,
				 char ** file, unsigned int * line,
				 DMALLOC_PNT * ret_attr);

/*
 * Dmalloc version of strerror to return the string version of
 * ERROR_NUM.  Returns an invaid errno string if ERROR_NUM is
 * out-of-range.
 */
extern	const char	*dmalloc_strerror(const int error_num);

#ifdef __cplusplus
}
#endif

/*
 * alloc macros to provide for memory FILE/LINE debugging information.
 */

#ifndef DMALLOC_DISABLE

#undef malloc
#define malloc(size) \
  _malloc_leap(__FILE__, __LINE__, size)
#undef calloc
#define calloc(count, size) \
  _calloc_leap(__FILE__, __LINE__, count, size)
#undef realloc
#define realloc(ptr, size) \
  _realloc_leap(__FILE__, __LINE__, ptr, size)
#undef recalloc
#define recalloc(ptr, size) \
  _recalloc_leap(__FILE__, __LINE__, ptr, size)
#undef memalign
#define memalign(alignment, size) \
  _memalign_leap(__FILE__, __LINE__, alignment, size)
#undef valloc
#define valloc(size) \
  _valloc_leap(__FILE__, __LINE__, size)
#undef strdup
#define strdup(str) \
  _strdup_leap(__FILE__, __LINE__, str)
#undef free
#define free(ptr) \
  _free_leap(__FILE__, __LINE__, ptr)

#undef xmalloc
#define xmalloc(size) \
  _xmalloc_leap(__FILE__, __LINE__, size)
#undef xcalloc
#define xcalloc(count, size) \
  _xcalloc_leap(__FILE__, __LINE__, count, size)
#undef xrealloc
#define xrealloc(ptr, size) \
  _xrealloc_leap(__FILE__, __LINE__, ptr, size)
#undef xrecalloc
#define xrecalloc(ptr, size) \
  _xrecalloc_leap(__FILE__, __LINE__, ptr, size)
#undef xmemalign
#define xmemalign(alignment, size) \
  _xmemalign_leap(__FILE__, __LINE__, alignment, size)
#undef xvalloc
#define xvalloc(size) \
  _xvalloc_leap(__FILE__, __LINE__, size)
#undef xstrdup
#define xstrdup(str) \
  _xstrdup_leap(__FILE__, __LINE__, str)
#undef xfree
#define xfree(ptr) \
  _xfree_leap(__FILE__, __LINE__, ptr)

#ifdef DMALLOC_FUNC_CHECK

/*
 * do debugging on the following functions.  this may cause compilation or
 * other problems depending on your architecture.
 */
#undef bcmp
#define bcmp(b1, b2, len)		_dmalloc_bcmp(b1, b2, len)
#undef bcopy
#define bcopy(from, to, len)		_dmalloc_bcopy(from, to, len)

#undef memcmp
#define memcmp(b1, b2, len)		_dmalloc_memcmp(b1, b2, len)
#undef memcpy
#define memcpy(to, from, len)		_dmalloc_memcpy(to, from, len)
#undef memset
#define memset(buf, ch, len)		_dmalloc_memset(buf, ch, len)

#undef index
#define index(str, ch)			_dmalloc_index(str, ch)
#undef rindex
#define rindex(str, ch)			_dmalloc_rindex(str, ch)

#undef strcat
#define strcat(to, from)		_dmalloc_strcat(to, from)
#undef strcmp
#define strcmp(s1, s2)			_dmalloc_strcmp(s1, s2)
#undef strlen
#define strlen(str)			_dmalloc_strlen(str)
#undef strtok
#define strtok(str, sep)		_dmalloc_strtok(str, sep)

#undef bzero
#define bzero(buf, len)			_dmalloc_bzero(buf, len)

#undef memccpy
#define memccpy(s1, s2, ch, len)	_dmalloc_memccpy(s1, s2, ch, len)
#undef memchr
#define memchr(s1, ch, len)		_dmalloc_memchr(s1, ch, len)

#undef strchr
#define strchr(str, ch)			_dmalloc_strchr(str, ch)
#undef strrchr
#define strrchr(str, ch)		_dmalloc_strrchr(str, ch)

#undef strcpy
#define strcpy(to, from)		_dmalloc_strcpy(to, from)
#undef strncpy
#define strncpy(to, from, len)		_dmalloc_strncpy(to, from, len)
#undef strcasecmp
#define strcasecmp(s1, s2)		_dmalloc_strcasecmp(s1, s2)
#undef strncasecmp
#define strncasecmp(s1, s2, len)	_dmalloc_strncasecmp(s1, s2, len)
#undef strspn
#define strspn(str, list)		_dmalloc_strspn(str, list)
#undef strcspn
#define strcspn(str, list)		_dmalloc_strcspn(str, list)
#undef strncat
#define strncat(to, from, len)		_dmalloc_strncat(to, from, len)
#undef strncmp
#define strncmp(s1, s2, len)		_dmalloc_strncmp(s1, s2, len)
#undef strpbrk
#define strpbrk(str, list)		_dmalloc_strpbrk(str, list)
#undef strstr
#define strstr(str, pat)		_dmalloc_strstr(str, pat)

#endif /* ! DMALLOC_DISABLE */
#endif /* DMALLOC_FUNC_CHECK */

/* we start FUNC_CHECK again because we had to stop it above */
#ifdef DMALLOC_FUNC_CHECK

/*
 * feel free to add your favorite functions here and to arg_check.[ch]
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * copied in from arg_check.h with all comments removed
 */
extern	int	_dmalloc_bcmp(const void * b1, const void * b2,
			      const DMALLOC_SIZE len);
extern	void	_dmalloc_bcopy(const char *from, char *to,
			       const DMALLOC_SIZE len);
extern	int	_dmalloc_memcmp(const void * b1, const void * b2,
				const DMALLOC_SIZE len);
extern	char	*_dmalloc_memcpy(char *to, const char *from,
				 const DMALLOC_SIZE len);
extern	char	*_dmalloc_memset(void * buf, const char ch,
				 const DMALLOC_SIZE len);
extern	char	*_dmalloc_index(const char *str, const char ch);
extern	char	*_dmalloc_rindex(const char *str, const char ch);
extern	char	*_dmalloc_strcat(char *to, const char *from);
extern	int	_dmalloc_strcmp(const char *s1, const char *s2);
extern	DMALLOC_SIZE	_dmalloc_strlen(const char *str);
extern	char	*_dmalloc_strtok(char *str, const char *sep);
extern	void	_dmalloc_bzero(void * buf, const DMALLOC_SIZE len);
extern	char	*_dmalloc_memccpy(char *s1, const char *s2, const char ch,
				  const DMALLOC_SIZE len);
extern	char	*_dmalloc_memchr(const char *s1, const char ch,
				 const DMALLOC_SIZE len);
extern	char	*_dmalloc_strchr(const char *str, const char ch);
extern	char	*_dmalloc_strrchr(const char *str, const char ch);
extern	char	*_dmalloc_strcpy(char *to, const char *from);
extern	char	*_dmalloc_strncpy(char *to, const char *from,
				  const DMALLOC_SIZE len);
extern	int	_dmalloc_strcasecmp(const char *s1, const char *s2);
extern	int	_dmalloc_strncasecmp(const char *s1, const char *s2,
				     const DMALLOC_SIZE len);
extern	int	_dmalloc_strspn(const char *str, const char *list);
extern	int	_dmalloc_strcspn(const char *str, const char *list);
extern	char	*_dmalloc_strncat(char *to, const char *from,
				  const DMALLOC_SIZE len);
extern	int	_dmalloc_strncmp(const char *s1, const char *s2,
				 const DMALLOC_SIZE len);
extern	char	*_dmalloc_strpbrk(const char *str, const char *list);
extern	char	*_dmalloc_strstr(const char *str, const char *pat);

#ifdef __cplusplus
}
#endif

#endif /* DMALLOC_FUNC_CHECK */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copied directly from malloc_lp.h, removed some routines
 */

/* internal dmalloc error number for reference purposes only */
extern
int		dmalloc_errno;

/*
 * leap routine to malloc
 */
extern
DMALLOC_PNT	_malloc_leap(const char *file, const int line,
			     DMALLOC_SIZE size);

/*
 * leap routine to calloc
 */
extern
DMALLOC_PNT	_calloc_leap(const char *file, const int line,
			     DMALLOC_SIZE ele_n, DMALLOC_SIZE size);

/*
 * leap routine to realloc
 */
extern
DMALLOC_PNT	_realloc_leap(const char *file, const int line,
			      DMALLOC_PNT old_p, DMALLOC_SIZE new_size);

/*
 * leap routine to recalloc
 */
extern
DMALLOC_PNT	_recalloc_leap(const char *file, const int line,
			       DMALLOC_PNT old_p, DMALLOC_SIZE new_size);

/*
 * leap routine to memalign
 */
extern
DMALLOC_PNT	_memalign_leap(const char *file, const int line,
			       DMALLOC_SIZE alignment, DMALLOC_SIZE size);

/*
 * leap routine to valloc
 */
extern
DMALLOC_PNT	_valloc_leap(const char *file, const int line,
			     DMALLOC_SIZE size);

/*
 * leap routine to free
 */
extern
DMALLOC_FREE_RET	_free_leap(const char *file, const int line,
				   DMALLOC_PNT pnt);

/*
 * leap routine to strdup
 */
extern
char	*_strdup_leap(const char *file, const int line,
		      const char *str);

/*
 * leap routine to malloc with error checking
 */
extern
DMALLOC_PNT	_xmalloc_leap(const char *file, const int line,
			      DMALLOC_SIZE size);

/*
 * leap routine to calloc with error checking
 */
extern
DMALLOC_PNT	_xcalloc_leap(const char *file, const int line,
			      DMALLOC_SIZE ele_n, DMALLOC_SIZE size);

/*
 * leap routine to realloc with error checking
 */
extern
DMALLOC_PNT	_xrealloc_leap(const char *file, const int line,
			       DMALLOC_PNT old_p, DMALLOC_SIZE new_size);

/*
 * leap routine to realloc with error checking
 */
extern
DMALLOC_PNT	_xrecalloc_leap(const char *file, const int line,
				DMALLOC_PNT old_p, DMALLOC_SIZE new_size);

/*
 * leap routine to memalign with error checking
 */
extern
DMALLOC_PNT	_xmemalign_leap(const char *file, const int line,
				DMALLOC_SIZE alignment, DMALLOC_SIZE size);

/*
 * leap routine to valloc with error checking
 */
extern
DMALLOC_PNT	_xvalloc_leap(const char *file, const int line,
			      DMALLOC_SIZE size);

/*
 * leap routine for strdup with error checking
 */
extern
char 	*_xstrdup_leap(const char *file, const int line,
		       const char *str);

/*
 * leap routine to free
 */
extern
DMALLOC_FREE_RET	_xfree_leap(const char *file, const int line,
				    DMALLOC_PNT pnt);

#ifdef __cplusplus
}
#endif

#endif /* ! __DMALLOC_H__ */
