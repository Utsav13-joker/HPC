#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <pthread.h>



int no_of_passwords = 4;

char *encoded_passwords[] = {
  "$6$KB$yPvAP5BWyF7oqOITVloN9mCAVdel.65miUZrEel72LcJy2KQDuYE6xccHS2ycoxqXDzW.lvbtDU5HuZ733K0X0",
  "$6$KB$rOGcNtZd4lOABjC1NL933L/bSqkiX4Cz9zj.PyJnV6NMZgRYii2piqQuVns0q/PoBJefsqgVNOHKqg41thBCt/",
  "$6$KB$NKDUcBiSiMfq8CGoE6fElAY3SKqBcpJrCA.6eCk6iqQ7WPJmfkHxsYtqnmVKZak6slQx6dTwOY2K2URu8YAz3/",
  "$6$KB$CTFOWoj82vHbQrSm8WWY0l3IPUCVoeQu/k4PlpQ3loBG1MzHXZcg57uPkhHww0Q1DqRlUhF3WSM4o629n/Sz51"
};



void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}



void kernel_function_1(char *salt_and_encoded){
  int p, q, r;    
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encoded, 0, 6);

  for(p='A'; p<='M'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
        sprintf(plain, "%c%c%02d", p, q, r); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encoded, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void kernel_function_2(char *salt_and_encoded){
  int p, q, r;     
  char salt[7];   
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encoded, 0, 6);

  for(p='N'; p<='Z'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
        sprintf(plain, "%c%c%02d", p, q, r); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encoded, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int time_variance(struct timespec *start, 
                    struct timespec *end, 
                    long long int *variance) {
  long long int dsec =  end->tv_sec - start->tv_sec; 
  long long int dnsec =  end->tv_nsec - start->tv_nsec; 

  if(dnsec < 0 ) {
    dsec--;
    dnsec += 1000000000; 
  } 
  *variance = dsec * 1000000000 + dnsec;
  return !(*variance > 0);
}

int main(int argc, char *argv[]){
  int x;

  struct timespec start, end;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);



  pthread_t th1;
  void kernel_function_1();
  pthread_t th2;
  void kernel_function_2();

  for(x=0; x<no_of_passwords; x<x++){
    pthread_create(&th1, NULL, (void*)kernel_function_1, encoded_passwords[x]);
    pthread_create(&th2, NULL, (void*)kernel_function_2, encoded_passwords[x]);
  }

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  
  clock_gettime(CLOCK_MONOTONIC, &end);
  time_variance(&start, &end, &time_elapsed);
  printf("Time elapsed was %lldnsecs or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));

  return 0;
}
