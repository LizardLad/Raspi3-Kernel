#include "headers/project.h"

void printf(char *fmt, ...)
{    
	__builtin_va_list args, args2;
	__builtin_va_start(args, fmt);
	__builtin_va_copy(args2, args);
	// malloc space for the string
	char *s = malloc(ammount_to_alloc_with_symbols(fmt, args2));
	// use sprintf to format our string
	vsprintf(s,fmt,args);
	__builtin_va_end(args);
	// print out as usual
	console_print(s);
	free(s);
}

size_t ammount_to_alloc_with_symbols(char *str, __builtin_va_list args)
{
	//The purpose of this function is to determine how much memory needs to be allocated to
	//contain the output of vsprintf
	uint64_t val = 0, len, counter;

	val = strlen(str);

	while(*str)
	{
		if(*str == '%')
		{
			str++;
			//Check for literal %
			if(*str == '%')
			{
				val--;	//Becuase two bytes have already been allocated for a one byte
					//Character
			}

			//Check for size modifier
			len = 0;
			counter = 0;
			while(*str >= '0' && *str <= '9')
			{
				counter++;
				len *= 10;
				len += *str - '0';
				str++;
			}
			val += len;
			val -= counter;

			//Check for %c
			if(*str == 'c')
			{
				val--;				//Becuase there are two characters 
				__builtin_va_arg(args, int32_t);	//denoting a char %c and only one is
			}					//needed
		
			//Check for %d
			if(*str == 'd')
			{
				val += 18;			//Was 20 but reduced by two 
				__builtin_va_arg(args, int32_t);	//because %d already has space
								//allocated
			}
	
			//Check for %x
			if(*str == 'x')
			{
				val += 18;				//Was 20 but reduced by two
				__builtin_va_arg(args, int64_t);	//because %x already has space
			}						//allocated

			//Check for %f
			//Doesn't work yet
			//Yet to be implemented in vsprintf

			//Check for %s
			if(*str == 's')
			{
				val -= 2;       
	        		val += strlen((char *)__builtin_va_arg(args, char *));
			}	
		}
		str++;
	}
	__builtin_va_end(args);
	
	return (size_t)val;
}

