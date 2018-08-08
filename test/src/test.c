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

static void testSimple() {
//#pragma PERSISTENT(x);
//int x[10] ={1,2,3,5,6,7,8,9};


  int a,b,c, r;
  //int a1[10] = {1,2,3,5,6,7,8,9};
  a = 1; b = 2; c = 4; r = 0;
  
  printf("Arithmetic Operations:\n");


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

  //if(a1[1] > b) {
    //  r++; 
  //}

  

  //assertFalse(testzero(0));
}

int main(void)
{
  testSimple();
  printf("EXIT\n");
  return 0;
}

