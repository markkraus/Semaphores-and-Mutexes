#include <pi/pi.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __APPLE__
    #define generate_random(randState, variable_addr) *(variable_addr) = erand48(randState)
    #define seed_random(seed, randState) srand48(seed)
    #define rand_state_t(name) unsigned short (name)[3]
#else
    #define generate_random(randState, variable_addr) drand48_r(randState, variable_addr)
    #define seed_random(seed, randState) srand48_r(seed, randState)
    #define rand_state_t(name) struct drand48_data (name)
#endif

// Shared variable! We need to protect it
size_t count_hits = 0;
pthread_mutex_t mutex;

/* Implementation */

/** This function generates random 2D coordinates in
       a 2x2 square, and counts the number of coordinate
       pairs that fall within a circle of radius 1 centered
       with the square
*/
void *calculate_pi(void *data) {

    calculate_pi_t *args = data;
    /**
        Generating random numbers
    */

    size_t private_count = 0;

    rand_state_t(randState);
   
    seed_random(449 , &randState); 

    for (size_t i = args->start ; i < args->end; i++ ) {

        double x;
        generate_random(&randState, &x);        // Returns a pseudo-random double between 0 .. 1.
        x *= 2;                                 // Make it between 0 .. +2
        x -= 1;                                 // Make it between -1 .. +1

        double y;
        generate_random(&randState, &y);        // Returns a pseudo-random double between 0 .. 1.
        y = 2*y-1;                              // Make it between -1 .. +1

        if ( x*x + y*y <= 1.0*1.0 ) {           // If it's within a circle of radius 1
            // Count hits, this is a private variable, no need to lock the mutex

            private_count++;
        }

    }

    // Add the private count to the shared count, lock the mutex first
    pthread_mutex_lock(&mutex);

    count_hits += private_count;

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    return NULL;
}
