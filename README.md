# TM4C Web Server

## Included Make Recipes

1. dependencies: default, generate object files from all included external libraries in *lib*.

2. build: compile the source code specific to this project with all external libraries and generate an ELF (for debugging symbols) and a binary in *bin*.

3. flash: flash the most recently generated binary.

4. debug: open a gdb debug session, connect to the target device, and reset it.

5. clean: delete the *bin/* and *obj/* directories and call *make clean* in all subdirectory Makefiles.

## Dependencies

This project uses the arm-none-eabi toolchain for compilation and debugging. [OpenOCD](https://github.com/openocd-org/openocd.git) is necessary for debugging the remote target, and [lm4flash](https://github.com/utzig/lm4tools.git) is used to flash the board.