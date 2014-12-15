#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS    20
#define NUM_INCREMENTS 1000000

int counter;
CRITICAL_SECTION crit_sec;

unsigned long WINAPI AddThings(void *param)
{
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        EnterCriticalSection(&crit_sec);
        counter += 1;
        LeaveCriticalSection(&crit_sec);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    HANDLE threads[NUM_THREADS];

    InitializeCriticalSection(&crit_sec);

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = CreateThread(NULL, 0, AddThings, NULL, 0, NULL);
        if (!threads[i]) {
            printf("ERROR; CreateThread() failed\n");
            exit(1);
        }
    }

    // Wait for threads to finish
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
    printf("Final value of counter is: %d\n", counter);

    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
    }
    DeleteCriticalSection(&crit_sec);

    return 0;
}
