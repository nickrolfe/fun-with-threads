# Fun with Threads

This is a Windows port of the code from Julia Evans's wonderful post about
[diving into concurrency](http://jvns.ca/blog/2014/12/14/fun-with-threads/).
All the functionality that was provided by pthreads in the original code is
supported directly in the Windows kernel, so these programs have no other
dependencies besides the C runtime.

## Build Instructions

* MingW: run `make`
* Visual Studio: run the VS2013 x86 Native Tools Command Prompt (or similar)
  from the Start Menu, then run `build_msvc.bat`.

## Porting Notes

### counter\_race.c

`pthread_create` has been replaced with `CreateThread`, and the multiple calls
to `pthread_join` have been replaced with a single call to
`WaitForMultipleObjects`.  Just like with pthreads, the final value is way
off, and it runs pretty quickly.

    $ time ./counter_race.exe
    Final value of counter is: 5588731

    real    0m0.172s
    user    0m0.000s
    sys     0m0.062s

### counter\_with\_mutex.c

Here we naively use `CreateMutex`, and replace the
`pthread_mutex_{lock,unlock}` calls with `WaitForSingleObject` and
`ReleaseMutex`.  The original pthreads code was pretty lethargic, but this is
absurdly slow!

    $ time ./counter_with_mutex.exe
    Final value of counter is: 20000000

    real    0m37.225s
    user    0m0.031s
    sys     0m0.015s

### counter\_with\_critical_sections.c

It turns out Win32 mutexes are overkill for our purposes -- for one thing,
[they can be shared across
processes](http://www.slideshare.net/abufayez/pthreads-vs-win32-threads) --
but critical sections will provide us with the same synchronization and much
lower overheads. We call `{Enter,Leave}CriticalSection` instead. This is way
faster than using either Win32 or pthread mutexes.

    $ time ./counter_with_critical_sections.exe
    Final value of counter is: 20000000

    real    0m0.858s
    user    0m0.015s
    sys     0m0.031s

### counter\_with\_atomics.c

Julia's version used the GCC atomic builtin `__sync_add_and_fetch`, and we can
use that with MinGW, since it's just GCC.

    $ time ./counter_with_atomics.exe
    Final value of counter is: 20000000

    real    0m0.577s
    user    0m0.031s
    sys     0m0.031s

Win32 has a similar API called
[`InterlockedIncrement`](http://msdn.microsoft.com/en-us/library/windows/desktop/ms683614%28v=vs.85%29.aspx).
Visual Studio replaces this with a `lock xadd` instruction -- pretty similar
to the GCC builtin. If we tried to use this with MinGW, it wouldn't be so
clever, and would simply make a library call.
