rem First, run vcvarsall.bat, or start the VS2013 x86 Native Tools Command
rem Prompt from the Start Menu, so the x86 compiler's on your path.
cl counter_race.c /link /out:counter_race_msvc.exe
cl counter_with_mutex.c /link /out:counter_with_mutex_msvc.exe
cl counter_with_critical_sections.c /link /out:counter_with_critical_sections_msvc.exe
cl counter_with_atomics.c /link /out:counter_with_atomics_msvc.exe
