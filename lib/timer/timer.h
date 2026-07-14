#ifndef TIMER_H
#define TIMER_H

#define TPS 100 // ticks per second
#define CALLBACK_MAX_COUNT 50

typedef struct {
    void (*timer_callback_handler_ptr)();
    int interval;
} timer_callback;


// TO-DO: make this into a list v
extern timer_callback timer_callbacks[CALLBACK_MAX_COUNT];

extern timer_callback* end;

// track number of ticks
extern int system_ticks;

extern int callback_count;

void H_timer();
extern void H_timer_stub();

extern void add_timer_callback(void (*timer_callback_handler_function), int interval);

#endif