#ifndef DEVICE_NULL_H
#define DEVICE_NULL_H


#include <set>
#include "audiere.h"
#include "internal.h"
#include "repeatable.h"
#include "threads.h"
#include "types.h"
#include "utility.h"


namespace audiere {

  class NullOutputStream;

  class NullAudioDevice
    : public RefImplementation<AudioDevice>
    , public Synchronized
  {
  public:
    static NullAudioDevice* create(const ParameterList& parameters);

  private:
    NullAudioDevice();
    ~NullAudioDevice();

  public:
    void ADR_CALL update();
    OutputStream* ADR_CALL openStream(SampleSource* source);
    OutputStream* ADR_CALL openBuffer(
      void* samples, int sample_count,
      int channel_count, int sample_rate, SampleFormat sample_format);

  private:
    void removeStream(NullOutputStream* stream);

    typedef std::set<NullOutputStream*> StreamList;
    StreamList m_streams;

    friend class NullOutputStream;
  };

  class NullOutputStream : public RefImplementation<OutputStream> {
  private:
    NullOutputStream(NullAudioDevice* device, SampleSource* source);
    ~NullOutputStream();

  public:
    void ADR_CALL play();
    void ADR_CALL stop();
    void ADR_CALL reset();
    bool ADR_CALL isPlaying();

    void ADR_CALL setRepeat(bool repeat);
    bool ADR_CALL getRepeat();

    void  ADR_CALL setVolume(float volume);
    float ADR_CALL getVolume();

    void  ADR_CALL setPan(float pan);
    float ADR_CALL getPan();

    bool ADR_CALL isSeekable();
    int  ADR_CALL getLength();
    void ADR_CALL setPosition(int position);
    int  ADR_CALL getPosition();

  private:
    void resetTimer();
    void update();
    int dummyRead(int samples_to_read);

    RefPtr<NullAudioDevice> m_device;
                          
    RepeatableStream* m_source;
    int m_channel_count;           //
    int m_sample_rate;             // cached stream format
    SampleFormat m_sample_format;  //

    bool m_is_playing;
    float m_volume;
    float m_pan;

    u64 m_last_update;

    friend class NullAudioDevice;
  };

}


#endif
