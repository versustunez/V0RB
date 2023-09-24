#include "Logo.h"

namespace VSTZ::Editor {
static std::string LogoData = R"(
<svg xmlns="http://www.w3.org/2000/svg" width="936" height="188" version="1.0" viewBox="0 0 702 141"><path fill="#ffffff" d="M2 7.5C2 8.5 60.5 125 61 125c.6 0 58-116.1 58-117.3 0-.4-7.2-.7-16-.7H87L74.3 32.5c-7 14-13 25.5-13.3 25.5-.3 0-6.3-11.5-13.3-25.5L35 7H18.5C9.4 7 2 7.2 2 7.5zM151 9.6c-7.1 3.2-14.4 10.4-18.1 17.6l-2.9 5.7v51.2l2.9 5.7c3.6 6.9 10.8 14.1 17.8 17.6l5.2 2.6h61.2l5.7-2.9c6.9-3.6 14.1-10.8 17.6-17.8 2.6-5.2 2.6-5.3 2.6-30.8 0-24-.1-25.8-2.1-29.8-3.1-6.3-13.5-16.4-19.8-19.2-5.3-2.5-5.6-2.5-35-2.5h-29.6L151 9.6zm59.8 29.2c.9.7 1.2 5.4 1 20.3l-.3 19.4h-50l-.3-19c-.1-10.4 0-19.5.2-20.2.4-1 5.9-1.3 24.3-1.3 13.1 0 24.4.4 25.1.8z"/><path fill="#3489fb" d="M176 58.5V69h21.1l-.3-10.3-.3-10.2-10.2-.3-10.3-.3v10.6zM253 7.5c0 .3 3.4 7.3 7.5 15.5l7.5 15h32.3c17.7 0 32.8.4 33.6.9.8.5 1.1 1.6.8 2.5-.6 1.4-4.9 1.6-41.2 1.6H253v41c0 22.6.2 41 .5 41s7.3-3.4 15.5-7.5l15-7.5V74h17.5l9 18 9 18h17.7c9.8 0 17.8-.2 17.8-.5s-3.8-8.2-8.5-17.5-8.5-17.2-8.5-17.5c0-.3.7-.5 1.5-.5 6 0 19.3-11.5 23.9-20.7 3.5-6.9 3.7-18.2.5-24.6-3.1-6.3-13.5-16.4-19.8-19.2L338.8 7h-42.9c-23.6 0-42.9.2-42.9.5zM376 7.5c0 .3 3.4 7.3 7.5 15.5l7.5 15h32.3c17.7 0 32.8.4 33.6.9.8.5 1.1 1.6.8 2.5-.6 1.4-4.9 1.6-41.2 1.6H376v67h87.1l5.2-2.6c7-3.5 14.2-10.7 17.8-17.6 4-7.9 4.1-18.2.3-26.1l-2.6-5.3 2.6-5.2c3.5-6.8 3.7-18.1.5-24.5-3.1-6.3-13.5-16.4-19.8-19.2L461.8 7h-42.9c-23.6 0-42.9.2-42.9.5zm81.5 69c0 1.9-.8 2-25.2 2.3L407 79v-5l25.3.2c24.4.3 25.2.4 25.2 2.3z"/></svg>)";
Logo::Logo(VSTZ::InstanceID id) : m_ID(id) {
  m_Image =
      juce::Drawable::createFromImageData(LogoData.c_str(), LogoData.size());
}
void Logo::paint(juce::Graphics &g) {
  if (m_Image) {
    m_Image->draw(g, 1.0f);
  }
}

void Logo::resized() {
  juce::RectanglePlacement placement = juce::RectanglePlacement::centred;
  juce::Rectangle<float> react{0, 0, (float)getWidth(), (float)getHeight()};
  if (m_Image == nullptr) {
    return;
  }
  auto bounds = m_Image->getDrawableBounds();
  if (react.getWidth() == 0 || react.getHeight() == 0 ||
      bounds.getWidth() == 0 || bounds.getHeight() == 0) {
    return;
  }
  auto transform = placement.getTransformToFit(bounds, react);
  m_Image->setTransform(transform);
}
} // namespace VSTZ::Editor