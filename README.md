# TM4C Bare Metal Template
This repo contains boilerplate code for starting a new embedded C project. The current structure uses the traditional *inc/* and *src/* folders and includes a linkerscript for the TM4C123GXL microcontroller. A makefile is also included to build the entire project based on its current structure. As a project grows in complexity, you'll most likely need to make changes to it.

## Included Make Recipes

1. all: default, compiles all the files in *src/*, links them to any standard library functions that may be used and generates both an ELF with debugging symbols and a binary stripped of all symbols to flash to the board.

2. flash: flash the most recently generated binary.

3. debug: open a gdb debug session, connect to the target device, and reset it.

4. clean: delete the *bin/* and *obj/* directories.

## Dependencies

This project uses the arm-none-eabi toolchain for compilation and debugging. [OpenOCD](https://github.com/openocd-org/openocd.git) is necessary for debugging the remote target, and [lm4flash](https://github.com/utzig/lm4tools.git) is used to flash the board.