#include <stdio.h>
#include <stdlib.h>
#include "Timer_config.h"

DWORD dwLast;
DWORD dwCurrent;

void test(void)
{
    printf("10s timeout \n");
    dwCurrent = GetTickCount();
    printf("cost %d \n", (dwCurrent-dwLast));
}

void test1(void)
{
    printf("3s timeout \n");
}
void main(void)
{
    bool ret = true;
    G_Timer timer;
    G_Timer timer_S;
    timer.m_Timer_Long = 10;
    timer.m_Timer_Name = "32";
    timer.m_func = &test;
    timer_S.m_Timer_Long = 3;
    timer_S.m_Timer_Name = "31";
    timer_S.m_func = &test1;
    ret = Main_Timer_Init();
    Timer_Register(&timer);
    Timer_Register(&timer_S);
    Timer_Start(timer);
    Timer_Start(timer_S);
    dwLast = GetTickCount();

    while (1)
    {
        // printf("1 \n");
    }
}
