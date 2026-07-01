#include "timer.h"
#include "../screen/screen_services.h"
#include "../io/io.h"

timer_callback timer_callbacks[];
timer_callback* end = &timer_callbacks[0];
int system_ticks = 0;

void H_timer() {
    system_ticks++;

    for(int i = 0; i < CALLBACK_COUNT; i++) {
        timer_callback callback = timer_callbacks[i];
        if(system_ticks % callback.interval == 0) {
            callback.timer_callback_handler_ptr();
        }
    }

    outb(0x20, 0x20);
}

extern void add_timer_callback(void (*timer_callback_handler_function), int interval) {
    if(&timer_callbacks[0] - end > CALLBACK_COUNT) return;
    end->timer_callback_handler_ptr = timer_callback_handler_function;
    end->interval = interval;
}