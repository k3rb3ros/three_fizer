#include "include/progressBar.h"

void printProgressBar(bool* running, int* error, progress_t* p)
{
    const char* action = p->encrypt ? "Encrypting" : "Decrypting";
    uint8_t count = 0;
    static struct winsize ws;
    const size_t term_width = ioctl(1, TIOCGWINSZ, &ws) >= 0 ? ws.ws_col : STD_TERM_WIDTH;
    const uint16_t bar_size = term_width - SUBTRACT_SIZE;
    double ratio = 0.0;
    const static struct timespec sleep_interval = { .tv_sec = 3, .tv_nsec = 0 }; //interval for nanosleep

    while(*(running) && *(error) == 0 && p->progress < p->operations)
    {
	    if(pthread_mutex_trylock(p->progress_mutex) == 0)
        {
            ratio = ((double)p->progress/(double)p->operations);
            //sleep the progress bar so the program can do some work
            //TODO figure out what header nanosleep is declared in if not <time.h>
            nanosleep(&sleep_interval, (struct timespec *)NULL);
        }

	    pthread_mutex_unlock(p->progress_mutex);
    	printf("%s[", action);

        int bar = ratio * bar_size;
	    for(int i=0; i<bar_size; ++i)
	    { 
	        if(i<bar) { printf("#"); } //use # as the solid part of the progress bar
	        else if(i == bar)
            {
	            //spin the cursor so that the user feels like something is happening
	            if(count == 0) printf("|");
	            else if(count == 1) printf("/");
	            else if(count == 2) printf("-");
	            else if(count == 3) printf("\\");
	        }
	        else { printf(" "); }
        }
        printf("]%.2f%%\r", ratio*100);
        fflush(stdout);
	    count = (count+1)%4;
    }
}

void setUpProgress(const arguments* args, progress_t* p, pthread_mutex_t* mutex)
{
    const uint64_t block_byte_size = args->state_size/8;
    uint64_t operations = 0;

    //if we are encrypting add 3 blocks for the header and MAC
    if(args->encrypt) { operations = 3*block_byte_size; }
    //each of the 4 steps mush be applied to the entire file
    operations += 4*(args->file_size);

    p->encrypt = args->encrypt;
    p->progress_mutex = mutex;
    p->operations = operations;
    p->progress = 0;
}
