#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void img_create(int n){

  //Img named as thread number created
  char org[17]; snprintf(org, 17, "img/input_%i.bmp", n);
  char dst[18]; snprintf(dst, 18, "local/blur_%i.bmp", n);

  //Open files
  FILE *fptr_r; fptr_r=fopen(org,"rb");
  FILE *fptr_w; fptr_w=fopen(dst,"wb");

  long ancho, alto, size_img;
  unsigned char r, g, b;
  unsigned char* ptr;

  //BMP bitmap signature
  unsigned char xx[54];
  for(int i=0; i<54; i++){
    xx[i] = fgetc(fptr_r);
    fputc(xx[i], fptr_w);
  }

  //Read width(bytes 18-21) && height(bytes 22-25)
  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
  size_img = ancho*alto*3;

  //Memory required to save all pixels + extra values for blur
  ptr = (unsigned char*)malloc((alto*2*3+ancho*2*3+4*3*3+alto*ancho*3)*sizeof(unsigned char));

  for(int i=0; i<(alto*ancho*3)+87; i+=3){
    b = fgetc(fptr_r);
    g = fgetc(fptr_r);
    r = fgetc(fptr_r);

    ptr[i] = b; //b
    ptr[i+1] = g; //g
    ptr[i+2] = r; //r
  }

  //Goes through matrix
  for(int i=0; i<alto-1; i++){
    for(int j=0; j<(ancho*3); j+=3){
      r = g = b = 0;
      //Visit every pixel around (total of 49 on 7*7)
      for(int k=-3; k<=3; k++)
        for(int l=-3; l<=3; l++)
          if((ancho*i*3+l)+j+(k*3) > 0 && (ancho*i*3+l)+j+(k*3)+2 < size_img){
            //Sum of pixel average
            b += ptr[(ancho*i*3+l)+j+(k*3)+0]/49;
            g += ptr[(ancho*i*3+l)+j+(k*3)+1]/49;
            r += ptr[(ancho*i*3+l)+j+(k*3)+2]/49;
          }

      fputc(b, fptr_w);
      fputc(g, fptr_w);
      fputc(r, fptr_w);
    }
  }

  //Close images and destroy vars
  free(ptr);
  fclose(fptr_r);
  fclose(fptr_w);

  printf("Img #%i was succesfull!!!\n", n);
}

int main(){
  double t1, t2; t1 = omp_get_wtime();

  int img_tests = 40;

  //Set number of threads and parallel operation
  omp_set_num_threads(img_tests);
  #pragma omp parallel
    img_create(omp_get_thread_num()+1);

  printf("\nImages: %i", img_tests);
  t2 = omp_get_wtime(); printf("\nTime: %lfs\n", t2-t1);
}
