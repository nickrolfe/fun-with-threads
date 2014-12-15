# MinGW makefile
all:
	gcc -std=c99 -o counter_race counter_race.c
	gcc -std=c99 -o counter_with_mutex counter_with_mutex.c
	gcc -std=c99 -o counter_with_critical_sections counter_with_critical_sections.c
	gcc -std=c99 -o counter_with_atomics counter_with_atomics.c
