#include <fftw3.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define FRAME_DATA 2047

void parse_frame(fftw_complex *input, FILE *fptr) {

    fseek(fptr, 0, SEEK_SET);
    fflush(fptr);

  for (int i = 0; i < FRAME_DATA; i++) {
    fscanf(fptr, "%lf", &input[i][0]);
  }
}

void write_out(fftw_complex *out) {

  FILE *outputer;
  char buff[20];

  if ((outputer = fopen("../pygraph/fftw_out.txt", "w")) == NULL) {
    printf("FAILED TO OPEN OUTPUT BUFFER");
    return;
  }

  for (int i = 0; i < FRAME_DATA; i++) {
    sprintf(buff, "%f\n", (out[i][0]));
    fputs(buff, outputer);
  };
}

void appendchar(char *array, char symbol) {
  int len = strlen(array);
  array[len] = symbol;
  array[len + 1] = '\0';
  return;
}

void display(fftw_complex *out, int compression, double amplitude) {
  static int snapshots = 0;

  char buffer[8000] = "\n\t|SOUND_VISUALIZER_V1.0|\n";
  int counter= 0;
  double avg;

  printf("\e[1;1H\e[2J");

  for (int i = 0; i < (FRAME_DATA/2.5); i += compression ) { // this is a nightmare (fix at some point)
    for (int j = 0 ; j < compression ; j++) {
      avg += out[i+j][0];
    }

    avg = (double)avg / (double)compression;
    //printf("%lf",avg);

    for (double k =0; k < avg; k+=amplitude) {
      appendchar(buffer, ':');
    }
      appendchar(buffer, '\n');
    avg = 0;
  }

  printf("%s", buffer);

  snapshots++;
  printf("frames:%d\n", snapshots);
}

int main() {

  // to compute a 1 dimension fourier transform of size n

  // size of transform to be computed
  FILE *fptr;

  fftw_complex *in, *out;

  // create input and output arrays

  fftw_plan p;
  // create plan pointer before initialization for safety

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FRAME_DATA);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FRAME_DATA);
  // the data stored defaults to a double [2] where :
  // real = in[i][0] ; imaginary in[i][1]

  p = fftw_plan_dft_1d(FRAME_DATA, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  // size , in , out , sign (- in this case) , flags
  // __sign__
  // FFTW_BACKWARDS = (+)
  // __FLAGS__
  // FFTW_ESTIMATE :  run a random fourier transorm algo
  // FFTW_MEASURE :  run multiple algos to determine the most optimal
  // when perfoming transforms of the same size and init time is irrelevant use
  // measure

  if ((fptr = fopen("buffer.txt", "r")) == NULL) {
    printf("FAILED TO OPEN FRAMEBUFFER\n");
    return 1;
  }

  while (1) {

    parse_frame(in, fptr);
    fftw_execute(p);

    for (int i = 0; i < FRAME_DATA; i++) {
      out[i][0] = fabs(out[i][0]);
      if (out[i][0] > 10){
        out[i][0] = 10;
      }
    }  // this set max amplitude for the bar

   write_out(out);
   usleep(10);
   display(out, 15 , 0.1);
  }


  fclose(fptr);
  // executes the fast fourrier transform
  // the result is unormalized

  // the result is stored on the output

  fftw_free(in);
  fftw_free(out);

  return 0;
}
