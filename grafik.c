#include "grafik_input/keyboard.h" // custom keyboard input
#include "grafik_config/config.h"  // .ini config tool
#include "grafik_input/mouse.h"    // cutsom mouse input
#include "grafik_log/log.h"        // custom logging
#include <sys/ioctl.h>             // I/O control
#include <sys/mman.h>              // Memory mapping files into memory
#include <linux/fb.h>              // to use the framebuffer device
#include <termios.h>               // disable text input
#include <pthread.h>	           // multithreading for input
#include <stdlib.h>                // 
#include <stdint.h>                // for example: uint8_t
#include <unistd.h>                // unix system calls
#include <string.h>                //
#include <fcntl.h>                 // file control
#include <stdio.h>                 // 
#include <time.h>                  // for log timestamps
#include <math.h>                  //

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color;

typedef struct {
	float x;
	float y;
	float z;
} Point3D;

typedef struct {
	float x;
	float y;
	float z;
	float pitch;
	float yaw;
	float roll;
} Camera;

uint8_t *fb_data;
int fb_width;
int fb_height;

void grafik_draw_pixel(int x, int y, Color color) {
	int offset = (y * (fb_width+ 10) + x) * 4;

	fb_data[offset+0] = color.b;
	fb_data[offset+1] = color.g; 
	fb_data[offset+2] = color.r; 
	fb_data[offset+3] = color.a;  
}

int grafik_fill_rect(int x, int y, int width, int height, Color color) {
	for (int local_y = 0; local_y < height; local_y++) {
		for (int local_x = 0; local_x < width; local_x++) {
			grafik_draw_pixel(x+local_x, y+local_y, color);
		}
	}
}

int grafik_draw_line(int x1, int y1, int x2, int y2, int width, Color color) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	float k = (float)dy / (float)dx;
	int m = y1 - k * x1;
	
	int temp_x = x1;
	int temp_y = y1;
	int is_not_complete = 1;
	
	while (is_not_complete) {
		//printf("draw_line pixel at : (%d, %d)\n", temp_x, temp_y);
		grafik_draw_pixel(temp_x, temp_y, color);
		grafik_draw_pixel(temp_x, temp_y+1, color);
		grafik_draw_pixel(temp_x, temp_y+2, color);
		
		if (k < 1) temp_x++;
		else temp_x--;
	
		temp_y = k * temp_x + m;
		
		if (k < 1) is_not_complete = temp_x < x2;
		else is_not_complete = temp_x > x2;
	}

}

// rotate yaw (camera: left / right)
void rotateY(Point3D *point, float angle) {
	float cosA = cos(angle);
	float sinA = sin(angle);
	float x = point->x;
	float z = point->z;

	point->x = x * cosA - z * sinA;
	point->z = x * sinA - z * cosA;
}

void translate(Point3D *point, float dx, float dy, float dz) {
	point->x += dx;
	point->y += dy;
	point->z += dz;
}


void *keyboard_input_thread(void *arg) {
	grafik_input_keyboard_start();	
}
void *mouse_input_thread(void *arg) {
	grafik_input_mouse_start();
}

int main() {

	printf("\033[%d;%dH", 1000, 0);

	// disable typing text
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~ICANON; 
	t.c_lflag &= ~ECHO;  
	tcsetattr(STDIN_FILENO, TCSANOW, &t);	

	//printf("event_device: %s\n", grafik_config_get_string("grafik_config/config.ini", "mouse_device"));

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
	
	// start input
	grafik_log_create("log.txt");
	
	pthread_t keyboard_thread, mouse_thread;
	if (pthread_create(&keyboard_thread, NULL, keyboard_input_thread, NULL)) {
		perror("error: failed to create keyboard thread");
		munmap(fb_data, fb_data_size);
		close(fb_fd);
		return EXIT_FAILURE;
	}
	/*
	if (pthread_create(&mouse_thread, NULL, mouse_input_thread, NULL)) {
		perror("error: failed to create mouse thread");
		munmap(fb_data, fb_data_size);
		close(fb_fd);
		return EXIT_FAILURE;
	}
	*/

	Point3D points[] = {
		{1.0, 0.0, -2.0},
		{0.0, 1.0, -2.0},
		{-1.0, 0.0, -2.0},
		{0.0, -1.0, -2.0}
	};

	Camera camera = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	/*
	float angle = M_PI / 4.0;
	float translationX = 2.0;
	float translationY = 0.0;
	float translationZ = 0.0;
	*/

	Color color_red  = {255, 0, 0, 0};
	Color color_green  = {0, 255, 0, 0};
	Color color_blue  = {0, 0, 255, 0};
	Color color_white  = {255, 255, 255, 0};
	//grafik_draw_pixel(0, 0, color_red);


	for (int i = 0; i < 300; i += 10) {
		memset(fb_data, 0, fb_data_size);

		// TODO: draw points to screen	

		//grafik_fill_rect(150+i, 150, 100, 100, color_red);
		//grafik_draw_line(100, 50, 300+(i*2), 60+(i*3), 1, color_white);
		usleep(1);
	}
	
	

	// unmap and close
	
	pthread_cancel(keyboard_thread);
	grafik_log_print("log.txt", "keyboard input stopped");
	//pthread_cancel(mouse_thread);
	//grafik_log_print("log.txt", "mouse input stopped");
	
	munmap(fb_data, fb_data_size);
	close(fb_fd);

}
