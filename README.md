# High Performance Timer
A generic timer for seconds level (currently only for Windows, It will support the linux in the future)


1. The number of this timer can be configured in the Timer_config.h file, the key word is TIMER_NUMBER;
2. The timer use the usleep as the timer cnt. the max offset of the timer will not over 10ms.
3. The way of timer Queue's query can be optimized, currently is the erodicing all the queue till find the corresponding element. 
   I think the querying algorithm can use more efficiently way, for example the dichotomy.
4. The Timer is used for single process, because of the IPC will cost some time if we set the timer for a generic service.

How to use this timer?

1. The sequence of the function using:
	1>. Main_Timer_Init(): Initial the Timer queue and create the thread and mutex, it can be invoked at the beginning of the system start.
	2>. Timer_Register(G_Timer *T): If you want to use a timer, you need register it first, so this function provide the register service. 
	    How to feed the timer, you can check the demo test code.
	3>. Timer_Start(G_Timer T): This function will start the timer you want.
	4>. Timer_Checker(G_Timer T): if you want to know the running status of the timer, you can use this function.

2. The Callback for every timer.
	This design provide a callback function for every timer, if the timer elapsed, the callback function will be triggerred.
	So users can define the call back, and register it, if you need a callback.
	