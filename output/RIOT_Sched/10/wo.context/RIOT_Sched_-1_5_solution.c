#include "thread.h"\n#include "sched.h"\n#include <stdio.h>\n\n#define THREAD1_STACKSIZE   THREAD_STACKSIZE_DEFAULT\n#define THREAD2_STACKSIZE   THREAD_STACKSIZE_DEFAULT\n#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)\n\nstatic char thread1_stack[THREAD1_STACKSIZE];\nstatic char thread2_stack[THREAD2_STACKSIZE];\n\nvoid *thread1_entry(void *arg) {\n    (void)arg;\n    while (1) {\n        printf("Thread 1 running\n");\n        thread_yield();\n    }\n    return NULL;\n}\n\nvoid *thread2_entry(void *arg) {\n    (void)arg;\n    while (1) {\n        printf("Thread 2 running\n");\n        thread_yield();\n    }\n    return NULL;\n}\n\nint main(void)\n{\n    kernel_pid_t thread1_pid, thread2_pid;\n\n    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),\n                                THREAD_PRIORITY, THREAD_CREATE_STACKTEST,\n                                thread1_entry, NULL, "thread1");\n\n    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),\n                                THREAD_PRIORITY, THREAD_CREATE_STACKTEST,\n                                thread2_entry, NULL, "thread2");\n\n    // Simulate runtime priority change\n    if (thread1_pid > 0) {\n        printf("Changing priority of Thread 1\n");\n        sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);\n    }\n\n    if (thread2_pid > 0) {\n        printf("Changing priority of Thread 2\n");\n        sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);\n    }\n\n    return 0;\n}