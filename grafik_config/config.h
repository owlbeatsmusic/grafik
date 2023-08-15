#ifndef CONFIG_H_
#define CONFIG_H_

char *grafik_config_string_from_key(char *key);
void grafik_config_add_string(char *key, char *value);
void grafik_configset_string(char *key, char *value);

#endif
