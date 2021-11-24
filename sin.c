/* Compare sin functions.
 * Compile with -lmpfr -lgmp -lm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <mpfr.h>

#include "comparisons.h"

/* 

All comparisons between -2 and 2

Performance (seconds to compute 20M values):
                                    tempo netto
- Identity:               1.792901 
- ASM fsin:	          8.824445 
- standard sinf:          5.100249 
- Unreal:	          5.918448  4.1
- Unreal simplified:	  4.671291  2.8

- Unreal simplified fmaf: 7.854344


Performance with -O3:

- Identity:	          1.620953 
- ASM fsin:	          7.928299 
- C standard:	          2.916727 
- Unreal:	          2.905149 
- Unreal simplified:	  2.602684 
- Unreal simplified fmaf: 3.440821 


Performance with -O3 -march=native:
- Identity:	          1.365248 
- ASM fsin:	          7.832927 
- C standard:	          2.872953 
- C standard wide:	  3.729577
- MPFR sin (30->12):    212.008571
- MPFR sin (12->12):    211.776870
- Unreal:	          2.610490
- Unreal simplified:	  2.462644 
- Unreal simplified fmaf: 2.463407


1) standard_sinf VS unreal_sin:

Points: 2 147 483 648
Points where f1 is bigger: 23 477 979 (0.010933%)
Points where f2 is bigger: 23 477 979 (0.010933%)
Maximum positive deviation (f1>f2):  1.788139e-07 with input 1.431490
Maximum negative deviation (f1<f2): -1.788139e-07 with input -1.720571
Total error: 0.000000e+00
Average error: 0.000000e+00
Total squared error: 4.757289e-08
Average squared error: 2.215285e-17


2) standard_sinf VS my_simplified_sin:

Points where f1 is bigger: 33 288 270 (0.015501%)
Points where f2 is bigger: 33 288 271 (0.015501%)
Maximum positive deviation (f1>f2):  1.569390e-04 with input 1.570755
Maximum negative deviation (f1<f2): -1.569390e-04 with input -1.570838
Total error: -9.000301e-06
Average error: -4.191092e-15
Total squared error: 6.846702e-02
Average squared error: 3.188244e-11


3) standard_sin VS standard_sinf_widened

Points where f1 is bigger: 222782557 (0.103741%)
Points where f2 is bigger: 222782558 (0.103741%)
Maximum positive deviation (f1>f2): 2.980235e-08 with input -1.359368
Maximum negative deviation (f1<f2): -2.980235e-08 with input 1.359368
Total error: -2.015225e-08
Average error: -9.384123e-18
Total squared error: 1.140431e-08
Average squared error: 5.310544e-18


4) standard_sin VS unreal_sin_widened

Points where f1 is bigger: 222782557 (0.103741%)
Points where f2 is bigger: 222782558 (0.103741%)
Maximum positive deviation (f1>f2): 1.706003e-07 with input 1.576068
Maximum negative deviation (f1<f2): -1.706003e-07 with input -1.576068
Total error: -2.015223e-08
Average error: -9.384113e-18
Total squared error: 3.806658e-08
Average squared error: 1.772613e-17


5) standard_sinf VS asm_sin

Points where f1 is bigger: 8 (0.000000%)
Points where f2 is bigger: 8 (0.000000%)
Maximum positive deviation (f1>f2): 5.960464e-08 with input -0.912922
Maximum negative deviation (f1<f2): -5.960464e-08 with input -1.775031
Total error: 0.000000e+00
Average error: 0.000000e+00
Total squared error: 4.275053e-14
Average squared error: 1.990726e-23


*/

// #define M_PI 3.14159265358979323846
#define PI M_PI
#define HALF_PI (M_PI/2)
#define INV_PI (1/M_PI)

void SinCos( float* ScalarSin, float* ScalarCos, float  Value );
void UnrealSin( float* ScalarSin, float  Value );
  
float identity(float angle);
float asm_fsin(float angle);
double asm_fsin_double(float angle);
  
float standard_sinf(float angle);
double standard_sinf_widened(float angle);
double standard_sin(float angle);
float standard_sin_narrowed(float angle);

float unreal_sin(float angle);
double unreal_sin_widened(float angle);
float my_sin(float Value);
float my_sin_fmaf(float Value);
float mpfr_sinf(float angle);

void performanceTests() {
  printf("Identity:\t");          testFloatFunctionPerformance(identity, -2, 2);
  printf("ASM fsin:\t");          testFloatFunctionPerformance(asm_fsin, -2, 2);
  printf("C standard:\t");        testFloatFunctionPerformance(standard_sinf, -2, 2);
  printf("C standard wide:\t");   testFloatFunctionPerformance(standard_sin_narrowed, -2, 2);
  printf("MPFR sin:\t");          testFloatFunctionPerformance(mpfr_sinf, -2, 2);
  printf("Unreal:\t");            testFloatFunctionPerformance(unreal_sin, -2, 2);
  printf("Unreal simplified:\t"); testFloatFunctionPerformance(my_sin, -2, 2);
  printf("Unreal simplified fmaf:\t"); testFloatFunctionPerformance(my_sin_fmaf, -2, 2);
}

mpfr_t x, mpfr_angle;

int main() {
  const unsigned precision = 12; // bits
  mpfr_init2(x, precision);
  mpfr_init2(mpfr_angle, precision);

  // printf("%u %u\n", MPFR_PREC_MIN, MPFR_PREC_MAX);
  // findFirstDifference(standard_sin, asm_fsin_double, 0, 1.5);
  performanceTests();
  // compareFloatFunctions(standard_sinf, asm_fsin, -2, 2);
  // compareFloatToDoubleFunctions(standard_sin, asm_fsin_double, -2, 2);
}


float mpfr_sinf(float angle) {
  const mpfr_rnd_t round = MPFR_RNDN;
  int err1 = mpfr_set_flt(mpfr_angle, angle, round);
  int err2 = mpfr_sin(x, mpfr_angle, round);
  return mpfr_get_flt(x, round);
}

float asm_fsin(float angle) {
  float answer;
  __asm("fsin" : "=t" (answer) : "0" (angle));
  return answer;
}

double asm_fsin_double(float angle) {
  double answer;
  __asm("fsin" : "=t" (answer) : "0" (angle));
  return answer;
}


float identity(float x) { return x; }

float standard_sinf(float angle) {
  return sin(angle);
}

double standard_sinf_widened(float angle) {
  return sin(angle);
}

double standard_sin(float angle) {
  return sin((double)angle);
}

float standard_sin_narrowed(float angle) {
  return (float) sin((double)angle);
}

float unreal_sin(float angle) {
  float s;
  UnrealSin(&s, angle);
  return s;
}

double unreal_sin_widened(float angle) {
  float s;
  UnrealSin(&s, angle);
  return s;
}

/* Like unreal_sin, but degree 7 instead of 11. */
float my_sin(float Value) {
  // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
  float quotient = (INV_PI*0.5f)*Value;
  if (Value >= 0.0f)
    {
      quotient = (float)((int)(quotient + 0.5f));
    }
  else
    {
      quotient = (float)((int)(quotient - 0.5f));
    }
  float y = Value - (2.0f*PI)*quotient;
  
  // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
  if (y > HALF_PI)
    {
      y = PI - y;
    }
  else if (y < -HALF_PI)
    {
      y = -PI - y;
    }
  
  float y2 = y * y;
  
  // 7-degree minimax approximation
  return ( ( ( (- 0.00019840874f ) * y2 + 0.0083333310f ) * y2 - 0.16666667f ) * y2 + 1.0f ) * y;
}


/* Like unreal_sin, but degree 7 instead of 11. */
float my_sin_fmaf(float Value) {
  // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
  float quotient = (INV_PI*0.5f)*Value;
  if (Value >= 0.0f) {
      quotient = (float)((int)(quotient + 0.5f));
  } else {
    quotient = (float)((int)(quotient - 0.5f));
  }
  float y = Value - (2.0f*PI)*quotient;
  
  // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
  if (y > HALF_PI) {
    y = PI - y;
  } else if (y < -HALF_PI) {
    y = -PI - y;
  } 
  
  float y2 = y * y;  
  // 7-degree minimax approximation
  return y * fmaf(fmaf(fmaf(- 0.00019840874f, y2, 0.0083333310f), y2, -0.16666667f), y2, +1.0f);
}


/**
 * Computes the sine of a scalar value. 
 * Modified from Unreal Engine to compute just sin and not cos.
 *
 * @param ScalarSin	Pointer to where the Sin result should be stored
 * @param Value  input angles 
 */
void UnrealSin( float* ScalarSin, float  Value )
{
  // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
  float quotient = (INV_PI*0.5f)*Value;
  if (Value >= 0.0f)
    {
      quotient = (float)((int)(quotient + 0.5f));
    }
  else
    {
      quotient = (float)((int)(quotient - 0.5f));
    }
  float y = Value - (2.0f*PI)*quotient;
  
  // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
  if (y > HALF_PI) {
      y = PI - y;
  } else if (y < -HALF_PI) {
      y = -PI - y;
  }
  
  float y2 = y * y;
  
  // 11-degree minimax approximation
  *ScalarSin = ( ( ( ( (-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f ) * y2 + 0.0083333310f ) * y2 - 0.16666667f ) * y2 + 1.0f ) * y;
}



/**
 * Computes the sine and cosine of a scalar value. From Unreal Engine.
 *
 * @param ScalarSin	Pointer to where the Sin result should be stored
 * @param ScalarCos	Pointer to where the Cos result should be stored
 * @param Value  input angles 
 */
void SinCos( float* ScalarSin, float* ScalarCos, float  Value )
{
  // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
  float quotient = (INV_PI*0.5f)*Value;
  if (Value >= 0.0f)
    {
      quotient = (float)((int)(quotient + 0.5f));
    }
  else
    {
      quotient = (float)((int)(quotient - 0.5f));
    }
  float y = Value - (2.0f*PI)*quotient;
  
  // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
  float sign;
  if (y > HALF_PI)
    {
      y = PI - y;
      sign = -1.0f;
    }
  else if (y < -HALF_PI)
    {
      y = -PI - y;
      sign = -1.0f;
    }
  else
    {
      sign = +1.0f;
    }
  
  float y2 = y * y;
  
  // 11-degree minimax approximation
  *ScalarSin = ( ( ( ( (-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f ) * y2 + 0.0083333310f ) * y2 - 0.16666667f ) * y2 + 1.0f ) * y;
  
  // 10-degree minimax approximation
  float p = ( ( ( ( -2.6051615e-07f * y2 + 2.4760495e-05f ) * y2 - 0.0013888378f ) * y2 + 0.041666638f ) * y2 - 0.5f ) * y2 + 1.0f;
  *ScalarCos = sign*p;
}
