#include <fftw3.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FRAME_DATA 2047

void parse_frame(fftw_complex *input, FILE *fptr);
void write_out(fftw_complex *out);
void appendchar(char *array, char symbol);
void display(fftw_complex *out, int compression, double sensitivity);

int main() {

  FILE *fptr;

  fftw_complex *in, *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FRAME_DATA);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * FRAME_DATA);
  // the data stored defaults to a double [2] where :
  // real = in[i][0] ; imaginary in[i][1]

  p = fftw_plan_dft_1d(FRAME_DATA, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  // size , in , out , sign (- in this case) , flags
  // https://www.fftw.org/fftw2_doc/fftw_2.html (for more informarion)

  if ((fptr = fopen("buffer.txt", "r")) == NULL) {
    printf("FAILED TO OPEN FRAMEBUFFER\n");
    return 1;
  }

  while (1) {

    parse_frame(in, fptr);
    fftw_execute(p);

    for (int i = 0; i < FRAME_DATA; i++) {
      out[i][0] = fabs(out[i][0]);
      if (out[i][0] > 10) {
        out[i][0] = 10;
      }
    } // this set max amplitude for the bar

    write_out(out);
    usleep(10);
    display(out, 80, 0.08);
  }

  fclose(fptr);
  fftw_free(in);
  fftw_free(out);

  return 0;
}

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

void display(fftw_complex *out, int compression, double sensitivity) {
  static int snapshots = 0;

  char buffer[8000] = "\n\t|SOUND_VISUALIZER_V1.0|\n";
  int counter = 0;
  double avg;

  printf("\e[1;1H\e[2J");

  for (int i = 0; i < (FRAME_DATA / 2.5);
       i += compression) { // this is a nightmare (fix at some point)
    for (int j = 0; j < compression; j++) {
      avg += out[i + j][0];
    }

    avg = (double)avg / (double)compression;

    printf("%lf::", avg);

    for (double k = 0; k < avg; k += sensitivity) {
      appendchar(buffer, ':');
    }
    appendchar(buffer, '\n');

    avg = 0;
  }

  printf("%s", buffer);

  snapshots++;
  printf("frames:%d\n", snapshots);
}
