#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     20
#define NUM_INCREMENTS  1000000

int counter;

unsigned long WINAPI AddThings(void *param)
{
    for (int i = 0; i < NUM_INCREMENTS; i++) {
#ifdef __MINGW32__
        // Use the GCC intrinsic, because MinGW is not clever enough to avoid
        // a library call for InterlockedIncrement.
        __sync_add_and_fetch(&counter, 1);
#else
        // MSVC uses a compiler-intrinsic here.
        InterlockedIncrement(&counter);
#endif
   }
   return 0;
}

int main (int argc, char *argv[])
{
    HANDLE threads[NUM_THREADS];

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

    return 0;
}
