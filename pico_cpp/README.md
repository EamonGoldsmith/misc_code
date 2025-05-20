# NotaryFirmware
pico SDK firmware for the Notary

# Build Steps
- install dependancies
    - cmake
    - gcc-arm-none-eabi
    - pico sdk

```sh
$ git clone --recusive git@github.com:SwinRoverTeam/NotaryFirmware.git
$ cd NotaryFirmware
$ mkdir build
$ cd build
$ export PICO_SDK_PATH=[path to sdk]
$ cmake -DPICO_PLATFORM=rp2350-arm-s -DPICO_BOARD=pico2 ..
$ make
```

# Upload - using PICOTOOL

- check firmware
```sh
$ sudo picotool info notary.uf2 -a
```
Firstly check there is no warning about no blocking loop.
You should see some space taken in flash memory, the correct pinouts, chip and board.
There should be two metadata blocks at the bottom for the bootloader and program data.

- plug pico into computer while holding BOOTSEL pin
- copy firmware file to pico

```sh
$ sudo picotool load notary.uf2 --family rp2350-arm-s
```
- reboot pico
```sh
$ sudo picotool reboot
```

# Debugging

- To open terminal session

```sh
$ export TERM=vt100
$ ls /dev/tty*
$ screen /dev/tty.usbmodemXXX 115200
```

or
```sh
$ sudo screen /dev/ttyACM0 115200
```

- Alternatively check which port the pico is connected to using dmesg(1)

# TODO
- DONE - build and example project using pico_sdk
- DONE - import babel_fish as submodule
- DONE - create an instance of the babel_fish USB debug driver
- DONE - test some functionality like heartbeat and error
