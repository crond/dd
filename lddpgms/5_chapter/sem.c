#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<stdarg.h>
#include<semaphore.h>
#include<errno.h>
#include<unistd.h>

sem_t G_sem;
#define SHARE_BW_THRD	0x0
#define TOT_ALWD_ACC	0x1
int main()
{
	if( (sem_init(&G_sem,SHARE_BW_THRD,TOT_ALWD_ACC)) == 0)
    {
        printf("\n SEM Init Success \n");
    }
    else
    {
        perror("Error while init Sem. ");
    }    
	return 0;

}
