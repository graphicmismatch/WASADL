#pragma once
#ifndef WASADL_H
#define WASADL_H

#include "sndfile.h"
#include "lib/tinyexpr/tinyexpr.h"

#define VERSION "v0.1"

typedef struct SequenceElement {
  long startSampleNumber;
  long sampleCount;
  te_real *data;
} SequenceElement;

typedef struct Sequence {
  long sequenceCount;
  long lastSampleCount;
  SequenceElement *sequenceData;
} Sequence;

typedef struct SoundData {
  int samplerate;
  double seconds;
  int channels;
  long sampleCount;
  double *data;
} SoundData;

typedef enum NormalizationMode {
  PEAKS,
  LUFS,
  RMS,
  HARD_CLIPPING,
  SOFT_CLIPPING_TANH,
  SOFT_CLIPPING_CUBIC
} NormalizationMode;

void createSoundData(SoundData *out, int samplerate, double seconds,
                     int channels, long sampleCount, double *data);

void createSequenceElement(SequenceElement *out, long sampleCount,
                           long startSampleNumber, te_real *data);

void sequenceToSoundData(Sequence *seq, te_real *out_buffer);

te_real getTimeFromSamples(long nsamples, int samplerate);

long findPeakIndex(const te_real *samples, long sampleCount, int inverse);

void normalizeSound(te_real *samples, long sampleCount, te_real threshold,
                    NormalizationMode mode);

int write_wav_from_doubles(const char *filename, const double *samples,
                           sf_count_t numFrames, int channels, int samplerate);

int writeSoundDataToWav(const char *filename, SoundData data);
void peaksNormalization(te_real *samples, long sampleCount, te_real threshold,
                        long peakIndex);
void LUFSNormalization(te_real *samples, long sampleCount, te_real threshold);
void RMSNormalization(te_real *samples, long sampleCount, te_real threshold);
void hardClippingNormalization(te_real *samples, long sampleCount,
                               te_real threshold);
void tanh_softClippingNormalization(te_real *samples, long sampleCount,
                                    te_real threshold);
void cubic_softClippingNormalization(te_real *samples, long sampleCount,
                                     te_real threshold);

#endif
