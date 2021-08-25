#include <intrin.h>
#include <cstdint>



unsigned int InternalStorage;
int Result;

unsigned int  rotl32(unsigned int  value, unsigned int count) {
	return value << count | value >> (32 - count);
}


unsigned char Permute(char Input) {
	unsigned char B1 = Input & 0x1;
	unsigned char B2 = (Input & 0x2) == 0x2;
	unsigned char B3 = (Input & 0x4) == 0x4;
	unsigned char B4 = (Input & 0x8) == 0x8;

	unsigned char B5 = (Input & 0x10) == 0x10;
	unsigned char B6 = (Input & 0x20) == 0x20;
	unsigned char B7 = (Input & 0x40) == 0x40;
	unsigned char B8 = (Input & 0x80) == 0x80;

	return B3 << 7 | B8 << 6 | B4 << 5 | B7 << 4 | B2 << 3 | B1 << 2 | B6 << 1 | B5;

}


unsigned int HashLetter(unsigned char Letter, unsigned int State) {
	

	unsigned char Mix = (State >> 8 & 0xff) ^ (State >> 16 & 0xff) ^ (State >> 24 & 0xff);
	Mix = Permute(Mix);
	unsigned char NewPart = Letter ^ Mix;

	State = rotl32(State,7) ^ NewPart;
	return State;
}


