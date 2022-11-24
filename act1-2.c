#include <stdio.h>
#include <omp.h>

void iteracion(int N){

  char dst[19]; snprintf(dst, 19, "euler/Euler_%i.txt", omp_get_thread_num()+1);

  FILE *fptr; fptr=fopen(dst,"w");

  double h, t[N], w[N], w0=0.5, a=0, b=2;

  h = (b-a)/N;
  w[0] = w0; t[0] = a;

  for(int i=1;i<N;i++){
      t[i] = a+(h*i);
      w[i] = w[i-1] + h*(1 + t[i-1]*t[i-1] - w[i-1]);

      fprintf(fptr, "%f\t %f\n", w[i], t[i]);
  }

  printf("Tread #%i\n   N: %i\n   W: %f\n\n", omp_get_thread_num()+1, N, w[N-1]); fclose(fptr);
}

int main(){
  double t1, t2; t1 = omp_get_wtime();
  int tests[] = {523703, 523703, 523703, 523703, 523703};

  omp_set_num_threads(5);
  #pragma omp parallel
    iteracion(tests[omp_get_thread_num()]);

  t2 = omp_get_wtime(); printf("\nTime: %lfs\n", t2-t1);
}
