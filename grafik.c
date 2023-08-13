#include <sys/ioctl.h>  // I/O control
#include <sys/mman.h>   // Memory mapping files into memory
#include <linux/fb.h>   // to use the framebuffer device
#include <stdlib.h>     // 
#include <stdint.h>     // for example: uint8_t
#include <unistd.h>     // unix system calls
#include <string.h>     //
#include <fcntl.h>      // file control
#include <stdio.h>      // 


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

	// Map framebuffer memory to user space 
	// (so that read and write)
	size_t fb_data_size = vinfo.yres_virtual *vinfo.xres_virtual * vinfo.bits_per_pixel / 8;
	uint8_t *fb_data = (uint8_t *)mmap(NULL, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t)0);	
	if (fb_data == MAP_FAILED) {
		perror("error mapping framebuffer to memory");
		close(fb_fd);
		return EXIT_FAILURE;
	}

	// clear screen from memory (set screen
	// to black)
	memset(fb_data, 0, fb_data_size);

	int x = 100;
	int y = 100;
	int offset = (y * vinfo.xres + x) * 4;

	fb_data[offset+3] = 0;   // (alpha) may not ebe needed
	fb_data[offset+2] = 255; // red
	fb_data[offset+1] = 255; // green
	fb_data[offset+0] = 255; // blue

	// unmap and close
	munmap(fb_data, fb_data_size);
	close(fb_fd);


}
