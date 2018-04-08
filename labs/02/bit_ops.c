#include <stdio.h>
#include <stdlib.h>
// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
  // YOUR CODE HERE
  // Returning -1 is a placeholder (it makes
  // no sense, because get_bit only returns
  // 0 or 1)
  // The principal idea is to zero out everything that is not
  // the value at n by left shifting a 1 until that position
  // then, righting shifting by n so that it's the only value left
  // we zero out everything before, because if not there will still be
  // bits left over when we right shift.
  return (x & (1 << n)) >> n;
}

// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
  // YOUR CODE HERE
  // I am given a a pointer some bits, x, a position n, and a value v
  // i have to assign the nth position in x to v
  // OR can set a bit to 1, AND can set it to 0
  // IF I can change a single bit in the mask, without affect the others, than I can AND that with the value of x and get what I want. However that's the origiinal problem. But what I can do in  mask, is left shift a one to position n, then negate that allowing me to easily set that bit with an OR and a left shift of v by n. ANDing that covers the case when the bit to be set is within x; however, is n is past the lenght of x, since x is infinitely left padded with 0s, the AND will have no effect. In case of that, or x with the value of v left shifted to n (since n is past x, and thus x[n] == 0, it'll be easily overridden);
  unsigned mask = ~(1 << n) | (v << n);
  *x &= mask;
  *x |= (v << n);
}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
  // YOUR CODE HERE
  set_bit(x, n, !get_bit(*x, n));
}


/*
 * YOU CAN IGNORE THE REST OF THIS FILE
 */

void test_get_bit(unsigned x,
                  unsigned n,
                  unsigned expected) {
  unsigned a = get_bit(x, n);
  if(a!=expected) {
    printf("get_bit(0x%08x,%u): 0x%08x, expected 0x%08x\n",x,n,a,expected);
  } else {
    printf("get_bit(0x%08x,%u): 0x%08x, correct\n",x,n,a);
  }
}
void test_set_bit(unsigned x,
                  unsigned n,
                  unsigned v,
                  unsigned expected) {
  unsigned o = x;
  set_bit(&x, n, v);
  if(x!=expected) {
    printf("set_bit(0x%08x,%u,%u): 0x%08x, expected 0x%08x\n",o,n,v,x,expected);
  } else {
    printf("set_bit(0x%08x,%u,%u): 0x%08x, correct\n",o,n,v,x);
  }
}
void test_flip_bit(unsigned x,
                   unsigned n,
                   unsigned expected) {
  unsigned o = x;
  flip_bit(&x, n);
  if(x!=expected) {
    printf("flip_bit(0x%08x,%u): 0x%08x, expected 0x%08x\n",o,n,x,expected);
  } else {
    printf("flip_bit(0x%08x,%u): 0x%08x, correct\n",o,n,x);
  }
}
int main(int argc,
         const char * argv[]) {
  printf("\nTesting get_bit()\n\n");
  test_get_bit(0b1001110,0,0);
  test_get_bit(0b1001110,1,1);
  test_get_bit(0b1001110,5,0);
  test_get_bit(0b11011,3,1);
  test_get_bit(0b11011,2,0);
  test_get_bit(0b11011,9,0);
  printf("\nTesting set_bit()\n\n");
  test_set_bit(0b1001110,2,0,0b1001010);
  test_set_bit(0b1101101,0,0,0b1101100);
  test_set_bit(0b1001110,2,1,0b1001110);
  test_set_bit(0b1101101,0,1,0b1101101);
  test_set_bit(0b1001110,9,0,0b1001110);
  test_set_bit(0b1101101,4,0,0b1101101);
  test_set_bit(0b1001110,9,1,0b1001001110);
  test_set_bit(0b1101101,7,1,0b11101101);
  printf("\nTesting flip_bit()\n\n");
  test_flip_bit(0b1001110,0,0b1001111);
  test_flip_bit(0b1001110,1,0b1001100);
  test_flip_bit(0b1001110,2,0b1001010);
  test_flip_bit(0b1001110,5,0b1101110);
  test_flip_bit(0b1001110,9,0b1001001110);
  printf("\n");
  return 0;
}
