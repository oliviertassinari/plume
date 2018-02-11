
# Nrf Plume

To build the soft of the nrf:

    make
	
If you did not flash the softdevice before, run :
    
	make flash-softdevice
	
To flash the soft, run

	make flash
	
If you want to debug, in a separate terminal, run

    make startdebug
	
And in your  terminal, run 

	arm-none-eabi-gdb _build/<file>.elf
	
In gdb you can use

	load
	rinit
	continue

