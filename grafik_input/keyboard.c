#include "../grafik_config/config.h"
#include "../grafik_log/log.h"
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int grafik_input_keyboard_start() {
	
	int file_path_size = 64;
	char *file_path = (char *)malloc(file_path_size * sizeof(char));
	grafik_config_get_string("grafik_config/config.ini", "event_device", file_path, file_path_size);

	grafik_log_print("log.txt", "keyboard input started");

	int fd = open(file_path, O_RDONLY);
	if (fd == -1) {
		printf("error opening \"%s\" device\n", file_path);
		free(file_path);
		return -1;
	}

	free(file_path);

	struct input_event ev;

	while (1) {	
		ssize_t n = read(fd, &ev, sizeof(struct input_event));
		if (n == (ssize_t)-1) {
			perror("error reading event");
			close(fd);
			return -1;
		}

		if (ev.type == EV_KEY && ev.value == 1) {
			char *log_message = (char *) malloc(128 * sizeof(char));
			sprintf(log_message, "keyboard_input: keycode=%d", ev.code);
			grafik_log_print("log.txt", log_message);
			free(log_message);
		}
	}
}
