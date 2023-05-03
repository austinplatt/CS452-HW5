#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"



int main() {

   //Head put tests
   Test_Put_AddToHeadEmptyList();
   Test_Put_AddToHead2Item();

   //Tail put tests
   Test_Put_AddToTailEmptyList();
   Test_Put_Add2Tail();

   //Tail/Head Put tests
   Test_Put_Add2Tail1Head();

   //Get Head tests
   Test_Get_Head_EmptyList();


   //Get tail tests
   Test_Get_Tail_EmptyList();
}
