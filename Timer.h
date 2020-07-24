#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#ifdef __linux__
#include <pthread.h>
#endif
#ifdef __WIN32__
#include <process.h>
#include <windows.h>
#endif
#include "Timer_config.h"

typedef struct {
    char * m_Timer_Name;
    int m_Timer_Long;
    void * m_func;
}G_Timer;


typedef enum{
    Timer_Inactive = 0,
    Timer_Active = 1,
    Timer_Lapsed = 2,
    Timer_Max
}Timer_Runing_Status;

typedef struct {
    char * m_Timer_Name;
    int m_Timer;
    bool m_Timer_Start_Flag;
    int m_Timer_Long;
    Timer_Runing_Status m_Timer_State;
    void (*func)();
}Internal_Timer;

typedef enum{
    Register_Successfully = 0,
    Register_fail = 1,
    Register_Max
}Timer_Rregister_Status;

Timer_Rregister_Status Timer_Register(G_Timer *T);
bool Timer_Start(G_Timer T);
Timer_Runing_Status Timer_Checker(G_Timer T);
bool Main_Timer_Init();
#endif // _TIMER_H_
