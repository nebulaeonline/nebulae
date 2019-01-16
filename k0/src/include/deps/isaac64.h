/*
------------------------------------------------------------------------------
isaac64.h: definitions for a random number generator
Bob Jenkins, 1996, Public Domain
------------------------------------------------------------------------------
*/
#ifndef STANDARD
#include "standard.h"
#endif

#ifndef ISAAC64
#define ISAAC64

#define RANDSIZL   (8)
#define RANDSIZ    (1<<RANDSIZL)

ub8 randrsl[RANDSIZ], randcnt;

/*
------------------------------------------------------------------------------
 If (flag==TRUE), then use the contents of randrsl[0..255] as the seed.
------------------------------------------------------------------------------
*/
VOID InitIsaac64CSPRNG(word flag);

VOID isaac64(VOID);/*
------------------------------------------------------------------------------
 Call rand() to retrieve a single 64-bit random value
------------------------------------------------------------------------------
*/
#define rand() \
   (!randcnt-- ? (isaac64(), randcnt=RANDSIZ-1, randrsl[randcnt]) : \
                 randrsl[randcnt])

UINT64 GetCSPRNG64(UINT64 min, UINT64 max);

#endif  /* RAND */

