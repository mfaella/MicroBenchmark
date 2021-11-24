void testFloatFunctionPerformance(float (*f)(float), float min, float max);
void testDistanceFunctionPerformance(float (*f)(float, float, float, float));
void findFirstDifference(double (*f1)(float), double (*f2)(float), float min, float max);
void compareFloatFunctions(float (*f1)(float), float (*f2)(float), float min, float max);
void compareFloatToDoubleFunctions(double (*f1)(float), double (*f2)(float), float min, float max);
