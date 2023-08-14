#include <string.h>
#include <stdio.h>
#include <time.h>

int grafik_log_create(char *file_path) {
	FILE *log_file = fopen(file_path, "w");
	if (log_file == NULL) {
		perror("error: failed to create(/open) log file");
		return -1;
	}
	fclose(log_file);

	return 0;
}

int grafik_log_print(char *file_path, char *input) {
	FILE *log_file = fopen(file_path, "a");
	if (log_file == NULL) {
		perror("error: failed to open logfile");
		return -1;
	}

	// time string
	time_t current_time;
	struct tm *local_time;
	char formatted_time[33];
	
	current_time = time(NULL);
	local_time = localtime(&current_time);
	strftime(formatted_time, sizeof(formatted_time), "[%Y-%m-%d %H:%M:%S]: ", local_time);

	// combine final string
	char final_string[sizeof(formatted_time) + sizeof(input) + 1];
	strcat(final_string, formatted_time);
	strcat(final_string, input);
	strcat(final_string, "\n");


	if (fputs(final_string, log_file) == EOF) {
		perror("error while writing to log file");
		fclose(log_file);
		return -1;
	}
	fclose(log_file);
	
	return 0;
}
