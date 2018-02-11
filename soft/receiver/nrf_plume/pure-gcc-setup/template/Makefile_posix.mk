#TERMINAL ?= gnome-terminal -e
TERMINAL ?= 

FLASH_START_ADDRESS = $(shell $(OBJDUMP) -h $(ELF) -j .text | grep .text | awk '{print $$4}')

ifdef SEGGER_SERIAL
JLINKEXE_OPTION = -SelectEmuBySn $(SEGGER_SERIAL)
JLINKGDBSERVER_OPTION = -select USB=$(SEGGER_SERIAL)
endif

MAKE_BUILD_FOLDER = mkdir -p $(OUTPUT_PATH)

JLINK = -JLinkExe $(JLINK_OPTIONS) $(JLINKEXE_OPTION)
JLINKGDBSERVER = JLinkGDBServer $(JLINK_OPTIONS) $(JLINKGDBSERVER_OPTION)

SOFTDEVICE_OUTPUT = $(OUTPUT_PATH)$(notdir $(SOFTDEVICE))

clean:
	rm -rf $(OUTPUT_PATH)
	rm -f *.jlink
	rm -f JLink.log
	#rm -f .gdbinit
	rm -f *~

flash: all flash.jlink
	$(JLINK) flash.jlink

flash.jlink:
	printf "r\n\
loadbin $(BIN) $(FLASH_START_ADDRESS)\n\
verifybin $(BIN) $(FLASH_START_ADDRESS)\n\
r\ng\nexit\n" > flash.jlink

SOFTDEVICE = $(SDK_PATH)softdevice/s110_nrf51822_6.0.0_softdevice.hex
flash-softdevice: erase-all flash-softdevice.jlink
ifndef SOFTDEVICE
	$(error "You need to set the SOFTDEVICE command-line parameter to a path (without spaces) to the softdevice hex-file")
endif

# Convert from hex to binary. Split original hex in two to avoid huge (>250 MB) binary file with just 0s.
	$(OBJCOPY) -Iihex -Obinary --remove-section .sec3 $(SOFTDEVICE) $(SOFTDEVICE_OUTPUT:.hex=_mainpart.bin)
	$(OBJCOPY) -Iihex -Obinary --remove-section .sec1 --remove-section .sec2 $(SOFTDEVICE) $(SOFTDEVICE_OUTPUT:.hex=_uicr.bin)
	$(JLINK) flash-softdevice.jlink

flash-softdevice.jlink:
	# Write to NVMC to enable write. Write mainpart, write UICR. Assumes device is erased.
	printf "w4 4001e504 1\n\
loadbin \"$(SOFTDEVICE_OUTPUT:.hex=_mainpart.bin)\" 0\n\
loadbin \"$(SOFTDEVICE_OUTPUT:.hex=_uicr.bin)\" 0x10001000\n\
verifybin \"$(SOFTDEVICE_OUTPUT:.hex=_mainpart.bin)\" 0\n\
verifybin \"$(SOFTDEVICE_OUTPUT:.hex=_uicr.bin)\" 0x10001000\n\
r\ng\nexit\n" > flash-softdevice.jlink

recover: recover.jlink erase-all.jlink pin-reset.jlink
	$(JLINK) recover.jlink
	$(JLINK) erase-all.jlink
	$(JLINK) pin-reset.jlink

recover.jlink:
	printf "si 0\nt0\nsleep 1\ntck1\nsleep 1\nt1\nsleep 2\nt0\nsleep 2\nt1\nsleep 2\nt0\nsleep 2\nt1\nsleep 2\nt0\nsleep 2\nt1\nsleep 2\nt0\nsleep 2\nt1\nsleep 2\nt0\nsleep 2\nt1\nsleep 2\nt0\nsleep 2\nt1\nsleep 2\ntck0\nsleep 100\nsi 1\nr\nexit\n" > recover.jlink

pin-reset.jlink:
	printf "w4 40000544 1\nr\nexit\n" > pin-reset.jlink

pin-reset: pin-reset.jlink
	$(JLINK) pin-reset.jlink

reset: reset.jlink
	$(JLINK) reset.jlink

reset.jlink:
	printf "r\ng\nexit\n" > reset.jlink

erase-all: erase-all.jlink
	$(JLINK) erase-all.jlink

erase-all.jlink:
	# Write to NVMC to enable erase, do erase all, wait for completion. reset
	printf "w4 4001e504 2\nw4 4001e50c 1\nsleep 100\nr\nexit\n" > erase-all.jlink

startdebug: all
	$(shell ps aux | grep -ie JLink | awk '{print $$2}' | xargs kill)
	@sleep 0.2
	$(JLINKGDBSERVER) -port $(GDB_PORT_NUMBER) -vd

debug-gdbinit:
	@printf "\
	target remote localhost:$(GDB_PORT_NUMBER)\n\
	def rinit\n\
	mon reset\n\
	end\n\
	def drunk\n\
	make\n\
	load\n\
	rinit\n\
	end\n\
	define reset_context\n\
	set $$r0=(*(uint32_t*)($$sp))\n\
	set $$r1=(*(uint32_t*)($$sp+4))\n\
	set $$r2=(*(uint32_t*)($$sp+8))\n\
	set $$r3=(*(uint32_t*)($$sp+12))\n\
	set $$r12=(*(uint32_t*)($$sp+16))\n\
	set $$lr=(*(uint32_t*)($$sp+20))\n\
	set $$pc=(*(uint32_t*)($$sp+24))\n\
	set $$sp = $$sp+28\n\
	end\n\
	" > .gdbinit

.PHONY: flash flash-softdevice erase-all startdebug
