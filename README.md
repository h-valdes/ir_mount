# ir_mount

Micro-ros Node for the IR Mount using an ESP32 to control the IR-LEDs. It can
change the ID and state of the mount.

## Mount
Schematic representation of the IR mount (L-shape)
```
(34)=(35)=(32)=(33)=(25)
                    ||
                    ||
                    (26)
```

LEDs 34, 25 26 are always on and set as reference. LEDs 35, 32 and 33 are 
reserved for the data. 
```
(1)=(data)=(data)=(data)=(1)
                        ||
                        ||
                        (1)
```

Using Triple Repetition Code for the ID Encoding with a Hamming Distance
of 3
```
ID 0 = 000 
ID 1 = 111
```

# Image
![](/docs/imgs/esp32.jpg)
