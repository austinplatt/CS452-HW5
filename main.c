#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"



int main() {

   // Head put tests
   Test_Put_AddToHeadEmptyList();
   Test_Put_AddToHead2Item();
   Test_Put_AddToHead3Item();

   // Tail put tests
   Test_Put_AddToTailEmptyList();
   Test_Put_Add2Tail();
   Test_Put_Add3Tail();

   // Mixed Put tests
   Test_Put_Add2Head1Tail();
   Test_Put_Add2Tail1Head();

   // Get Head tests
   Test_Get_Head_EmptyList();
   Test_Get_Head_1Item();
   Test_Get_AddToHead2ItemGetHead();

   // Get tail tests
   Test_Get_Tail_EmptyList();
   Test_Get_Tail_1Item();
   Test_Get_AddToTail2ItemGetTail();

   // Mixed get tests
   Test_Get_Add2Head2TailGetTail();
   Test_Get_Add2Tail2HeadGetHead();
   Test_Get_Add3TailGetHead();

   // Head ith tests
   Test_ith_Head_EmptyList();
   Test_ith_Head_OutOfBounds();
   Test_ith_Head_1Item();
   Test_ith_Head_3Item();

   // Tail ith tests
   Test_ith_Tail_EmptyList();
   Test_ith_Tail_OutOfBounds();
   Test_ith_Tail_1Item();
   Test_ith_Tail_3Item();
   
   // Head rem tests
   Test_rem_Head_EmptyList();
   Test_rem1_Head_Add1();
   Test_rem1_Head_Add2();
   Test_remMiddle_Head_Add3();
   Test_remHead_AddHead();

   // Tail rem tests
   Test_rem_Tail_EmptyList();
   Test_rem1_Tail_Add1();
   Test_rem1_Tail_Add2();
   Test_remMiddle_Tail_Add3();
   Test_remTail_AddTail();
   
   // Mixed rem tests
   Test_remMiddle_GetHeadTail();
   Test_remMiddle_GetHeadTail();
}
