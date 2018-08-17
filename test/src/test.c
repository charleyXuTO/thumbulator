
#include <stdio.h>
#include <string.h>

/*
#define TEST(...) if(__VA_ARGS__) {					 \
                    printf("OK: " #__VA_ARGS__ " passed at %s:%d\n", __FILE__,__LINE__); \
                  } else {						 \
                    printf("FAIL: " #__VA_ARGS__ " failed at %s:%d\n", __FILE__,__LINE__); \
                  }

#define assertTrue(...) TEST(__VA_ARGS__)
#define assertFalse(...) TEST(!(__VA_ARGS__))

static int testzero(int hm)
{
  return hm > 0;
}
*/
/*
static void testSimple() {
//#pragma PERSISTENT(x);
//int x[10] ={1,2,3,5,6,7,8,9};


  int a,b,c, r;
  int a1[10] = {1,2,3,5,6,7,8,9};
  a = 1; b = 2; c = 4; r = 0;
  
  //printf("Arithmetic Operations:\n");


  if((a << b) == 4) {
      r++;
  }

  if((c >> a) == 2) {
      r++;
  }

  if((0>0)==0) {
      r++;
  }
  if((a>b)==0) {
      r++;
  }
double a2 = 2.2;
double c2 = 1.2;
double b2 = 3.96;

b2 = b2 + 0;
//a2 = c2*b2;
//int32_t w = 0;

//int16_t z = 3000;
//int16_t f = 3000;
//w = z*f;

  if (r == 4 && b2 == 3.96) {
  }
  else { 
   while (1) {
    int k = 5;
   }
  }

  if(a1[1] > b) {
      r++; 
  }

  

  //assertFalse(testzero(0));
}
*/
int main(void)
{
 // testSimple();
 // printf("EXIT\n");


double x =1.1;
x = 3.3- x;
if (x == 2.2) {
x = 1.0;
}
else {

x = 0.0;

}

  return 0;
}

