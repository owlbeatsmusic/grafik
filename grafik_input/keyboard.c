#include "../grafik_log/log.h"
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int grafik_input_keyboard_start() {
	
	grafik_log_print("log.txt", "keyboard input started");

	int fd = open("/dev/input/event9", O_RDONLY);
	if (fd == -1) {
		perror("error opening dev/input0 device");
		return -1;
	}

	struct input_event ev;

	while (1) {	
		ssize_t n = read(fd, &ev, sizeof(struct input_event));
		if (n == (ssize_t)-1) {
			perror("error reading event (input0)");
			close(fd);
			return -1;
		}

		if (ev.type == EV_KEY && ev.value == 1) {
			char *log_message = (char *) malloc(20 * sizeof(char));
			sprintf(log_message, "keyboard_input: keycode=%d", ev.code);
			grafik_log_print("log.txt", log_message);
			free(log_message);
		}
	}
}
