#ifndef PLAYGROUND_FIBONACCI_H
#define PLAYGROUND_FIBONACCI_H
#define FIBONACCI_MODULUS 666013
/* All variants use F(0)=0, F(1)=1 and return F(n) modulo 666013. */
int fibo_recursive(unsigned int n);
int fibo_iterative(unsigned int n);
int fibo_logarithmic(unsigned int n);
#endif
