#include <sys/ioctl.h>  // I/O control
#include <sys/mman.h>   // Memory mapping files into memory
#include <linux/fb.h>   // to use the framebuffer device
#include <stdlib.h>     // 
#include <stdint.h>     // for example: uint8_t
#include <unistd.h>     // unix system calls
#include <string.h>     //
#include <fcntl.h>      // file control
#include <stdio.h>      // 

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color;

uint8_t *fb_data;
int fb_width;
int fb_height;

int x_transform_to_top_left(int x) {
	return x;
}

void grafik_draw_pixel(int x, int y, Color color) {
	//int offset = (y * fb_width + x + 19) * 4;
	int offset = (y * (fb_width+ 10) + x) * 4;

	fb_data[offset+0] = color.b;
	fb_data[offset+1] = color.g; 
	fb_data[offset+2] = color.r; 
	fb_data[offset+3] = color.a;  
}

void grafik_fill_rect(int x, int y, int width, int height, Color color) {
	for (int local_y = 0; local_y < height; local_y++) {
		for (int local_x = 0; local_x < width; local_x++) {
			grafik_draw_pixel(x+local_x, y+local_y, color);
		}
	}
}

int main() {
	
	// opening the framebuffer device (file descriptor) 
	// in read/write
	int fb_fd = open("/dev/fb0", O_RDWR);
	if (fb_fd == -1) {
		perror("error failed to open framebuffer device\n");
		return EXIT_FAILURE;
	}

	// getting the framebuffer information, reading
	// from /dev/fb0 to vinfo.
	struct fb_var_screeninfo vinfo;
	if(ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
		perror("error failed to read frambuffer variable information\n");
		close(fb_fd);
		return EXIT_FAILURE;
	}

	fb_width  = vinfo.xres_virtual;
	fb_height = vinfo.yres_virtual;


	// Map framebuffer memory to user space 
	// (so that read and write)
	size_t fb_data_size = fb_height * fb_width * vinfo.bits_per_pixel / 8;
	fb_data = (uint8_t *)mmap(NULL, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t)0);	
	if (fb_data == MAP_FAILED) {
		perror("error mapping framebuffer to memory");
		close(fb_fd);
		return EXIT_FAILURE;
	}

	// clear screen from memory (set screen
	// to black)
	memset(fb_data, 0, fb_data_size);
	
	Color color_red  = {255, 0, 0, 0};
	Color color_green  = {0, 255, 0, 0};
	Color color_blue  = {0, 0, 255, 0};
	Color color_white  = {255, 255, 255, 0};
	grafik_draw_pixel(0, 0, color_red);
	
	for (int i = 0; i < 300; i+= 1) {
		memset(fb_data, 0, fb_data_size);
		grafik_fill_rect(150+i, 150, 100, 100, color_red);
		usleep(100000);
		
	}
	// unmap and close
	munmap(fb_data, fb_data_size);
	close(fb_fd);


}
