#ifndef CONFIG_H_
#define CONFIG_H_

char *grafik_config_get_string(char *file_path, char *key);
//void grafik_config_add_string(char *key, char *value);
void grafik_configset_string(char *file_path, char *key, char *value);

#endif
