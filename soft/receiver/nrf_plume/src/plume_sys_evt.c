#include "conf.h"

// Persistent storage system event handler
void pstorage_sys_event_handler (uint32_t p_evt);

void sys_evt_dispatch(uint32_t sys_evt) {
    pstorage_sys_event_handler(sys_evt);
}
