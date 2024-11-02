//
//  random.h
//  random


#ifndef random_h
#define random_h
#include <stdint.h>

/**
 * @brief Generates a random integer.
 * 
 * This function returns a random integer value. The range and 
 * distribution of the random values depend on the implementation.
 * 
 * @return A random integer.
 */
int random();
 /**
  Numero entero en [izq, der]
  */
 int randInt(int izq, int der);

#endif /* random_h */