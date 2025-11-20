#include "WASADL.h"

typedef struct Sequence {
  long sequenceCount;
  long lastSampleCount;
  SequenceElement *sequenceData;
} Sequence;

typedef struct SequenceElement {
  long startSampleNumber;
  long sampleCount;
  double *data;
} SequenceElement;

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

int main() {
  printf("WASADL %s\n", VERSION);
  const char *c = "floor(sin((440*2*pi)*t))";
  long count;
  int err;
  double *r = te_evalfunc(c, 0, 10, (double)(1.0 / 44100.0), &err, &count);
  struct SoundData ss;
  ss.sampleCount = 441000;
  ss.samplerate = 44100;
  ss.channels = 1;
  ss.seconds = 10;
  ss.data = r;
  writeSoundDataToWav("test2.wav", ss);
  free(r);
  return EXIT_SUCCESS;
}

// Populates The SoundData struct.
void createSoundData(struct SoundData *out, int samplerate, double seconds,
                     int channels, long sampleCount, double *data) {
  out->samplerate = samplerate;
  out->seconds = seconds;
  out->channels = channels;
  out->sampleCount = sampleCount;
  out->data = data;
}

// Populates a SequenceElement
void createSequenceElement(struct SequenceElement *out, long sampleCount,
                           long startSampleNumber, double *data) {
  out->startSampleNumber = startSampleNumber;
  out->sampleCount = sampleCount;
  out->data = data;
}

// Converts a Sequence to SoundData
void sequenceToSoundData(SoundData *out, Sequence seq) {
  out->data = (double *)malloc(seq.lastSampleCount * sizeof(double));
  for (int i = 0; i < seq.sequenceCount; i++) {
    SequenceElement se = seq.sequenceData[i];
    for (int j = se.startSampleNumber;
         j < se.startSampleNumber + se.sampleCount; j++) {
      out->data[j] += se.data[j - se.startSampleNumber];
    }
  }
}

double getTimeFromSamples(int nsamples, int samplerate) {
  return (double)nsamples / (double)samplerate;
}

// gets the peak value
long findPeak(double *samples, int sampleCount, bool inverse) {
  long ind = 0;
  float val = samples[0];
  for (int i = 1; i < sampleCount; i++) {
    if ((!inverse && samples[i] > val) || (inverse && samples[i] < val)) {
      ind = i;
      val = samples[i];
    }
  }
  return ind;
}

int writeSoundDataToWav(const char *filename, SoundData data) {
  return write_wav_from_doubles(filename, data.data,
                                data.samplerate * data.seconds, data.channels,
                                data.samplerate);
}
int write_wav_from_doubles(const char *filename, const double *samples,
                           sf_count_t numFrames, int channels, int samplerate) {
  SF_INFO sfinfo;
  SNDFILE *file;

  sfinfo.samplerate = samplerate;
  sfinfo.frames = numFrames;
  sfinfo.channels = channels;
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; // 16-bit PCM WAV

  file = sf_open(filename, SFM_WRITE, &sfinfo);
  if (!file) {
    fprintf(stderr, "Error: %s\n", sf_strerror(NULL));
    return 1;
  }

  sf_count_t written = sf_writef_double(file, samples, numFrames);
  if (written != numFrames) {
    fprintf(stderr, "Error writing samples: %s\n", sf_strerror(file));
    sf_close(file);
    return 1;
  }

  sf_close(file);
  return 0;
}

void normalizeSound(SoundData *out, double threshold, NormalizationMode mode) {
  long peakInd = findPeak(out->data, out->sampleCount, false);
  long revPeakInd = findPeak(out->data, out->sampleCount, true);
  double peak = abs((out->data)[peakInd]);
  double revPeak = abs((out->data)[revPeakInd]);
  long truePeakIndex = peakInd;
  if (revPeak > peak) {
    truePeakIndex = revPeakInd;
  }
}

void peaksNormalization(SoundData *out, double threshold, long peakIndex) {
  double peakVal = out->data[peakIndex] if (peakVal > 0.0d) {
    double scale = threshold / peakVal;
    for (int i = 0; i < (out->sampleCount); i++) {
      out->data[i] *= scale;
    }
  }
}
void LUFSNormalization() {}
void RMSNormalization() {}
void hardClippingNormalization() {}
void tanh_softClippingNormalization() {}
void cubic_softClippingNormalization() {}
