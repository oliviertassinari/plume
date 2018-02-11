#################################################
# Fichier de configuration de gdb
#################################################

#target extended-remote localhost:3333
target extended-remote localhost:2331
monitor device stm32f427zg
monitor speed 1000
monitor reset

# Reset the target and call its init script
define rinit
  mon reset
end

define flash_run
  monitor reset
  load
  monitor reset
  echo Continue...\n
  cont
end

define reset_context
  set $r0=(*(uint32_t*)($sp))
  set $r1=(*(uint32_t*)($sp+4))
  set $r2=(*(uint32_t*)($sp+8))
  set $r3=(*(uint32_t*)($sp+12))
  set $r12=(*(uint32_t*)($sp+16))
  set $lr=(*(uint32_t*)($sp+20))
  set $pc=(*(uint32_t*)($sp+24))

  set $sp = $sp+28

end
