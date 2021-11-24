/* Compare sin functions.
 * Compile with -lmpfr -lgmp -lm.
 * Possibly with -O3 -march=native
 */

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>

#include "comparisons.h"

float fake_distance(float x1, float y1, float x2, float y2);
float distance(float x1, float y1, float x2, float y2);
float squared_distance(float x1, float y1, float x2, float y2);

void performanceTests() {
  printf("Baseline:\t");          testDistanceFunctionPerformance(fake_distance);
  printf("Distance:\t");          testDistanceFunctionPerformance(distance);
  printf("Squared distance:\t");  testDistanceFunctionPerformance(squared_distance);
}


int main() {
  performanceTests();
}


float fake_distance(float x1, float y1, float x2, float y2) {
  return x1;
}

float distance(float x1, float y1, float x2, float y2) {
  return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

float squared_distance(float x1, float y1, float x2, float y2) {
  return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}
