#ifndef edid_h
#define edid_h

#include <stdint.h>

#define HEADER_PATTERN 0x00FFFFFF00

struct __attribute__((__packed__)) edid_manufacturer_id
{
	uint8_t b_part_1: 2;
	uint8_t a: 5;
	uint8_t reserved: 1;
	uint8_t c: 5;
	uint8_t b_part_2: 3;
};

struct __attribute__((__packed__)) edid_header_info
{
	uint64_t fixed_header_pattern;
	struct edid_manufacturer_id man_id;
	uint16_t man_product_code;
	uint32_t serial_number;
	uint8_t man_week;
	uint8_t man_year; //Add 1990 to obtain value range of 1990 - 2245
	uint8_t EEID_major; //Usually 1 (for 1.3)
	uint8_t EEID_minor; //Usually 3 (for 1.3)
};

struct __attribute__((__packed__)) edid_video_input_params_bitmap_analog
{
	uint8_t vsync_pulse_seperated_composite_green_sync_used: 1;
	uint8_t sync_on_green_supported: 1;
	uint8_t composite_sync_on_hsync_supported: 1;
	uint8_t seperate_sync_supported: 1;
	uint8_t blank_to_blank_setup: 1;
	uint8_t video_white_sync_levels: 2;
	uint8_t input_type: 1; //If input_type == 1 then it's digital and this is the WRONG struct!
};

struct __attribute__((__packed__)) edid_video_input_params_bitmap_digital
{
	uint8_t video_interface: 4;
	uint8_t bit_depth: 3;
	uint8_t input_type: 1; //If input_type == 1 then it's digital and this is the CORRECT struct!
};

struct __attribute__((__packed__)) edid_supported_features_bitmap
{
	uint8_t continuous_timings_GFT_CVT: 1;
	uint8_t prefered_timing_mode_index: 1; //Specified in descriptor block 1 differes between EDID versions
	uint8_t standard_RGB_colorspace: 1;
	uint8_t display_type: 2; //Values mean different things depending on digital or analog connection
	uint8_t DPMS_active_off: 1;
	uint8_t DPMS_suspend: 1;
	uint8_t DPMS_standby: 1;
};

struct __attribute__((__packed__)) edid_basic_display_params
{
	uint8_t video_input_params_bitmap; //Cast to digital type first check input type then decide on struct
	uint8_t horizontal_screen_size_cm; //(Range 1-255) unless it is a projector one of these will be 0 in which case landscape aspect ratio (range 1.00-3.54), datavalue = (AR*100)-99 (example 16:9 = 79, 4:3 = 34)
	uint8_t vertical_screen_size_cm; //(Range 1-255)  if(horizontal_screen_size_cm == 0), protrait aspect ratio again like a projector. (Range 0.28-0.99), datavalue = (100/AR)-99 (example 9:16 79, 3:4 = 34). If either byte is 0, screen size and aspect ratio are undefined (e.g. projector)
	uint8_t display_gamma; //Factory default (range 1.00-3.54), datavalue = (gamma*100)-100 = (gamma-1)*100. If 255 gamma id defined by DI-EXT block
	struct edid_supported_features_bitmap supported_features;
};

struct __attribute__((__packed__)) edid_chromaticity_coordinates
{
	uint8_t green_y_least_sig_2_bits: 2;
	uint8_t green_x_least_sig_2_bits: 2;
	uint8_t red_y_least_sig_2_bits: 2;
	uint8_t red_x_least_sig_2_bits: 2;

	uint8_t blue_white_least_significant_2_bits;

	uint8_t red_x_most_sig_8_bits; //(0-255) encodes fractional 0-0.996 (255/256); 0-0.999 (1023/1024) with lsbits
	uint8_t red_y_most_sig_8_bits;
	uint8_t green_x_most_sig_8_bits;
	uint8_t green_y_most_sig_8_bits;
	uint8_t blue_x_most_sig_8_bits;
	uint8_t blue_y_most_sig_8_bits;
	uint8_t default_white_point_x_most_sig_8_bits;
	uint8_t default_white_point_y_most_sig_8_bits;
};

struct __attribute__((__packed__)) edid_timing_bitmap_common
{
	uint8_t manufacturer_reserved: 7;
	uint8_t timing_17: 1;
	uint8_t timing_16: 1;
	uint8_t timing_15: 1;
	uint8_t timing_14: 1;
	uint8_t timing_13: 1;
	uint8_t timing_12: 1;
	uint8_t timing_11: 1;
	uint8_t timing_10: 1;
	uint8_t timing_9: 1;
	uint8_t timing_8: 1;
	uint8_t timing_7: 1;
	uint8_t timing_6: 1;
	uint8_t timing_5: 1;
	uint8_t timing_4: 1;
	uint8_t timing_3: 1;
	uint8_t timing_2: 1;
	uint8_t timing_1: 1;
};

struct __attribute__((__packed__)) edid_standard_timing_information_entry_empty
{
	uint8_t byte_1;
	uint8_t byte_2;
};

struct __attribute__((__packed__)) edid_standard_timing_information_entry
{
	uint8_t x_res; //X resolution, divided by 8 with 31 subtracted from it. True X res = (x_res + 31) * 8
	uint8_t vertical_freq: 6; //60 needs to be added to get true freq
	uint8_t aspect_ratio: 2; 	//0b00 = 16:10 v1.3+ or 0x00 = 1:1 for versions before 1.3
				//0b01 = 4:3
				//0b10 = 5:4
				//0b11 = 16:9
				
};

struct __attribute__((__packed__)) edid_standard_timing_information
{
	struct edid_standard_timing_information_entry entry_1;
	struct edid_standard_timing_information_entry entry_2;
	struct edid_standard_timing_information_entry entry_3;
	struct edid_standard_timing_information_entry entry_4;
	struct edid_standard_timing_information_entry entry_5;
	struct edid_standard_timing_information_entry entry_6;
	struct edid_standard_timing_information_entry entry_7;
	struct edid_standard_timing_information_entry entry_8;
};

struct __attribute__((__packed__)) edid_detailed_timing_features_bitmap_analog_sync
{
	uint8_t sync_on_all_RGB_lines: 1;
	uint8_t vsync_serration: 1;
	uint8_t sync_type: 1;
	uint8_t type: 1; //If 0 this is the correct struct!
};

struct __attribute__((__packed__)) edid_detailed_timing_features_bitmap_digital_composite
{
	uint8_t reserved: 1;
	uint8_t vsync_polarity: 1; //1 = positive
	uint8_t type: 2; //If 2 this is the correct struct!
};

struct __attribute__((__packed__)) edid_detailed_timing_features_bitmap_digital_seperate
{
	uint8_t hsync_polatiry: 1; //1 = positive
	uint8_t sync_serration: 1;
	uint8_t type: 2; //If 3 this is the correct struct!
};

struct __attribute__((__packed__)) edid_detailed_timing_features_bitmap
{
	uint8_t line_interleaved_side_by_side_stereo: 1;
	uint8_t sync_types: 4; //Cast to correct struct this is raw not meaning anything yet!
	uint8_t stereo_mode: 2;
	uint8_t interlaced: 1;
};

struct __attribute__((__packed__)) edid_detailed_timing_descriptor
{
	uint16_t pixel_clock; //In KHz
	uint8_t horizontal_active_lsbits; //(0-4095)
	uint8_t horizontal_blanking_lsbits; //(0-4095)
	uint8_t horizontal_blanking_msbits: 4;
	uint8_t horizontal_active_msbits: 4;
	uint8_t vertical_active_lsbits; //(0-4095)
	uint8_t vertical_blanking_lsbits; //(0-4095)
	uint8_t vertical_blanking_msbits: 4;
	uint8_t vertical_active_msbits: 4;
	
	uint8_t horizontal_front_porch_pixles_lsbits; //(0-1023)
	uint8_t horizontal_sync_pulse_width_pixels_lsbits; //(0-1023)

	uint8_t verical_sync_pulse_width_pixels_lsbits: 4;
	uint8_t verical_front_porch_pixels_lsbits: 4;

	uint8_t vertical_sync_pulse_width_pixels_msbits: 2;
	uint8_t vertical_front_porch_pixels_msbits: 2;
	uint8_t horizontal_sync_pulse_width_pixels_msbits: 2;
	uint8_t horizontal_front_porch_pixels_msbits: 2;

	uint8_t horizontal_image_size_mm_lsbits; //(0-4095)
	uint8_t vertical_image_size_mm_lsbits; //(0-4095)
	uint8_t vertical_image_size_mm_msbits: 4;
	uint8_t horizontal_image_size_mm_msbits: 4;

	uint8_t horizontal_boarder_pixels; //Double to get the full value
	uint8_t vertical_boarder_pixels; //Double to get the full value

	struct edid_detailed_timing_features_bitmap features_bitmap;
};

struct __attribute__((__packed__)) edid_descriptor_block_space
{
	struct edid_detailed_timing_descriptor detailed_timing_descriptor_1;
	struct edid_detailed_timing_descriptor detailed_timing_descriptor_2;
	struct edid_detailed_timing_descriptor detailed_timing_descriptor_3;
	struct edid_detailed_timing_descriptor detailed_timing_descriptor_4;
};

struct __attribute__((__packed__)) edid_last_block
{
	uint8_t num_of_extensions;
	uint8_t checksum;
};

struct __attribute__((__packed__)) edid_data
{
	struct edid_header_info header_info;
	struct edid_basic_display_params basic_display_params;
	struct edid_chromaticity_coordinates chromaticity_coordinates;
	struct edid_timing_bitmap_common common_timing_bitmap;
	struct edid_standard_timing_information standard_timing_information;
	struct edid_descriptor_block_space descriptor_block_space;
	struct edid_last_block last_block;
};

void print_three_letter_code(struct edid_data *edid);

#endif
