#include "include/printf.h"
#include "include/edid.h"

void print_three_letter_code(struct edid_data *edid)
{
        char string[4];
        string[3] = '\0';
        string[2] = edid->header_info.man_id.c + 64;
        string[1] = '\0';
        string[1] = ((edid->header_info.man_id.b_part_1 << 3) | (edid->header_info.man_id.b_part_2)) + 64;
        string[0] = edid->header_info.man_id.a + 64;
        printf("[INFO] Monitor Manufacturer: %s\n", string);
}


