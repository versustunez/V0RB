#pragma once
namespace VSTZ {

class Filter {
public:
  enum class Type { LowShelf, HighShelf, Peak, NO };

  void SetFilterType(Type type);
  void SetSampleRate(double sampleRate);
  void Reset();
  double Apply(double in, int channel);
  bool IsBypassed() const { return m_Type == Type::NO; }

  void CalculateCoefficients(double gain, double frequency, double q = 0.0);

  double GetMagnitudeForFrequency(double frequency, double sampleRate) const;

  double GetSampleRate() { return m_SampleRate; }

protected:
  Type m_Type{Type::NO};
  double m_SampleRate{44100.0f};

  struct State {
    double m_xL1{0}, m_xL2{0};
    double m_yL1{0}, m_yL2{0};
  };

  State m_States[8]{};

  double m_B0{0}, m_B1{0}, m_B2{0};
  double m_A0{0}, m_A1{0}, m_A2{0};
};

} // namespace VSTZ
