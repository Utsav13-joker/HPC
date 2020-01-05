#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <math.h>
#include <malloc.h>

int no_of_password = 4;

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

void function_position_1(char *salt_and_encoded){
  int p, q, r;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int total = 0;   

  substr(salt, salt_and_encoded, 0, 6);

  for(p='A'; p<='M'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
        sprintf(plain, "%c%c%02d", p, q, r); 
        enc = (char *) crypt(plain, salt);
        total++;
        if(strcmp(salt_and_encoded, enc) == 0){
          printf("#%-8d%s %s\n", total, plain, enc);
        } else {
          printf(" %-8d%s %s\n", total, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", total);
}

void function_position_2(char *salt_and_encoded){
  int p, q, r, o;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int total = 0;   

  substr(salt, salt_and_encoded, 0, 6);

  for(p='N'; p<='Z'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
	for(o = 0; o<=99; o++){
        sprintf(plain, "%c%c%02d%02d", p, q, r, o); 
        enc = (char *) crypt(plain, salt);
        total++;
        if(strcmp(salt_and_encoded, enc) == 0){
          printf("#%-8d%s %s\n", total, plain, enc);
        } else {
          printf(" %-8d%s %s\n", total, plain, enc);
        }
      }
    }
  }
}
  printf("%d solutions explored\n", total);
}

int time_variance(struct timespec *start, struct timespec *end, long long int *variance) {
  long long int ds =  end->tv_sec - start->tv_sec; 
  long long int dn =  end->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *variance = ds * 1000000000 + dn;
  return !(*variance > 0);
}

int main(int argc, char** argv){
  int x;
  struct timespec start, end;   
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int size, position;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &position);
  if(size!=3){
      if(position==0){
          printf("This program needs to run in 3 processes\n");
      }
  }
  else{
      if(position==0){
          MPI_Send(&encoded_passwords[4], 4, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
          MPI_Send(&encoded_passwords[4], 4, MPI_CHAR, 2, 0, MPI_COMM_WORLD);
      }
      else if(position==1){
          char *recieved_encoded_passwords[4];
          MPI_Recv(&recieved_encoded_passwords, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          
          for(x=0; x<no_of_password; x<x++){
            function_position_1(encoded_passwords[x]);
          }
      }
      else{
        char *recieved_encoded_passwords[4];
        MPI_Recv(&recieved_encoded_passwords, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          
        for(x=0; x<no_of_password; x<x++){
            function_position_2(encoded_passwords[x]);
        }
      }
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();


  if(position == 0){
    long long int time_elapsed;
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_variance(&start, &end, &time_elapsed);
    printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
           (time_elapsed/1.0e9)); 
  }

  return 0;
}

