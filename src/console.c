#include <stdint.h>
#include "include/malloc.h"
#include "include/string.h"
#include "include/lfb.h"
#include "include/GLES.h"
#include "include/scene.h"
#include "include/console.h"

//Good luck is you are trying to debug this file
//The function here is a wrapper around lfb_print to 
//allow for a scrolling buffer
//Since there are no HID yet there is no need to implement scrollback
//Since we have malloc (Yay!) we can determine the number of lines able to be displayed and how many 
//characters are in each line programmaticly
//We know that each of the characters are the same size in width and height the width and height of
//each character is stored in the font so font_width / total_screen_width is the number of characters
//that can be displayed horisontally.
//The same applies to the vertical
//use font_height / total_screen_height
//Once they are figured out we need to calculate how much memory needs to ba allocated to do this the
//calculation number_of_chars_width * number_of_chars_height is required this allows for that number 
//of bytes to be stored
//Newlines that were placed in the printed string will be kept however when this funtion is printing to
//the screen it will insert newlines where required to create a wrap around effectthis will only be used
//in the case that a newline wasn't encountered and the end of the screen was encountered
//The newline will not be exclusive to whitespace it will split strings where required

char *console_rollback_buffer;
psf_t *font;

void console_init()
{
	//Need to allocate memory
	//lfb_init needs to be called before this will work properly
	font = (psf_t*)&_binary_src_font_font_psf_start;
	console_rollback_buffer = malloc((lfb_width / font->width) * (lfb_height / font->height) + 1);
	if(console_rollback_buffer == NULL)
	{
		;//Eventually raise error
	}
	//Yay we now have the allocated memory
	//Now we have that memory we can initialise the array to 255 because lfb_print ignores it
	for(uint64_t i = 0; i < (lfb_width / font->width) * (lfb_height / font->height); i++)
	{
		console_rollback_buffer[i] = 255;
	}
	console_rollback_buffer[(lfb_width / font->width) * (lfb_height / font->height) + 1] = '\0';
	//We are done here!
}

void console_print_char(char c)
{
	char temp[2];
	temp[0] = c;
	temp[1] = 0;
	console_print(temp);
}

void console_print(char *input)
{
	uint64_t input_length = strlen(input);
	//Ok so now i need to shift the memory in the array becasue I'm not using virtual memory
	//I have to do this left to right because lfb_print opperates top to bottom so it all needs to 
	//be shifted left to right by the input length
	//
	//I also need to take into account once the number of newline characters already in the array
	//lfb_print ignores anything that is a 255 so anything that should't be taken into consideration
	//by lfb_print shall be set to a 255
	uint16_t number_of_lines = (lfb_height / font->height);	
	//So there should always be number_of_lines - 1 '\n' characters in the array
	//If there are more well they need to be removed along with all previous characters so they 
	//need to be set to the value 255
	//
	//There always needs to be checking that I am not allocating to a negative array index
	
	for(int64_t i = 1; i <= input_length; i++) //I need to shift everything by the input_length
	{
		for(int64_t j = 1; j < (lfb_width / font->width) * (lfb_height / font->height); j++)
		{
			console_rollback_buffer[j-1] = console_rollback_buffer[j];
		}
	}
	
	int64_t j = input_length;
	for(int64_t i = (lfb_width / font->width) * (lfb_height / font->height); (i > 0 || i == 0) && (j > 0 || j == 0); i--)
	{
		console_rollback_buffer[i] = input[j];
		j--;
	}
	
	uint16_t characters_since_newline = 0;
	for(uint64_t i = 0; i <= (lfb_width / font->width) * (lfb_height / font->height); i++)
	{
		if(console_rollback_buffer[i] == '\n')
		{
			//This is where I look for too many characters and place a newline in
			characters_since_newline = 0;
		}
		else if(console_rollback_buffer[i] != 255)
		{
			if(characters_since_newline == (lfb_width / font->width) - 2)
			{
				characters_since_newline = 0; //After this i have to create a space 
							      //that means if there isn't enough space in the 
							      //array remove one line if there is well I have to 
							      //make space by coping enough to the left
				if(console_rollback_buffer[0] == 255) //Enough just to copy
				{
					for(uint64_t k = 1; k <= i; k++)
                                        {
                                                console_rollback_buffer[k-1] = console_rollback_buffer[k];
                                        }
                                        console_rollback_buffer[i-1] = '\n';
				}
				else
				{
					for(uint64_t k = 0; k <= (lfb_width / font->width) * (lfb_height / font->height) - 2; k++)
					{
						if(console_rollback_buffer[k] == '\n')
						{
							while(k)
							{
								console_rollback_buffer[k] = 255;
								k--;
							}
							break;
						}
					}
					//Now do the same as the above
					for(uint64_t k = 1; k <= i; k++)
					{
						console_rollback_buffer[k-1] = console_rollback_buffer[k];
					}
					console_rollback_buffer[i-1] = '\n';
				}
			}
		characters_since_newline++;
		}
	}

	uint64_t number_of_newlines = 0;
	for(uint64_t i = 0; i <= (lfb_width / font->width) * (lfb_height / font->height); i++)
	{
		if(console_rollback_buffer[i] == '\n')
		{
			number_of_newlines++;
		}
	}

	int64_t number_of_newlines_req_diff = (number_of_lines - 1) - number_of_newlines;
	
	//Just before printing clear the screen for now just draw a big black rectangle
	if(number_of_newlines != 0)
	{
		lfb_clear_rect();
		//V3D_RenderScene(&scene);
	}

	if(number_of_newlines_req_diff < 0) //Have too many newlines!
	{
		//Remove enough of them
		for(uint64_t i = 0; i < (lfb_width / font->width) * (lfb_height / font->height); i++)
		{
			if(console_rollback_buffer[i] == '\n')
			{
				number_of_newlines_req_diff++;
			}
			if(number_of_newlines_req_diff == 0)
			{
				while(i)
				{
					console_rollback_buffer[i] = 255;
					i--;
				}
				break;
			}
		}
	}

	else if(number_of_newlines_req_diff > 0) //Don't have enough!
	{
		//So instead of adding more newlines use the print offset in lfb_print
		//number_of_newlines_req_diff will work as the y input
		lfb_print(0, number_of_newlines_req_diff, console_rollback_buffer);
	}
	
	//Ok so in the case of not having enough newlines is done
	if(number_of_newlines_req_diff == 0)
	{
		lfb_print(0, 0, console_rollback_buffer);
	}
}	
