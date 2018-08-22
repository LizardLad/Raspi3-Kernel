#include "headers/project.h"

int memcmp(signed char *cs_in, char *ct_in, size_t n)
{
  size_t i;
  const unsigned char * cs = (const unsigned char*) cs_in;
  const unsigned char * ct = (const unsigned char*) ct_in;

  for (i = 0; i < n; i++, cs++, ct++)
  {
    if (*cs < *ct)
    {
      return -1;
    }
    else if (*cs > *ct)
    {
      return 1;
    }
  }
  return 0;
}
