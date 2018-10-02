#include "include/stdint.h"
#include "include/malloc.h"

int32_t strlen(char *s)
{
	int32_t i;
	for(i = 0; s[i] != '\0'; i++)
	{
		;
	}
	return i;
}

void reverse(char s[]) 
{ 
	int32_t c, i, j; 
	for (i = 0, j = strlen(s)-1; i < j; i++, j--)
	{ 
		c = s[i]; 
		s[i] = s[j]; 
		s[j] = c; 
	} 
} 

void itoa(int32_t n, char s[]) 
{ 
	int32_t i, sign; 
	if ((sign = n) < 0)  /* record sign */
	{ 
		n = -n;          /* make n positive */
	} 
	i = 0; 
	do
	{      /* generate digits in reverse order */ 
		s[i++] = n % 10 + '0';  /* get next digit */ 
	}
	while ((n /= 10) > 0);    /* delete it */ 
	if (sign < 0)
	{ 
		s[i++] = '-';
	} 
	s[i] = '\0'; 
	reverse(s); 
}

char *strtok_r(char *input, char *delim, char **save_ptr)
{
	char no = 0;
        if(input == NULL || delim == NULL || input[0] == '\0' || delim[0] == '\0') return NULL;
        for(int32_t i = 0; input[i] != '\0'; i++)
        {
                if(input[i] == delim[0])
                {
                        int32_t save_point = i;
                        i--;
			no = 0;
                        //I found the delimiter now if multi character delim figure out if this is the one
                        for(int32_t j = 0; delim[j] != '\0'; j++)
                        {
                                i++;
                                if(input[i] != delim[j]) 
				{
					no = 1;
					break;
				}
                        }
			if(no == 0)
			{
				//If I get here it has succseeded
                        	input[save_point] = '\0';
                        	i++;
                        	*save_ptr = &(input[i++]);
                        	return &(input[0]);
			}
                }
        }
        return NULL;
}
