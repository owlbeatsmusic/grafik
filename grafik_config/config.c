#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define KEY_VALUE_SETS_SIZE 5
#define KEY_VALUE_SET_BUFFER_SIZE 128

typedef struct {
	char key_buffer[KEY_VALUE_SET_BUFFER_SIZE];
	char value_buffer[KEY_VALUE_SET_BUFFER_SIZE];
} KeyValueSet;

int grafik_config_parse_internal(char *file_content, int file_length, KeyValueSet key_value_sets[]) {
	char temp_buf[128];
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
}

//void grafik_config_add_string(char *file_path, char *key, char *value) {}

int grafik_config_get_string(char *file_path, char *key, char *destination_buffer, int buffer_size) {
	if (buffer_size != KEY_VALUE_SET_BUFFER_SIZE) {
		printf("error: destination_buffer needs to be %d\n", KEY_VALUE_SET_BUFFER_SIZE);
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

	KeyValueSet key_value_sets[KEY_VALUE_SETS_SIZE];
	
	grafik_config_parse_internal(file_content, file_length, key_value_sets);

	// print the key_value_sets array
	for (int i = 0; i < KEY_VALUE_SETS_SIZE; i++) {
		printf("%d:\n   key  : %s\n   value: %s\n", i, key_value_sets[i].key_buffer, key_value_sets[i].value_buffer);
	}
	
	// find matching key and set destination buffer
	for (int i = 0; i < KEY_VALUE_SETS_SIZE; i++) {
		if (strcmp(key_value_sets[i].key_buffer, key) != 0) continue;
		for (int j = 0; j < KEY_VALUE_SET_BUFFER_SIZE; j++) {
			destination_buffer[j] = key_value_sets[i].value_buffer[j];
		}
		break;
	}

	/*; 

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

	*/

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
	
	KeyValueSet key_value_sets[KEY_VALUE_SETS_SIZE];
	
	/*
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

	*/

	grafik_config_parse_internal(file_content, file_length, key_value_sets);

	// print the key_value_sets array
	for (int i = 0; i < KEY_VALUE_SETS_SIZE; i++) {
		printf("%d:\n   key  : %s\n   value: %s\n", i, key_value_sets[i].key_buffer, key_value_sets[i].value_buffer);
	}

	free(file_content);
	fclose(file);
	
	return 0;
}
