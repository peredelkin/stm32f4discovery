#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Here is a good place to include header files that are required across
your application. */
#include "stm32f4xx.h"

#define configDEBUG

// Использование вытеснения.
#define configUSE_PREEMPTION                    1
// Использование оптимизированного выбора задач.
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
// Использование останова системного счётчика. 
#define configUSE_TICKLESS_IDLE                 0
// Частота CPU.
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )
// Частота прерывания таймера RTOS. 
#define configTICK_RATE_HZ                      1000
// Максимальное число приоритетов.
#define configMAX_PRIORITIES                    16
// Минимальный размер стека в машинных словах.
#define configMINIMAL_STACK_SIZE                128
// Максимальная длина имени задачи.
#define configMAX_TASK_NAME_LEN                 16
// Использование 16 битного системного счётчика.
#define configUSE_16_BIT_TICKS                  0
// Задача idle должна отдавать процессорное время.
#define configIDLE_SHOULD_YIELD                 1
// Уведомления потоков.
#define configUSE_TASK_NOTIFICATIONS            1
// Использовать мьютексы.
#define configUSE_MUTEXES                       1
// Использовать рекурсивные мьютексы.
#define configUSE_RECURSIVE_MUTEXES             1
// Использовать счётные семафоры.
#define configUSE_COUNTING_SEMAPHORES           1

#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */
// Максимум зарегистрированных очередей.
#define configQUEUE_REGISTRY_SIZE               4
// Использовать наборы очередей.
#define configUSE_QUEUE_SETS                    1
// Чередовать задачи с равным приоритетом.
#define configUSE_TIME_SLICING                  1
// Использование структуры Newlib reent.
#define configUSE_NEWLIB_REENTRANT              0

#define configENABLE_BACKWARD_COMPATIBILITY     0
// Количество пользовательских указателей.
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0

/* Memory allocation related definitions. */
// Поддержка статического выделения памяти.
#define configSUPPORT_STATIC_ALLOCATION         1
// Поддержка динамического выделения памяти.
#define configSUPPORT_DYNAMIC_ALLOCATION        0
// Полный размер кучи.
#define configTOTAL_HEAP_SIZE                   4096
// Кучу выделяет приложение.
#define configAPPLICATION_ALLOCATED_HEAP        1

/* Hook function related definitions. */
// Передача управления задаче idle.
#define configUSE_IDLE_HOOK                     0
// Тик системного счётчика.
#define configUSE_TICK_HOOK                     0
// Проверка переполнения стека.
#ifdef configDEBUG
#define configCHECK_FOR_STACK_OVERFLOW          2
#else
#define configCHECK_FOR_STACK_OVERFLOW          0
#endif
// Ошибка при выделении памяти.
#define configUSE_MALLOC_FAILED_HOOK            0
// Первый запуск задачи управления таймерами.
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0


/* Run time and task stats gathering related definitions. */
// Сбор статистики времени выполнения.
#define configGENERATE_RUN_TIME_STATS           0
// Функционал для трассировки и визуализации выполнения.
#define configUSE_TRACE_FACILITY                0
// Функции получения форматированной статистики.
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
// Использование сопрограмм.
#define configUSE_CO_ROUTINES                   0
// Максимальное число приоритетов сопрограмм.
#define configMAX_CO_ROUTINE_PRIORITIES         2
/* Software timer related definitions. */
// Использование таймеров.
#define configUSE_TIMERS                        1
// Приоритет задачи управления таймерами.
#define configTIMER_TASK_PRIORITY               5
// Длина очереди команд управления таймерами.
#define configTIMER_QUEUE_LENGTH                8
// Размер стека задачи управления таймерами.
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Define to trap errors during development. */
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
//#define configASSERT( ( x ) ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )
#ifdef configDEBUG
#define configASSERT( x ) if( ( x )== 0) { taskDISABLE_INTERRUPTS(); for( ;; ); }
#endif

/* FreeRTOS MPU specific definitions. */
//#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          0
#define INCLUDE_xTaskGetCurrentTaskHandle       0
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

/* Interrupt nesting behaviour configuration. */
// Приоритет прерывания ядра.
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
// Максимальный приоритет прерываний, использующих системные вызовы FreeRTOS.
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

//#define configMAX_API_CALL_INTERRUPT_PRIORITY   

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */
