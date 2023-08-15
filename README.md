# grafik
Small graphcis library using linux framebuffer. Made for learing. (The grafik_input and grafik_log *should* both work separately in other projetcs. 

### setup grafik
```console
$ ./make.sh
```

### setup input
if input does not work, use the tool to setup the right input device. (finding the correct "/dev/input/eventX")
```console
$ ./make_input.sh
$ ./input_device_detector
```
