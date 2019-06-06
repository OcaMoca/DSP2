#include "processing.h"
#include "iir.h"

#define CLIP16(x) ((x>32767)?32767:((x<-32768)?-32768:x))

Int16 shelvingLP(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, float k)
{

	Int16 filtered_input, output;
	Int32 accum;

	filtered_input = first_order_IIR(input, coeff, z_x, z_y);
	accum = (input + (Int32)filtered_input)>>1;
	accum += ((input - (Int32)filtered_input)>>1)*k;
	output = ((Int16)CLIP16(accum));

	return output;
}

Int16 shelvingHP(Int16 input, Int16* coeff,  Int16* z_x, Int16* z_y, float k)
{
	/* Your code here */
	return 0;
}

Int16 shelvingPeek(Int16 input, Int16* coeff,  Int16* z_x, Int16* z_y, float k)
{
	/* Your code here */
	return 0;
}
