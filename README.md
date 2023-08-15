# Grafik
Small graphcis library using linux framebuffer. Made for learing. (The grafik_input, grafik_log & grafik_config *should* both work separately in other projetcs.) 

### Setup
```console
$ ./make.sh
```

### Setup Input
If input does not work, use the tool to setup the right input device. (finding the correct "/dev/input/eventX")
```console
$ ./make_input.sh
$ ./input_device_detector
```
