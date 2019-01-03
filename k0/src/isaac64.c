/*
------------------------------------------------------------------------------
isaac64.c: My random number generator for 64-bit machines.
By Bob Jenkins, 1996.  Public Domain.
------------------------------------------------------------------------------
*/
#ifndef STANDARD
#include "include/standard.h"
#endif
#ifndef ISAAC64
#include "include/isaac64.h"
#endif

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "include/k0.h"

extern    ub8 randrsl[RANDSIZ], randcnt;
static    ub8 mm[RANDSIZ];
static    ub8 aa=0, bb=0, cc=0;

#define ind(mm,x)  (*(ub8 *)((ub1 *)(mm) + ((x) & ((RANDSIZ-1)<<3))))
#define rngstep(mix,a,b,mm,m,m2,r,x) \
{ \
  x = *m;  \
  a = (mix) + *(m2++); \
  *(m++) = y = ind(mm,x) + a + b; \
  *(r++) = b = ind(mm,y>>RANDSIZL) + x; \
}

void isaac64()
{
  register ub8 a,b,x,y,*m,*m2,*r,*mend;
  m=mm; r=randrsl;
  a = aa; b = bb + (++cc);
  for (m = mm, mend = m2 = m+(RANDSIZ/2); m<mend; )
  {
    rngstep(~(a^(a<<21)), a, b, mm, m, m2, r, x);
    rngstep(  a^(a>>5)  , a, b, mm, m, m2, r, x);
    rngstep(  a^(a<<12) , a, b, mm, m, m2, r, x);
    rngstep(  a^(a>>33) , a, b, mm, m, m2, r, x);
  }
  for (m2 = mm; m2<mend; )
  {
    rngstep(~(a^(a<<21)), a, b, mm, m, m2, r, x);
    rngstep(  a^(a>>5)  , a, b, mm, m, m2, r, x);
    rngstep(  a^(a<<12) , a, b, mm, m, m2, r, x);
    rngstep(  a^(a>>33) , a, b, mm, m, m2, r, x);
  }
  bb = b; aa = a;
}

#define mix(a,b,c,d,e,f,g,h) \
{ \
   a-=e; f^=h>>9;  h+=a; \
   b-=f; g^=a<<9;  a+=b; \
   c-=g; h^=b>>23; b+=c; \
   d-=h; a^=c<<15; c+=d; \
   e-=a; b^=d>>14; d+=e; \
   f-=b; c^=e<<20; e+=f; \
   g-=c; d^=f>>17; f+=g; \
   h-=d; e^=g<<14; g+=h; \
}

void InitIsaac64CSPRNG(word flag)
{
   word i;
   ub8 a,b,c,d,e,f,g,h;
   aa=bb=cc=(ub8)0;
   a=b=c=d=e=f=g=h=0x9e3779b97f4a7c13ULL;  /* the golden ratio */
   
   // Right now we're seeding with the date & time.  Don't do that!! 
   // TODO -- MORE ENTROPY!
   EFI_STATUS efi_time_result = EFI_SUCCESS;
   EFI_TIME system_time;
   EFI_TIME_CAPABILITIES system_time_capabilities;

   efi_time_result = gRT->GetTime(&system_time, &system_time_capabilities);
   if (EFI_ERROR(efi_time_result)) {
       kernel_panic(L"Error obtaining system time from UEFI: %r\n", efi_time_result);
   }
   else {
       if (k0_VERBOSE_DEBUG) {
           Print(L"Obtained system time from UEFI\n");
       }
       UINT8* raw_date = (UINT8*)(VOID*)(&system_time);
       UINT8* raw_randrsl = (UINT8*)(VOID*)(&randrsl);
       int z, k;
       for (z = 0; z < (RANDSIZ / (int)sizeof(system_time)); z++) {
           for (k = 0; k < (int)sizeof(system_time); k++) {
               raw_randrsl[(z * sizeof(system_time)) + k] = raw_date[k];
           }
       }
   }

   for (i=0; i<4; ++i)                    /* scramble it */
   {
     mix(a,b,c,d,e,f,g,h);
   }

   for (i=0; i<RANDSIZ; i+=8)   /* fill in mm[] with messy stuff */
   {
     if (flag)                  /* use all the information in the seed */
     {
       a+=randrsl[i  ]; b+=randrsl[i+1]; c+=randrsl[i+2]; d+=randrsl[i+3];
       e+=randrsl[i+4]; f+=randrsl[i+5]; g+=randrsl[i+6]; h+=randrsl[i+7];
     }
     mix(a,b,c,d,e,f,g,h);
     mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
     mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
   }

   if (flag) 
   {        /* do a second pass to make all of the seed affect all of mm */
     for (i=0; i<RANDSIZ; i+=8)
     {
       a+=mm[i  ]; b+=mm[i+1]; c+=mm[i+2]; d+=mm[i+3];
       e+=mm[i+4]; f+=mm[i+5]; g+=mm[i+6]; h+=mm[i+7];
       mix(a,b,c,d,e,f,g,h);
       mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
       mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
     }
   }

   isaac64();          /* fill in the first set of results */
   randcnt=RANDSIZ;    /* prepare to use the first set of results */

   if (k0_VERBOSE_DEBUG) {
       Print(L"Isaac64 CSPRNG initialized\n");
   }
}

UINT64 GetCSPRNG64(UINT64 minimum, UINT64 maximum) {
    UINT64 num = (rand() % (maximum - minimum + 1)) + minimum;
    return num;
}
