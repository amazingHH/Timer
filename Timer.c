#include "Timer.h"


static int g_Timer_Cnt = 0;
static int g_Timer_Index = 0;
HANDLE Timer_Mutex;

static Internal_Timer Timer_CFG_Table[TIMER_NUMBER] = {};
DWORD WINAPI Main_Timer_Start(void);
// static void Timer_Callback();

static bool Mutex_Init(void)
{
    bool ret = true;
    Timer_Mutex = CreateMutex(NULL,false,NULL);
    if (Timer_Mutex == NULL)
    {
        printf("Timer Mutex create failure \n");
        ret = false;
    }
    return ret;
}

static bool Mutex_lock(void)
{
    bool ret = true;
    DWORD temp = WaitForSingleObject(Timer_Mutex, INFINITE);
    // printf("temp = %d \n",temp);
    if (temp == WAIT_OBJECT_0)
        ret = true;
    else if (temp == WAIT_ABANDONED)
        ret = false;
    else if (temp == WAIT_FAILED )
        ret = false;
    else
        ret = false;
    return ret;
}

static bool Mutex_unlock(void)
{
    bool ret = true;
    int ret_value;
    ret_value = ReleaseMutex(Timer_Mutex);
    if (ret_value == 0)
        ret = false;
    return ret;
}

static void Array_Init(void)
{
    int i = 0;
    if (Mutex_lock() == false)
    {
        printf("Mutex Lock failure, fatal error \n");
        return;
    }
    for (i = 0; i < TIMER_NUMBER; i++)
    {
        Timer_CFG_Table[i].m_Timer = 0;
        Timer_CFG_Table[i].m_Timer_Long = 0;
        Timer_CFG_Table[i].m_Timer_Name = "";
        Timer_CFG_Table[i].m_Timer_Start_Flag = false;
        Timer_CFG_Table[i].m_Timer_State = Timer_Inactive;
        // Timer_CFG_Table[i].func = (void *)NULL;
    }
    if (Mutex_unlock() == false)
    {
        printf("Mutex Unlock failure, fatal error \n");
        return;
    }
}

Timer_Rregister_Status Timer_Register(G_Timer *T)
{
    Timer_Rregister_Status ret = Register_fail;
    if (g_Timer_Index >= TIMER_NUMBER)
    {
        printf("[Timer] :: Your array is full, register fail");
    } else {
        if (Mutex_lock() == false)
        {
            printf("Mutex Lock failure, fatal error \n");
            return;
        }
        Timer_CFG_Table[g_Timer_Index].m_Timer_Name = T->m_Timer_Name;
        Timer_CFG_Table[g_Timer_Index].m_Timer_Long = T->m_Timer_Long;
        (Timer_CFG_Table[g_Timer_Index].func) = (T->m_func);
        g_Timer_Index ++;
        ret =Register_Successfully;
        if (Mutex_unlock() == false)
        {
            printf("Mutex Unlock failure, fatal error \n");
            return;
        }
    }
    return ret;
}

bool Timer_Start(G_Timer T)
{
    uint_8 i = 0;
    bool ret = false;
    for (i = 0; i < g_Timer_Index; i++)
    {
        if(strcmp(Timer_CFG_Table[i].m_Timer_Name,T.m_Timer_Name) == 0)
        {
            if (Mutex_lock() == false)
            {
                printf("Mutex Lock failure, fatal error \n");
                return;
            }
            Timer_CFG_Table[i].m_Timer_Start_Flag = true;
            Timer_CFG_Table[i].m_Timer_State = Timer_Active;
            ret = true;
            if (Mutex_unlock() == false)
            {
                printf("Mutex Unlock failure, fatal error \n");
                return;
            }
        }
    }
    return ret;
}

Timer_Runing_Status Timer_Checker(G_Timer T)
{
    uint_8 i = 0;
    Timer_Runing_Status ret = Timer_Inactive;
    for (i = 0; i < g_Timer_Index; i++)
    {
        if(strcmp(Timer_CFG_Table[i].m_Timer_Name,T.m_Timer_Name) == 0)
        {
            ret = Timer_CFG_Table[i].m_Timer_State;
        }
    }
    return ret;
}

bool Main_Timer_Init()
{
#ifdef __linux__
    int ret = 0;
    pthread_t MainTimer;
    ret = pthread_create(&MainTimer,NULL,Main_Timer_Start,NULL);
    if(ret < 0)
    {
        printf("[TIMER]thread create failure \n");
        return false;
    }
    Array_Init();
#endif // __linux__
//#ifdef __WIN32__
    bool ret = true;
    HANDLE Timer_Thread_Handle;
    // Sys_Err_Type ret = NO_ERR;
	//Core_Buffer_Init();
	//Feed_Core_Buffer(JAGUAR);
    Timer_Thread_Handle = CreateThread(NULL, 0, Main_Timer_Start,NULL,0,NULL);
    if (Timer_Thread_Handle == 0)
    {
        printf("Create Speed Thread Error \n");
        ret = false;
        return ret;
    }
    Mutex_Init();
    Array_Init();
    return ret;
// #endif // __WIN32__
}

DWORD WINAPI Main_Timer_Start(void)
{
    while(1)
    {
        uint_8 i = 0;
        usleep(500000);
        for (i = 0; i < g_Timer_Index; i++)
        {
            if (Timer_CFG_Table[i].m_Timer_Start_Flag == true)
            {
                if (Mutex_lock() == false)
                {
                    printf("Mutex Lock failure, fatal error \n");
                    return;
                }
                Timer_CFG_Table[i].m_Timer ++;
                Timer_CFG_Table[i].m_Timer_State = Timer_Active;
                if ((Timer_CFG_Table[i].m_Timer/TIME_TRANSFER_CONSTANT) >= Timer_CFG_Table[i].m_Timer_Long)
                {
                    Timer_CFG_Table[i].m_Timer = 0;
                    Timer_CFG_Table[i].m_Timer_Start_Flag = false;
                    Timer_CFG_Table[i].m_Timer_State = Timer_Lapsed;
                    Timer_CFG_Table[i].func();
                    // Timer_CFG_Table[i].func();
                }
                if (Mutex_unlock() == false)
                {
                    printf("Mutex Unlock failure, fatal error \n");
                    return;
                }
            }
        }
    }
}
