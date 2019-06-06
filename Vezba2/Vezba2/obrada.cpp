#include <math.h>
#include <stdlib.h>
#include "obrada.h"
#include "sr_fft.h"

#define LOW 300
#define HIGH 4000
#define SAMPLE_RATE 44100
#define PI 3.1415926535

double time_buffer[FFT_SIZE];
double fft_buffer[FFT_SIZE];
double in_delay[FFT_SIZE/2];
double out_delay[FFT_SIZE/2];

extern double window[FFT_SIZE];

void obrada(double *in, double *out, int N)
{
  int i;

  int k1 = 2*FFT_SIZE*LOW / SAMPLE_RATE;
  int k2 = 2*FFT_SIZE*HIGH / SAMPLE_RATE;

  for (i = 0; i < N; i++) {
	  time_buffer[i] = in_delay[i];
	  time_buffer[N + i] = in[i];
  }

  for (i = 0; i < N; i++) {
	  in_delay[i] = in[i];
  }

  for (i = 0; i < 2 * N; i++) {
	  time_buffer[i] *= window[i];
  }

  fft(time_buffer, fft_buffer, FFT_ORDER);

  /*for (i = 0; i < k1; i++) {
	  fft_buffer[i] = 0;
  }

  for (i = k2; i < 2*N; i++) {
	  fft_buffer[i] = 0;
  }*/

  double moduo;

  for (i = 0; i < N; i++) {
	  moduo = sqrt(pow(fft_buffer[2 * i], 2) + pow(fft_buffer[2 * i + 1], 2));
	  fft_buffer[2 * i] = moduo*cos(PI*i*i);
	  fft_buffer[2 * i + 1] = moduo*sin(PI*i*i);
  }

  ifft(fft_buffer, time_buffer, FFT_ORDER);

  for (i = 0; i < N; i++) {
	  time_buffer[i] *= window[i];
	  out_delay[i] *= window[N + i];
  }

  for (i = 0; i < N; i++) {
	  out[i] = out_delay[i] + time_buffer[i];
  }

  for (i = 0; i < N; i++) {
	  out_delay[i] = time_buffer[N + i];
  }

}
