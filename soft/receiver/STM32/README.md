This program is used on our receivers.

## About

This directory contains the sources to flash the STM32 on the plume receiver board

## Compile

make :)

## Flash

We use a SWD probe and the JLinkGDBServer
   JLinkGDBServer -if SWD -vd
   arm-none-eabi-gdb build/plume_receiver.elf

We defined some macro : see .gdbinit for more information
