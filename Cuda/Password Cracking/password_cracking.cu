#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>

__device__ int is_a_match(char *no_of_tries) {
  char password1[] = "AV7201";
  char password2[] = "FT2702";
  char password3[] = "IR7503";
  char password4[] = "SC5504";

  char *p = no_of_tries;
  char *q = no_of_tries;
  char *r = no_of_tries;
  char *s = no_of_tries;
  char *pw1 = password1;
  char *pw2 = password2;
  char *pw3 = password3;
  char *pw4 = password4;

  while(*p == *pw1) {
   if(*p == '\0')
    {
    printf("Found password: %s\n",password1);
      break;
    }

    p++;
    pw1++;
  }
    
  while(*q == *pw2) {
   if(*q == '\0')
    {
    printf("Found password: %s\n",password2);
      break;
}

    q++;
    pw2++;
  }

  while(*r == *pw3) {
   if(*r == '\0')
    {
    printf("Found password: %s\n",password3);
      break;
    }

    r++;
    pw3++;
  }

  while(*s == *pw4) {
   if(*s == '\0')
    {
    printf("Found password: %s\n",password4);
      return 1;
    }

    s++;
    pw4++;
  }
  return 0;

}


__global__ void  kernel() {
char a,b,c,d;
 
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(a='0'; a<='9'; a++){
      for(b='0'; b<='9'; b++){
        for(c='0';c<='9';c++){
          for(d='0';d<='9';d++){
            password[2] = a;
            password[3] = b;
            password[4]= c;
            password[5]=d;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      }
   }
}
int time_variance(struct timespec *start,
                    struct timespec *end,
                    long long int *variance) {
  long long int sec =  end->tv_sec - start->tv_sec;
  long long int nsec =  end->tv_nsec - start->tv_nsec;

  if(nsec < 0 ) {
    sec--;
    nsec += 1000000000;
  }
  *variance = sec * 1000000000 + nsec;
  return !(*variance > 0);
}


int main() {

  struct  timespec start, end;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &end);
  time_variance(&start, &end, &time_elapsed);
  printf("Time elapsed was %lldnsecs or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 
  return 0;
}



