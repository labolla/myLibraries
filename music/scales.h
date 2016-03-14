/*
  scales.h - library to built and use common scales
  Created by LaBolla, March 13 2016
  https://github.com/labolla
  Released into the public domain.

*/

#include "pitches.h"

#ifndef SCALES_H_INCLUDED
#define SCALES_H_INCLUDED

// define offset idx where octave 'n' start; and offset of each note
#define OCTAVE_1_IDX  1
#define OCTAVE_2_IDX 13
#define OCTAVE_3_IDX 25
#define OCTAVE_4_IDX 37
#define OCTAVE_5_IDX 49
#define OCTAVE_6_IDX 61
#define OCTAVE_7_IDX 73

#define C_OFFSET      0
#define CS_OFFSET     1
#define D_OFFSET      2
#define DS_OFFSET     3
#define E_OFFSET      4
#define F_OFFSET      5
#define FS_OFFSET     6
#define G_OFFSET      7
#define GS_OFFSET     8
#define A_OFFSET      9
#define AS_OFFSET    10
#define B_OFFSET     11

// define full range for chromatic scale, i.e. the basis for all other scales
#define FULL_CHROMATIC_SIZE 89

int scale_chromatic[FULL_CHROMATIC_SIZE] = { NOTE_B0,
    NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1,
    NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2,
    NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
    NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
    NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
    NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8 };


#define CHROMATIC_SIZE 12
int chromatic_offset[CHROMATIC_SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

#define DIATONIC_SIZE  7
int ionic_offset[DIATONIC_SIZE] = { 2, 2, 1, 2, 2, 2, 1};
int doric_offset[DIATONIC_SIZE] = { 2, 1, 2, 2, 2, 1, 2};
int eolyc_offset[DIATONIC_SIZE] = { 1, 2, 2, 2, 1, 2, 2};
int frygian_offset[DIATONIC_SIZE] = { 2, 2, 2, 1, 2, 2, 1};
int lydian_offset[DIATONIC_SIZE] = { 2, 2, 1, 2, 2, 1, 2};
int misolydian_offset[DIATONIC_SIZE] = { 2, 1, 2, 2, 1, 2, 2};
int corsica_offset[DIATONIC_SIZE] = { 1, 2, 2, 1, 2, 2, 2};

#define PENTATONIC_SIZE 5
int pentatonic_major[PENTATONIC_SIZE] = {2, 2, 3, 2, 3};
int pentatonic_minor[PENTATONIC_SIZE] = {3 ,2, 2, 3, 2};

#define BLUES_SIZE   6
int blues_minor[BLUES_SIZE] = {3 ,2, 1, 1, 3, 2};

#endif
