/* 
	"idd" for short
*/
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

void *event_thread(void *arg) {
	struct thread_args *args = (struct thread_args *)arg;
	int event_device_number = args->event_device_number;
	
	int fd = open("/dev/input/eventX", O_RDONLY);
	if (fd == -1) {

	}
	
	while (1) {
			
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


	char input_char;
	printf("Enter a charcter: ");
	fflush(stdout);
	scanf("%c", &input_char);
	printf("input_char=%c\n", input_char);
	
	for (int i = -1; i < devices; i++) {
		struct thread_args args;
		args.event_device_number = i;
		pthread_t thread;
		if (pthread_create(&thread, NULL, event_thread, (void *)&args) != 0) {
			perror("error: failed to create thread");
			return EXIT_FAILURE;
		}
	}
	
	while (1) {

	}

	return 0;
}






