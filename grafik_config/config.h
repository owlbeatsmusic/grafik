#ifndef CONFIG_H_
#define CONFIG_H_

int grafik_config_get_string(char *file_path, char *key, char *destination_buffer, int buffer_size);
//void grafik_config_add_string(char *key, char *value);
void grafik_configset_string(char *file_path, char *key, char *value);

#endif
