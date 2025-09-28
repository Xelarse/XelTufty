#include "pico/stdlib.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include "pico/time.h"
#include "pico/platform.h"

#include "common/pimoroni_common.hpp"
#include "drivers/st7789/st7789.hpp"
#include "drivers/button/button.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/tufty2040/tufty2040.hpp"
#include "libraries/pngdec/PNGdec.h"
#include "include/images/bearIconPngSmall.h"
#include "include/images/bearIconPng.h"
#include "include/images/octocat.h"
#include "include/images/octocat_8bpp.h"


using namespace pimoroni;

// Tufty is 320 x 240 with 133mhz cpu with 264kb of sram
// input range is 3v - 5.5v

Tufty2040 tufty;
PNG png;

ST7789 st7789(
  Tufty2040::WIDTH,
  Tufty2040::HEIGHT,
  ROTATE_0,
  ParallelPins{
    Tufty2040::LCD_CS,
    Tufty2040::LCD_DC,
    Tufty2040::LCD_WR,
    Tufty2040::LCD_RD,
    Tufty2040::LCD_D0, 
    Tufty2040::BACKLIGHT
  }
);

struct shapePoint {
  float      x;
  float      y;
  uint8_t    r;
  float     dx;
  float     dy;
  uint16_t pen;
};

struct textRenderInfo {
  std::string text = "";
  Point pos{310,100};
  int wrapPixelCount = 100;
  float scale = 6.0f;
  float angle = 180.f;
  uint8_t spacing = 1;
};

struct imageRenderInfo {
  int x;
  int y;
};

PicoGraphics_PenRGB556 graphics(st7789.width, st7789.height, nullptr);
Pen bearBasePen = graphics.create_pen(254, 246, 210);
Pen bearFleshPen = graphics.create_pen(246, 216, 190);
Pen bearNosePen = graphics.create_pen(186, 145, 99);
Pen bearDecalLightPen = graphics.create_pen(252, 224, 138);
Pen bearDecalDarkPen = graphics.create_pen(232, 188, 141);
Pen blackPen = graphics.create_pen(0, 0, 0);

Button button_a(Tufty2040::A, Polarity::ACTIVE_HIGH);
Button button_b(Tufty2040::B, Polarity::ACTIVE_HIGH);
Button button_c(Tufty2040::C, Polarity::ACTIVE_HIGH);
Button button_up(Tufty2040::UP, Polarity::ACTIVE_HIGH);
Button button_down(Tufty2040::DOWN, Polarity::ACTIVE_HIGH);

uint32_t time() {
  absolute_time_t t = get_absolute_time();
  return to_ms_since_boot(t);
}

auto getShapes() -> std::vector<shapePoint> {
  const uint8_t shapeCount = 100;
  std::vector<shapePoint> out;
  out.reserve(shapeCount);

  shapePoint shape{};
  for(size_t i = 0; i < shapeCount; ++i) {
    shape.x = rand() % graphics.bounds.w;
    shape.y = rand() % graphics.bounds.h;
    shape.r = (rand() % 10) + 3;
    shape.dx = float(rand() % 255) / 64.0f;
    shape.dy = float(rand() % 255) / 64.0f;
    shape.pen = graphics.create_pen(rand() % 255, rand() % 255, rand() % 255);
    out.emplace_back(shape);
  }

  return std::move(out);
}

auto shapesDemo(PicoGraphics_PenRGB556& graphics) -> void {
  static std::vector<shapePoint> shapes = getShapes();

  for(auto& shape : shapes) {
    shape.x += shape.dx;
    shape.y += shape.dy;
    if((shape.x - shape.r) < 0) {
      shape.dx *= -1;
      shape.x = shape.r;
    }
    if((shape.x + shape.r) >= graphics.bounds.w) {
      shape.dx *= -1;
      shape.x = graphics.bounds.w - shape.r;
    }
    if((shape.y - shape.r) < 0) {
      shape.dy *= -1;
      shape.y = shape.r;
    }
    if((shape.y + shape.r) >= graphics.bounds.h) {
      shape.dy *= -1;
      shape.y = graphics.bounds.h - shape.r;
    }

    graphics.set_pen(shape.pen);
    graphics.circle(Point(shape.x, shape.y), shape.r);
  }
}

auto centerText(PicoGraphics_PenRGB556& graphics, textRenderInfo& textRenderInfo, int scrWidth) -> void {
  // FIXME: rendering this isnt working as expected cout the values and check tomoz, maybe hersshey needs something special?
  auto textWidth = graphics.measure_text(textRenderInfo.text, textRenderInfo.scale, textRenderInfo.spacing, false);
  textRenderInfo.pos.x = (scrWidth * 0.5f) - (textWidth * 0.5f);
}

auto drawBackground(PicoGraphics_PenRGB556& graphics) -> void {
    graphics.set_pen(bearFleshPen);
    graphics.clear();


    int screenWidth = graphics.bounds.w;
    int screenHeight = graphics.bounds.h;

    graphics.set_pen(bearNosePen);

    // Middle Base
    graphics.circle(Point{screenWidth/2, -40}, 100);

    // Side beans
    graphics.circle(Point{screenWidth/2 - (screenWidth/4 + 20), 80}, 50);
    graphics.circle(Point{screenWidth/2 + (screenWidth/4 + 20), 80}, 50);

    // upper Bean
    graphics.circle(Point{screenWidth/2, 120}, 50);
}

auto pngDrawCallback(PNGDRAW* pDraw) -> int {
  imageRenderInfo* pngInfo = reinterpret_cast<imageRenderInfo*>(pDraw->pUser);

  std::vector<uint16_t> lineBuffer;
  lineBuffer.resize(pDraw->iWidth * sizeof(uint16_t));
  png.getLineAsRGB565(pDraw, lineBuffer.data(), PNG_RGB565_BIG_ENDIAN, 0xffffffff);

  uint16_t* currentPixel = lineBuffer.data();
  for(int x = 0; x < pDraw->iWidth; ++x) {
    // auto rgbVal = RGB(RGB565(*pixel));
    auto rgbVal = RGB565(*currentPixel);
    graphics.set_pixel_dither(Point{pngInfo->x + x, pngInfo->y + pngInfo->y + pDraw->y}, rgbVal);
    ++currentPixel;
  }
  
  return 1; // For some reason returning a PNG_SUCCESS causes the library to EARLY quit, lowkey dumb but changed to allow for processing of this func
}

template<class Resolution, class Step>
auto timestampDifference(const std::chrono::steady_clock::time_point& before, const std::chrono::steady_clock::time_point& after) -> double {
  std::chrono::duration<Resolution, Step> delta{after - before};
  return delta.count();
}

int main() {
  stdio_init_all();
  st7789.set_backlight(255);
  graphics.set_font("bitmap8");
  auto pngInfo = imageRenderInfo{0,0};

  int pngOpen = png.openRAM(bigBeawIconPng, sizeof(bigBeawIconPng), pngDrawCallback);

  bool needsRedraw = true;
  while(true) {
    const auto startTs{std::chrono::steady_clock::now()};
    auto processingTs{std::chrono::steady_clock::now()};
    auto renderTs{std::chrono::steady_clock::now()};

    if (needsRedraw) {
      // drawBackground(graphics);
      textRenderInfo tInfo{.text = "Xels Tufty"};
      graphics.set_pen(blackPen);
      graphics.text(tInfo.text, tInfo.pos, tInfo.wrapPixelCount, tInfo.scale, tInfo.angle, tInfo.spacing);

      if (pngOpen == PNG_SUCCESS) {
        png.decode(&pngInfo, 0);
      }
      processingTs = std::chrono::steady_clock::now();
      // update screen
      st7789.update(&graphics);
      renderTs = std::chrono::steady_clock::now();
      needsRedraw = false;
    }

    // FIXME: This borks console output, math is mathin but cant prove it out until I have moving stuff
    //Force a 1 fps limit to reduce battery drain
    // static const double forcedMinWaitMs{100e6};
    // auto delta = forcedMinWaitMs - (renderTs - startTs).count();
    // if (delta > 0.0) {
    //   // using namespace std::chrono_literals;
    //   // std::this_thread::sleep_for(delta * 1ms);
    //   busy_wait_ms(delta);
    // }
    // auto sleepTs{std::chrono::steady_clock::now()};

    std::cout << 
    "Compute: "   << timestampDifference<double, std::milli>(startTs, processingTs) << 
    "ms Render: " << timestampDifference<double, std::milli>(processingTs, renderTs) << 
    // "ms Sleep: "  << timestampDifference<double, std::milli>(renderTs, sleepTs) <<
    "ms Total: "  << timestampDifference<double, std::milli>(startTs, renderTs) << "ms" << std::endl;
  }

  png.close();
  return 0;
}
