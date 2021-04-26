/**
 * @file
 * Contains an implementation of the extractMessage function.
 */

#include <iostream> // might be useful for debugging
#include <assert.h>
#include "extractMessage.h"

using namespace std;

char *extractMessage(const char *message_in, int length) {
   // Length must be a multiple of 8
   assert((length % 8) == 0);

   // allocates an array for the output
   char *message_out = new char[length];
   for (int i=0; i<length; i++) {
   		message_out[i] = 0;    // Initialize all elements to zero.
	}

	// TODO: write your code here


   for (int matrix_num = 0; matrix_num < length; matrix_num += 8) {
       for (int i = 0; i < 8; i++) {
           for (int j = 0; j < 8; j++) {
               // get the (i,j) bit
               int char_bit = (message_in[matrix_num + i] >> j) & 1;
               // Put it in (j,i)
               message_out[matrix_num + j] += (char_bit<<i);
           }
       }
   }

   return message_out;
}
