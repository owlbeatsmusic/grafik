#include "../grafik_log/log.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void grafik_input_keyboard_start() {
	while (1) {	
		grafik_log_print("log.txt", "keyboard input started\n");	
		sleep(1);
	}
}
