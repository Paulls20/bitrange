# bitrange

A helper function to set the range of bits of std::bitset at compile time. 
Using the method 2 as explained in the following link
https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/BitOp/bitRange.html

Check the output in godbolt.org
https://godbolt.org/g/zziuWH

Method 2 This is somewhat complicated, so we'll need to break it down into steps.

First, we need to create a mask with the appropriate number of 1's. 
Suppose we want k 1's. Initially, we'll create a mask with n - k 0's, followed by k 1's. 
This is easier (for now) than creating the actual mask we need.

The question is how to compute k given low and high? 

This is a classic off-by-one situation. Your first instinct might be high - low. But this would be incorrect. 
If the index high == low, then high - low == 0.

That's not what you want. When high == low, you want to test for a single bit, i.e. bhigh (which is the same as blow since high == low). Thus, you want the formula to equal 1, when high == low.

So, the formula should be 
high - low + 1.

Here's the code to do this.

   unsigned char mask = ~0 ;   // creates all 1's, using bitwise negation of 0
   int numOnes = (high - low) + 1 ; 
   int numBits = sizeof( unsigned char ) * 8 ;

   // creates (numBits - numOnes) 0's followed by numOnes 1's
   mask >>= ( numBits - numOnes ) ; 
   
Then, we need to shift left to place the 1's in the correct spot. The question is how much to shift left?
The answer is a bit tricky. Here's how to figure the answer. 

Think about the least significant bit. It's at position b0. It currently has a value of 1. 
Where should this bit end up? Since it's the rightmost bit, it should be the rightmost 1 when we shift left. 
And where's the rightmost 1? It's at blow. So we need to shift left by low bits. 
The remaining 1's shift left with it, and the leftmost 1 should land at bhigh because we computed the number of 1's ahead of time.

   mask <<= low ;
   
So, the entire code to create the mask looks like:

   unsigned char mask = ~0 ;   // creates all 1's, using bitwise negation of 0
   int numOnes = (high - low) + 1 ; 
   int numBits = sizeof( unsigned char ) * 8 ;

   // creates (numBits - numOnes) 0's followed by numOnes 1's
   mask >>= ( numBits - numOnes ) ; 
   mask <<= low ; // shift left group of 1's to correct location
   
Notice that no loops were used.

The one possible problem we have is the mask being shifted to the right. 
If we had used a signed char, instead of an unsigned char, we might have had the sign bit shifted in from the left. 
That would not create the desired mask (instead, it would leave us with all 1's).

So, here's a solution that avoids this problem.


   unsigned char mask = ~0 ;   // creates all 1's, using bitwise negation of 0
   int numOnes = (high - low) + 1 ; 

   // creates (numBits - numOnes) 1's followed by numOnes 0's
   mask <<= numOnes ; 

   // creates (numBits - numOnes) 0's followed by numOnes 1's
   mask = ~mask ;
   mask <<= low ; // shift left the group of 1's to correct location
   
By left shifting, we guarantee that 0's are shifted in from the right, regardless of whether the type is signed or unsigned.
However, we need the additional step of negating the mask to get the pattern we want.
This solution avoids using right shifts, thus should be more portable.
