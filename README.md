# TM4C Web Server

A simple HTTP web server that runs on a TM4C. A very basic custom network stack (see dependencies) implements all *necessary* TCP/IP functionality. That is to say, some of the functionality that any real stack would normally support (e.g., IP packet fragmentation, TCP segment reordering) is not yet implemented.

## Included Make Recipes

1. dependencies: default, generate object files from all included external libraries in *lib*.

2. build: compile the source code specific to this project with all external libraries and generate an ELF (for debugging symbols) and a binary in *bin*.

3. flash: flash the most recently generated binary.

4. debug: open a gdb debug session, connect to the target device, and reset it.

5. clean: delete the *bin/* and *obj/* directories and call *make clean* in all subdirectory Makefiles.

## Dependencies

Two Git submodules are included as external libraries for this project:

- [An incomplete TM4C HAL](https://github.com/davidday99/tm4c-hal)
- [A toy network stack](https://github.com/davidday99/network-stack)

This project uses the arm-none-eabi toolchain for compilation and debugging. [OpenOCD](https://github.com/openocd-org/openocd.git) is necessary for debugging the remote target, and [lm4flash](https://github.com/utzig/lm4tools.git) is used for programming the chip.