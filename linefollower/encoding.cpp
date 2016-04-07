#include <Arduino.h>
#include <EncoderMod.h>
#include "conf.h"
#include "encoding.h"

Encoder frEn(ENC_RF_1, ENC_RF_2);
Encoder flEn(ENC_LF_1, ENC_LF_2);
Encoder brEn(ENC_RB_1, ENC_RB_2); 
Encoder blEn(ENC_LB_1, ENC_LB_2);

//Returns the value of the selected encoder
float EnRead1(){
		return(flEn.read() * MM_PER_STEP);
}
float EnRead2(){
		return(frEn.read() * MM_PER_STEP);
}
float EnRead3(){
		return(blEn.read() * MM_PER_STEP);
}
float EnRead4(){
		return(brEn.read() * MM_PER_STEP);
}

//Writes value to the selected encoder
void EnWrite1(float new_position){
		flEn.write( (int32_t)rint(new_position / MM_PER_STEP));
}
void EnWrite2(float new_position){
		frEn.write( (int32_t)rint(new_position / MM_PER_STEP));
}
void EnWrite3(float new_position){
		blEn.write( (int32_t)rint(new_position / MM_PER_STEP));
}
void EnWrite4(float new_position){
		brEn.write( (int32_t)rint(new_position / MM_PER_STEP));
}

//Returns veloctiy of selected encoder
float EnVelocity1(){
	 return (1000 * flEn.stepRate() * MM_PER_STEP);
}
float EnVelocity2(){
	 return (1000 * frEn.stepRate() * MM_PER_STEP);
}
float EnVelocity3(){
	 return (1000 * blEn.stepRate() * MM_PER_STEP);
}
float EnVelocity4(){
	 return (1000 * brEn.stepRate() * MM_PER_STEP);
}

//Returns extrapolation of the selected encoder
float EnExtrapolate1(){
	return (flEn.extrapolate() * MM_PER_STEP);
}
float EnExtrapolate2(){
	return (frEn.extrapolate() * MM_PER_STEP);
}
float EnExtrapolate3(){
	return (blEn.extrapolate() * MM_PER_STEP);
}
float EnExtrapolate4(){
	return (brEn.extrapolate() * MM_PER_STEP);
}
