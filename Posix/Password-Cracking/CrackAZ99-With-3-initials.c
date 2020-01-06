#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>


int no_of_passwords = 4;

char *encoded_passwords[] = {
  "$6$KB$uRKLpTBu9AT/EiH1yY2DaE2i72nK8iuAHDgGULmsML9BmwH3zmDod/.x5qmz/.QLFl9A2fUE1wgEhq45zhHiR.",
  "$6$KB$Fn3JTlKaYAYjyg2QH/8BkjqLpMWQRL.roXWgSf/SJ6.ZmHscDZ20I2hB26S0itVx5eRz.g.NyKC6qz0K5pYJj/",
  "$6$KB$kklYtCkhnqJO.eCsR5xXFeeXUal2jOWeYqa62xRgCo89KlLDtTukjNK2HpiM7ybpEMvdpWUkglufTduDqzjKD0",
  "$6$KB$XiHds32SG1nfQoq2QJqulUvCVwoRHUtl6Ye6d.2YtyxikvplVWRVrKLv1DhF8/r2ZTsvJZIkmkY9hKoO8RFJX0"
};



void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}



void pw_crack(char *salt_and_encoded){
  int p, q, r, s;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   
  substr(salt, salt_and_encoded, 0, 6);

  for(p='A'; p<='Z'; p++){
    for(q='A'; q<='Z'; q++){
	for(s='A'; s<='Z'; s++){
	      for(r=0; r<=99; r++){
		sprintf(plain, "%c%c%c%02d", p, q, s, r); 
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
  }
  printf("%d solutions explored\n", count);
}

int time_variance(struct timespec *start, struct timespec *end, 
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
  
  for(x=0;x<no_of_passwords;x<x++) {
    pw_crack(encoded_passwords[x]);
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  time_variance(&start, &end, &time_elapsed);
  printf("Time elapsed was %lldnsecs or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 


  return 0;
}
