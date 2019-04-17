#pragma once

#include "effect.h"
#include "noncopyable.h"

#include <chrono>
#include <memory>

class demo : private noncopyable
{
public:
    demo(int width, int height);
    ~demo();

    void redraw();

private:
    int width_;
    int height_;
    std::unique_ptr<effect> arrow_effect_;
    std::unique_ptr<effect> text_effect_;
    std::chrono::steady_clock::time_point start_;
};
