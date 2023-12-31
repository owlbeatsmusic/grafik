#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//void grafik_config_add_string(char *file_path, char *key, char *value) {}

int grafik_config_get_string(char *file_path, char *key, char *destination_buffer, int buffer_size) {
	char line_buf[60];
	if (buffer_size < (int)sizeof(line_buf)) {
		printf("error: destination_buffer too small, need to be over %ld\n", sizeof(line_buf) / sizeof(char));
		return -1;
	}
	FILE *file = fopen(file_path, "r"); 
	if (file == NULL) {
		printf("error: failed to open file(%s)\n", file_path);
		return -1;
	}

	fseek(file, 0L, SEEK_END);
	int file_length = ftell(file);
	fseek(file, 0L, SEEK_SET);
	
	char *file_content = (char *)malloc(file_length * sizeof(char) + 1);
	if (file_content == NULL) {
		perror("error: Memory allocation failed");
		return -1;
	}
	
	// use exact same line when wanting to get next line
	// TODO: use while loop to create a string of entire file
	fread(file_content, file_length, 1, file);
	file_content[file_length] = '\0';
	
	char *result = strstr(file_content, key);
	
	int position = -1;
	if (result != NULL) {
		position = result - file_content;
	}

	int index = 0;
	int j = 0;
	while (file_content[position+j] != '\n' && file_content[position+j] != '\0') {
		if (file_content[position+j] != key[j] && file_content[position+j] != '=') {
			line_buf[index] = file_content[position+j];
			index++;
		} 
		j++;
	}
	line_buf[index] = '\0';

	free(file_content);
	fclose(file);	

	for (int i = 0; i < sizeof(line_buf); i++) {
		destination_buffer[i] = line_buf[i];
	}

	return 0;
}

int grafik_config_set_string(char *file_path, char *key, char *value) {	
	FILE *file = fopen(file_path, "rw"); 
	if (file == NULL) {
		printf("error: failed to open file(%s)\n", file_path);
		return -1;
	}

	fclose(file);
}
