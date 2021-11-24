#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>

/* Tests the performance of a float->float function over random
   inputs between min and max.
   Prints the results on stdout.
*/
void testFloatFunctionPerformance(float (*f)(float), float min, float max) {
  const int attempts = 200000000;
  double tot = 0;

  clock_t start = clock();
  for (int i =0; i<attempts; i++) {
    float x = (float)(rand()/(double)(RAND_MAX));
    float y = f(x);
    tot += y;    
  }
  double time = (double)(clock()-start)/CLOCKS_PER_SEC;
  printf("%f (tot %f)\n", time, tot);
}


/* Tests the performance of a 2D distance function over random inputs.
   Prints the results on stdout.
*/
void testDistanceFunctionPerformance(float (*f)(float, float, float, float)) {
  const int attempts = 100000000;
  double tot = 0;

  clock_t start = clock();
  for (int i =0; i<attempts; i++) {
    float x1 = (float)(rand()/(double)(RAND_MAX));
    float y1 = (float)(rand()/(double)(RAND_MAX));
    float x2 = (float)(rand()/(double)(RAND_MAX));
    float y2 = (float)(rand()/(double)(RAND_MAX));
    float y = f(x1, y1, x2, y2);
    tot += y;    
  }
  double time = (double)(clock()-start)/CLOCKS_PER_SEC;
  printf("%f (tot %f)\n", time, tot);
}


void findFirstDifference(double (*f1)(float), double (*f2)(float), float min, float max) {
  // For all floats from min to max
  for (float x = min; x<max; x = nextafter(x, max)) {
    double y1 = f1(x);
    double y2 = f2(x);

    if (y1!=y2) {
      printf("x=%.20f  \t%.17g \t%a\n", x, x, x);
      printf("f1=%.20f \t%.17g \t%a\n", y1, y1, y1);
      printf("f2=%.20f \t%.17g \t%a\n", y2, y2, y2);
      return;
    }
  }
  printf("No difference was found.\n");
}


void compareFloatFunctions(float (*f1)(float), float (*f2)(float), float min, float max) {
  unsigned n = 0, n_plus = 0, n_minus = 0;
  float max_pos_error_input = 0, max_neg_error_input = 0;
  long double tot_error = 0, tot_squared_error = 0;
  double max_pos_error = 0, max_neg_error = 0;

  float progress = (max-min)/100, next_progress = min + progress;

  // For all floats from min to max
  for (float x = min; x<max; x = nextafter(x, max)) {
    if (x>=next_progress) {
      //printf("\t%f", x);
      //fflush(stdout);
      next_progress += progress;
    }

    double y1 = f1(x);
    double y2 = f2(x);
    double error = y1 - y2;
    if (error > max_pos_error) {
      max_pos_error = error;
      max_pos_error_input = x;
    } else if (error < max_neg_error) {
      max_neg_error = error;
      max_neg_error_input = x;
    } 
    tot_error += error;
    tot_squared_error += error * error;
    if (error>0) {
      printf("%f\t", x);
      n_plus++;
    }
    else if (error<0) n_minus++;
    n++;
  }

  printf("\nPoints: %u\n", n);
  printf("Points where f1 is bigger: %u (%f)\n", n_plus, n_plus/(double)n);
  printf("Points where f2 is bigger: %u (%f)\n", n_minus, n_minus/(double)n);
  printf("Maximum positive deviation (f1>f2): %e with input %f\n", max_pos_error, max_pos_error_input);
  printf("Maximum negative deviation (f1<f2): %e with input %f\n", max_neg_error, max_neg_error_input);
  printf("Total error: %Le\n", tot_error);
  printf("Average error: %Le\n", tot_error/n);
  printf("Total squared error: %Le\n", tot_squared_error);
  printf("Average squared error: %Le\n", tot_squared_error/n);
}


void compareFloatToDoubleFunctions(double (*f1)(float), double (*f2)(float), float min, float max) {
  unsigned n = 0, n_plus = 0, n_minus = 0;
  float max_pos_error_input = 0, max_neg_error_input = 0;
  long double tot_error = 0, tot_squared_error = 0;
  double max_pos_error = 0, max_neg_error = 0;

  float progress = (max-min)/100, next_progress = min + progress;

  // For all floats from min to max
  for (float x = min; x<max; x = nextafter(x, max)) {
    if (x>=next_progress) {
      printf("\t%f", x);
      fflush(stdout);
      next_progress += progress;
    }

    double y1 = f1(x);
    double y2 = f2(x);
    double error = y1 - y2;
    if (error > max_pos_error) {
      max_pos_error = error;
      max_pos_error_input = x;
    } else if (error < max_neg_error) {
      max_neg_error = error;
      max_neg_error_input = x;
    } 
    tot_error += error;
    tot_squared_error += error * error;
    if (error>0) n_plus++;
    else if (error<0) n_minus++;
    // if (error) {}
    n++;
  }

  printf("\nPoints: %u\n", n);
  printf("Points where f1 is bigger: %u (%f)\n", n_plus, n_plus/(double)n);
  printf("Points where f2 is bigger: %u (%f)\n", n_minus, n_minus/(double)n);
  printf("Maximum positive deviation (f1>f2): %e with input %.15f\n", max_pos_error, max_pos_error_input);
  printf("Maximum negative deviation (f1<f2): %e with input %.15f\n", max_neg_error, max_neg_error_input);
  printf("Total error: %Le\n", tot_error);
  printf("Average error: %Le\n", tot_error/n);
  printf("Total squared error: %Le\n", tot_squared_error);
  printf("Average squared error: %Le\n", tot_squared_error/n);
}


