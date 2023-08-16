#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//void grafik_config_add_string(char *file_path, char *key, char *value) {}

char *grafik_config_get_string(char *file_path, char *key) {
	FILE *file = fopen(file_path, "r"); 
	if (file == NULL) {
		printf("error: failed to open file(%s)\n", file_path);
		return NULL;
	}

	fseek(file, 0L, SEEK_END);
	int file_length = ftell(file);
	fseek(file, 0L, SEEK_SET);
	
	char *file_content = (char *)malloc(file_length * sizeof(char));
	
	// use exact same line when wanting to get next line
	// TODO: use while loop to create a string of entire file
	fgets(file_content, file_length, file);
	
	printf("file_content=\"%s\"\nkey=\"%s\"\n", file_content, key);
	
	char *result = strstr(file_content, key);
	
	int position = -1;
	if (result != NULL) {
		position = result - file_content;
	}

	printf("index=%d\n", position);

	free(file_content);
	fclose(file);			
}

int grafik_config_set_string(char *file_path, char *key, char *value) {	
	FILE *file = fopen(file_path, "rw"); 
	if (file == NULL) {
		printf("error: failed to open file(%s)\n", file_path);
		return -1;
	}



	fclose(file);
}
