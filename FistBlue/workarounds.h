#ifndef INC_WORKAROUNDS
#define INC_WORKAROUNDS
/* workarounds.h */

#ifdef CPS
#error Copiling with UNIX workarounds on a CPS
#endif


struct inputs {
	unsigned char	p10;
	unsigned char	p11;
	unsigned char	p20;
	unsigned char	p21;
};

#define GET_INPUT_1       0
#define GET_INPUT_P1_1    gInputs.p11
#define GET_INPUT_P1_0    gInputs.p10
#define GET_INPUT_P2_1    0
#define GET_INPUT_P2_0    0
#define CHECK_SERVICE_BUTTON 0


#define GET_JUMPER(x) 0

#endif