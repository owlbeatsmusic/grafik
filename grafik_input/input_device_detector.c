/* 
	"idd" for short
*/
#include <linux/input.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>

struct thread_args {
	int event_device_number;
};

int current_event_device_number = -1;

void *event_thread(void *arg) {
	struct thread_args *args = (struct thread_args *)arg;
	int event_device_number = args->event_device_number;
	
	char *prefix = "/dev/input/event";
	// 10 is the largest length of a integer in charcters
	char device_number_string[10];
	sprintf(device_number_string, "%d", event_device_number);

	char event_device_path[sizeof(prefix) + sizeof(device_number_string)];

	strcat(event_device_path, prefix);
	strcat(event_device_path, device_number_string);

	int fd = open(event_device_path, O_RDONLY);
	if (fd == -1) {
		perror("error openeing input device");
		return 0;
	
	}
	
	struct input_event ev;
	while (current_event_device_number == -1) {
		//printf("loop: %s\n", event_device_path);
		ssize_t n = read(fd, &ev, sizeof(struct input_event));
		if (n == (ssize_t)-1) {
			perror("error reading input");
			close(fd);
			return 0;
		}

		if (ev.type == EV_KEY) {
			current_event_device_number = event_device_number;
			return 0;
		}
		
	}

}

int main() {

	// first get how many event devices exsist
	
	int devices = 0;
	
	DIR *dir = opendir("/dev/input");
	if (dir == NULL) {
		perror("error opening directory(/dev/input");
		return EXIT_FAILURE;
	}
	const char *prefix = "event";
	struct dirent *entry;
	
	while ((entry = readdir(dir)) != NULL) {
		if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
			devices++;
		}
	}
	closedir(dir);

	printf("devices=%d\n", devices);


	pthread_t threads[devices];
	for (int i = 0; i < devices; i++) {
		struct thread_args args;
		args.event_device_number = i-1;;
		if (pthread_create(&threads[i], NULL, event_thread, (void *)&args) != 0) {
			perror("error: failed to create thread");
			return EXIT_FAILURE;
		}
	}
	
	char input_char;
	printf("Enter a charcter: ");
	fflush(stdout);
	scanf("%c", &input_char);
	printf("input_char=%c\n", input_char);	
	fflush(stdout);
	printf("keyboard input device detected: \"/dev/input/event%d\"\n", current_event_device_number);

	for (int i = 0; i < devices; i++) {
		pthread_join(threads[i], NULL);
		printf("thread: %d joined.\n", i);
	}

	return 0;
}






