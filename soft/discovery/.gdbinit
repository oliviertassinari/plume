#################################################
# Fichier de configuration de gdb
#################################################

target extended-remote localhost:3333
#target extended-remote localhost:2331
#monitor device stm32f407
#monitor speed 1000
#monitor reset

# Reset the target and call its init script
define flash_run
  monitor reset init
  load
  monitor reset init
  echo Continue...\n
  cont
