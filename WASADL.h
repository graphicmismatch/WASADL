#include "tinyexpr.h"
#include <math.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#define VERSION "v0.1"

struct SoundData;
struct SequenceElement;
struct Sequence;
int write_wav_from_doubles(const char *, const double *, sf_count_t, int, int);
int writeSoundDataToWav(const char *, SoundData);
void sequenceToSoundData(SoundData *, Sequence seq);
