#include <videoDriver.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

uint8_t font_bitmap[4096] = {
    // Relleno para las primeras letras (Espacio, símbolos, etc.)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    

    // Font para la letra ' '
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    // Font para la letra '!'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    // Font para la letra ' " '
    0b00000000,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    // Font para la letra '#'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010010,
    0b00010010,
    0b00100100,
    0b01111110,
    0b00100100,
    0b01111110,
    0b00100100,
    0b01001000,
    0b01001000,
    0b00000000,
    0b00000000,
    // Font para la letra '$'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00111110,
    0b01011000,
    0b01011000,
    0b00111000,
    0b00011100,
    0b00011010,
    0b00011010,
    0b00011100,
    0b01111000,
    0b00011000,
    0b00000000,
    0b00000000,

    // Font para la letra '%'
    0b00000000,
    0b00000000,
    0b00110000,
    0b01001000,
    0b00101000,
    0b00110000,
    0b00010000,
    0b00010000,
    0b00110000,
    0b01001000,
    0b01000100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '&'
    0b00000000,
    0b00000000,
    0b00011000,
    0b00100100,
    0b00100100,
    0b00101000,
    0b00010000,
    0b00101000,
    0b01001010,
    0b01000110,
    0b00111010,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '''
    0b00000000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '('
    0b00000000,
    0b00000000,
    0b00001100,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00010000,
    0b00010000,
    0b00001100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra ')'
    0b00000000,
    0b00000000,
    0b00110000,
    0b00001000,
    0b00001000,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00110000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '*'
    0b00000000,
    0b00000000,
    0b00010000,
    0b01010100,
    0b00111000,
    0b00010000,
    0b00010000,
    0b00111000,
    0b01010100,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '+'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b01111110,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra ','
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00010000,
    0b00100000,

    // Font para la letra '-'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '.'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,

    // Font para la letra '/'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '0' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000110,
    0b01001010,
    0b01010010,
    0b01100010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '1' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00011000,
    0b00101000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '2' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b00000010,
    0b00000010,
    0b00001100,
    0b00110000,
    0b01000000,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '3' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b00000010,
    0b00011100,
    0b00000010,
    0b00000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '4' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00001100,
    0b00010100,
    0b00100100,
    0b01000100,
    0b01111110,
    0b00000100,
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '5' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b01000000,
    0b01000000,
    0b01111100,
    0b00000010,
    0b00000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '6' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '7' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000010,
    0b00000010,
    0b00000100,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '8' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para el número '9' estirado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra ':'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra ';'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00010000,
    0b00100000,
    0b00000000,
    0b00000000,

    // Font para la letra '<'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '='
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '>'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '?'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    // Font para la letra '@'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01011110,
    0b01010110,
    0b01010110,
    0b01011110,
    0b01000000,
    0b00111110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,


    // Font para la letra 'A'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'B'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'C'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'D'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'E'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111110,
    0b00000000,
    0b00000000,
    // Font para la letra 'F'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    // Font para la letra 'G'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b01000000,
    0b01000000,
    0b01000110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'H'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'I'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00111110,
    0b00000000,
    0b00000000,
    // Font para la letra 'K'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01000100,
    0b01001000,
    0b01110000,
    0b01001100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'L'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111110,
    0b00000000,
    0b00000000,
    // Font para la letra 'M'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01100110,
    0b01100110,
    0b01011010,
    0b01011010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'N'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01100010,
    0b01100010,
    0b01010010,
    0b01010010,
    0b01011010,
    0b01001010,
    0b01000110,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'O'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'P'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111100,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    // Font para la letra 'Q'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01001010,
    0b01000100,
    0b00111010,
    0b00000000,
    0b00000000,
    // Font para la letra 'P'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'S'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00111100,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'T'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00000000,
    // Font para la letra 'U'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
    // Font para la letra 'V' 
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00100100,
    0b00100100,
    0b00100100,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    // Font para la letra 'W'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01011010,
    0b01011010,
    0b01100110,
    0b01100110,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'X'
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b00100100,
    0b00100100,
    0b00011000,
    0b00011000,
    0b00100100,
    0b00100100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,
    // Font para la letra 'Y'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100010,
    0b00100010,
    0b00010100,
    0b00010100,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00000000,
    // Font para la letra 'Z' [REVISAR]
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000100,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b01000000,
    0b01111110,
    0b00000000,
    0b00000000,

    // Font para la letra 'a'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000000,
    0b00000000,

    // Font para la letra 'b'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111100,
    0b00000000,
    0b00000000,

    // Font para la letra 'c'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,

    // Font para la letra 'd'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000000,
    0b00000000,

    // Font para la letra 'e'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01111110,
    0b01000000,
    0b01000000,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,

    // Font para la letra 'f'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001110,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00111100,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00000000,

    // Font para la letra 'g'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000010,
    0b00000010,
    0b01111100,
    0b00000000,

    // Font para la letra 'h'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,

    // Font para la letra 'i'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00011100,
    0b00000000,
    0b00000000,

    // Font para la letra 'j'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b01001000,
    0b00110000,
    0b00000000,

    // Font para la letra 'k'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b01000010,
    0b01000100,
    0b01001000,
    0b01110000,
    0b01010000,
    0b01001000,
    0b01000100,
    0b01000010,
    0b00000000,
    0b00000000,

    // Font para la letra 'l'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00110000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00111000,
    0b00000000,
    0b00000000,

    // Font para la letra 'm'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b00101010,
    0b00101010,
    0b00101010,
    0b00101010,
    0b00100010,
    0b00100010,
    0b00000000,
    0b00000000,

    // Font para la letra 'n'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000,
    0b00000000,

    // Font para la letra 'o'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,

    // Font para la letra 'p'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01011100,
    0b01100010,
    0b01100010,
    0b01100010,
    0b01011100,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,

    // Font para la letra 'q'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111010,
    0b01000110,
    0b01000110,
    0b01000110,
    0b00111010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000000,

    // Font para la letra 'r'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01011100,
    0b01100010,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,

    // Font para la letra 's'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b01000000,
    0b01000000,
    0b00111100,
    0b00000010,
    0b00000010,
    0b01111100,
    0b00000000,
    0b00000000,

    // Font para la letra 't'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00100000,
    0b01111000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100100,
    0b00011000,
    0b00000000,
    0b00000000,

    // Font para la letra 'u'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000000,
    0b00000000,

    // Font para la letra 'v'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100010,
    0b00100010,
    0b00100010,
    0b00100010,
    0b00010100,
    0b00010100,
    0b00001000,
    0b00000000,
    0b00000000,

    // Font para la letra 'w'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100010,
    0b00100010,
    0b00100010,
    0b00101010,
    0b00101010,
    0b00110110,
    0b00010010,
    0b00000000,
    0b00000000,

    // Font para la letra 'x'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100010,
    0b00100010,
    0b00010100,
    0b00001000,
    0b00010100,
    0b00100010,
    0b00100010,
    0b00000000,
    0b00000000,

    // Font para la letra 'y'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000100,
    0b01000100,
    0b01000100,
    0b01000100,
    0b00111100,
    0b00000100,
    0b00000100,
    0b00111000,
    0b00000000,

    // Font para la letra 'z'
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111100,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b01111100,
    0b00000000,
    0b00000000,


    // Seguir con el resto de caracteres...
};


void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor) {
	int cx,cy;
	int mask[8]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	unsigned char *glyph=font_bitmap+(int)c*16;

	for(cy=0;cy<16;cy++){
		for(cx=0;cx<8;cx++){
			putPixel(glyph[cy] & mask[cx] ? fgcolor : bgcolor, x+cx, y+cy-12);
		}
	}
}