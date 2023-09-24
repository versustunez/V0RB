#include "VisualBuffer.h"

namespace VSTZ {
void VisualBuffer::Insert(double value) {
  PeakBuffer[Index++] = value;
  if (Index >= 128) {
    Index = 0;
    double currentPeak = 0;
    for (double val : PeakBuffer)
      currentPeak = currentPeak < val ? val : currentPeak;
    Buffer.Insert(currentPeak);
  }
}
} // namespace VSTZ