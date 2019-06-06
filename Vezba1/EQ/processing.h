//////////////////////////////////////////////////////////////////////////////
// *
// * Laboratorijske vezbe iz predmeta OAiS DSP 2
// * Godina: 2019
// *
// * Vezba 1: Ekvalizacija audio signala
// * Autor:
// *
// *
/////////////////////////////////////////////////////////////////////////////

#ifndef EQ_PROCESSING_H_
#define EQ_PROCESSING_H_
#include "tistdtypes.h"

Int16 shelvingLP(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, float k);

Int16 shelvingHP(Int16 input, Int16* coef, Int16* z_x, Int16* z_y, float k);

Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* z_x, Int16* z_y, float k);


#endif /* EQ_PROCESSING_H_ */
