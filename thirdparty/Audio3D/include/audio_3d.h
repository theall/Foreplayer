#ifndef AUDIO_3D_SOURCE_H_
#define AUDIO_3D_SOURCE_H_

#include <cstdint>
#include <vector>
class FFTFilter;
class HRTF;
class Reberation;

class Audio3DSource {
 public:
  Audio3DSource(int sample_rate, int block_size);
  virtual ~Audio3DSource();

  void SetPosition(int x, int y, int z);
  void SetDirection(float elevation_deg, float azimuth_deg, float distance);

  void ProcessBlock(const std::vector<float>&input,
                    std::vector<float>* output_left,
                    std::vector<float>* output_right);
 private:
  void CalculateXFadeWindow();
  void ApplyXFadeWindow(const std::vector<float>& block_a,
                        const std::vector<float>& block_b,
                        std::vector<float>* output);

  static void ApplyDamping(float damping_factor, std::vector<float>* block);
  const int sample_rate_;
  const int block_size_;
  float elevation_deg_;
  float azimuth_deg_;
  float distance_;

  float damping_;

  std::vector<float> xfade_window_;
  std::vector<float> prev_signal_block_;

  HRTF* hrtf_;
  FFTFilter* left_hrtf_filter_;
  FFTFilter* right_hrtf_filter_;

  Reberation* reberation_;
};

#endif
