# Grafik
Small graphcis library using linux framebuffer. Also includes custom logging tool, .ini read/write tool and a input tool. Made for learing. (The grafik_input, grafik_log & grafik_config *should* both work separately in other projetcs.) 

This project uses no external libraries.

### Setup
```console
$ ./make.sh
```

### Setup Input
If input does not work, use the tool to setup the right input device. (finding the correct "/dev/input/eventX"). This should edit the grafik/config.ini file to the correct event device. For now you might have to do this after doing a reboot.
```console
$ ./make_input.sh
$ ./input_device_detector
```
