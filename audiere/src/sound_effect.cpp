#include <vector>
#include "internal.h"


namespace audiere {

  class SingleSoundEffect : public RefImplementation<SoundEffect> {
  public:
    SingleSoundEffect(OutputStream* os) {
      m_stream = os;
    }

    void ADR_CALL play() {
      m_stream->reset();
      m_stream->play();
    }

    void ADR_CALL stop() {
      m_stream->stop();
      m_stream->reset();
    }

    void ADR_CALL setVolume(float volume) {
      m_stream->setVolume(volume);
    }

    float ADR_CALL getVolume() {
      return m_stream->getVolume();
    }

    void ADR_CALL setPan(float pan) {
      m_stream->setPan(pan);
    }

    float ADR_CALL getPan() {
      return m_stream->getPan();
    }

    void ADR_CALL setPitchShift(float shift) {
      m_stream->setPitchShift(shift);
    }

    float ADR_CALL getPitchShift() {
      return m_stream->getPitchShift();
    }

  private:
    RefPtr<OutputStream> m_stream;
  };


  class MultipleSoundEffect : public RefImplementation<SoundEffect> {
  public:
    MultipleSoundEffect(AudioDevice* device, SampleBuffer* sb) {
      m_device = device;
      m_buffer = sb;

      m_volume = 1;
      m_pan = 0;
      m_shift = 1;
    }

    void ADR_CALL play() {
      // open the stream and play it
      OutputStream* stream = m_device->openStream(m_buffer->openStream());
      if (!stream) {
        return;
      }
      stream->setVolume(m_volume);
      stream->setPan(m_pan);
      stream->setPitchShift(m_shift);
      stream->play();

      // go through the list and see if any streams are done playing
      // so we can reuse them
      for (unsigned i = 0; i < m_streams.size(); ++i) {
        if (!m_streams[i]->isPlaying()) {
          m_streams[i] = stream;
          return;
        }
      }

      m_streams.push_back(stream);
    }

    void ADR_CALL stop() {
      m_streams.clear();
    }

    void ADR_CALL setVolume(float volume) {
      m_volume = volume;
    }

    float ADR_CALL getVolume() {
      return m_volume;
    }

    void ADR_CALL setPan(float pan) {
      m_pan = pan;
    }

    float ADR_CALL getPan() {
      return m_pan;
    }

    void ADR_CALL setPitchShift(float shift) {
      m_shift = shift;
    }

    float ADR_CALL getPitchShift() {
      return m_shift;
    }
    
  private:
    RefPtr<AudioDevice> m_device;
    RefPtr<SampleBuffer> m_buffer;
    std::vector<RefPtr<OutputStream> > m_streams;

    float m_volume;
    float m_pan;
    float m_shift;
  };


  ADR_EXPORT(SoundEffect*, AdrOpenSoundEffect)(
    AudioDevice* device,
    SampleSource* source,
    SoundEffectType type)
  {
    if (!device || !source) {
      return 0;
    }

    switch (type) {
      case SINGLE: {
        OutputStream* os = OpenSound(device, source, true);
        return (os ? new SingleSoundEffect(os) : 0);
      }
        
      case MULTIPLE: {
        SampleBuffer* sb = CreateSampleBuffer(source);
        return (sb ? new MultipleSoundEffect(device, sb) : 0);
      }

      default:
        return 0;
    }
  }

}
