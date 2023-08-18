#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
	char key_buffer[255];
	char value_buffer[255];
} KeyValueSet;

//void grafik_config_add_string(char *file_path, char *key, char *value) {}

int grafik_config_get_string(char *file_path, char *key, char *destination_buffer, int buffer_size) {
	char line_buf[60];
	if (buffer_size < (int)sizeof(line_buf)) {
		printf("error: destination_buffer too small, need to be over %ld\n", sizeof(line_buf) / sizeof(char));
		return -1;
	}
	FILE *file = fopen(file_path, "r"); 
	if (file == NULL) {
		printf("error: failed to open config file(%s)\n", file_path);
		return -1;
	}

	fseek(file, 0L, SEEK_END);
	int file_length = ftell(file);
	fseek(file, 0L, SEEK_SET);
	
	char *file_content = (char *)malloc(file_length * sizeof(char) + 1);
	if (file_content == NULL) {
		perror("error: Memory allocation failed");
		fclose(file);
		return -1;
	}
	
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
		printf("error: failed to open config file(%s)\n", file_path);
		return -1;
	}
	
	fseek(file, 0L, SEEK_END);
	int file_length = ftell(file);
	fseek(file, 0L, SEEK_SET);
	
	char *file_content = (char *)malloc(file_length * sizeof(char) + 1);
	if (file_content == NULL) {
		perror("error: Memory allocation failed");
		fclose(file);
		return -1;
	}
	
	fread(file_content, file_length, 1, file);
	file_content[file_length] = '\0';
	
	// 64 = number of sets in config.ini file
	int key_value_sets_size = 5;
	KeyValueSet key_value_sets[key_value_sets_size];
	
	char temp_buf[255];
	memset(temp_buf, 0, sizeof(temp_buf)); // clear temp_buf
	int temp_buf_index = 0;
	int key_value_sets_index = 0;
	for (int i = 0; i < file_length+1; i++) {
		if (file_content[i] == '=') {
			strcpy(key_value_sets[key_value_sets_index].key_buffer, temp_buf);
		}
		if (file_content[i] == '\n') {
			strcpy(key_value_sets[key_value_sets_index].value_buffer, temp_buf);
			key_value_sets_index++;
		}
		if (file_content[i] == '=' || file_content[i] == '\n') {
			memset(temp_buf, 0, sizeof(temp_buf)); // clear temp_buf
			temp_buf_index = 0;
			i++;
		}
		temp_buf[temp_buf_index] = file_content[i];
		temp_buf_index++;
	}

	// print the key_value_sets array
	for (int i = 0; i < key_value_sets_size; i++) {
		printf("%d:\n   key  : %s\n   value: %s\n", i, key_value_sets[i].key_buffer, key_value_sets[i].value_buffer);
	}

	free(file_content);
	fclose(file);
	
	return 0;
}
