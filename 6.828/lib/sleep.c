#include <inc/lib.h>


void
sleep(int msec)
{
    unsigned now = sys_time_msec();
    unsigned end = now + msec;

    if ((int)now < 0 && (int)now > -MAXERROR)
        panic("sys_time_msec: %e", (int)now);
    if (end < now)
        panic("sleep: wrap");
        
    while (sys_time_msec() < end)
        sys_yield();
}