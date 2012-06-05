/* 
 * File:   vesa.h
 * Author: caos
 *
 * Created on June 4, 2012, 8:08 AM
 */

#ifndef VESA_H
#define	VESA_H

typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;

typedef struct
{
    char signature[4];
    uint16_t version;
    uint32_t oem_string_ptr;
    uint32_t capabilities;
    uint32_t video_mode_ptr;
    uint16_t total_memory;

    uint32_t oem_software_rev;
    char *oem_vendor_name;
    char *oem_product_name;
    char *oem_product_rev;
} vesa_info_t;

typedef struct
{
    uint16_t mode_attributes;
    uint8_t wina_attributes;
    uint8_t winb_attributes;
    uint16_t win_granularity;
    uint16_t win_size;
    uint16_t wina_segment;
    uint16_t winb_segment;
    uint32_t win_pos_func_ptr;
    uint16_t bytes_per_scan_line;
    uint16_t width;
    uint16_t height;
    uint8_t char_width;
    uint8_t char_height;
    uint8_t num_planes;
    uint8_t bits_per_pixel;
    uint8_t num_banks;
    uint8_t memory_model_type;
    uint8_t bank_size;
    uint8_t num_image_pages;
    uint8_t reserved1;
    uint8_t red_mask_size;
    uint8_t red_field_position;
    uint8_t green_mask_size;
    uint8_t green_field_position;
    uint8_t blue_mask_size;
    uint8_t blue_field_position;
    uint8_t reserved_mask_size;
    uint8_t reserved_mask_position;
    uint8_t direct_color_mode_info;
    uint32_t phys_base_ptr;
    uint32_t offscreen_mem_ptr;
    uint16_t offscreen_mem_size;
    uint16_t lin_bytes_per_scan_line;
    uint8_t bnk_num_image_pages;
    uint8_t lin_num_image_pages;
    uint8_t lin_red_mask_size;
    uint8_t lin_red_field_position;
    uint8_t lin_green_mask_size;
    uint8_t lin_green_field_position;
    uint8_t lin_blue_mask_size;
    uint8_t lin_blue_field_position;
    uint8_t lin_rsvd_mask_size;
    uint8_t lin_rsvd_field_position;
    uint32_t max_pixel_clock;
    uint8_t reserved4[190];
} vesa_mode_info_t;


void init_vesa(vesa_mode_info_t* mode_info);
void load_font(void* font);
void put_pixel(int x, int y, unsigned int color);
void draw_char(int x, int y, unsigned int fg, unsigned int bg, unsigned char ch);

#endif	/* VESA_H */

