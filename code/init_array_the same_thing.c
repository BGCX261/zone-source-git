#include <stdio.h>


/*
 * PID-map pages start out as NULL, they get allocated upon
 * first use and are never deallocated. This way a low pid_max
 * value does not cause lots of bitmaps to be allocated, but
 * the scheme scales to up to 4 million PIDs, runtime.
 */

#define ELE_NUM 10
typedef struct pidmap {
	int nr_free;
	void *page;
} pidmap_t;


static pidmap_t pidmap_array[ELE_NUM] =
	 { [ 0 ... ELE_NUM-1 ] = { 2, NULL } };

int
main(void)
{
    int i;

    for (i = 0; i < ELE_NUM; i++){
        printf("pidmap_array[%d] = %d\n", i, pidmap_array[i].nr_free);
    }
    
    return 1;
}
