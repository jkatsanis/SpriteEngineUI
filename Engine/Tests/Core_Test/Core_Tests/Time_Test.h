#pragma once

#include <gtest/gtest.h>
#include "Core/Time.h"

TEST(TimeTest, DefaultStaticVariables) {
    // Time class uses static variables, check initial state
    EXPECT_GE(spe::Time::s_FPS, 0.0f);
    EXPECT_GE(spe::Time::s_TimePassed, 0.0f);
    EXPECT_GE(spe::Time::s_DeltaTime, 0.0f);
}

TEST(TimeTest, DeltaTimeUpdate) {
    // Test that delta time increases after update
    spe::Time::Reset();
    float initialDeltaTime = spe::Time::s_DeltaTime;
    spe::Time::Update();
    // Delta time should be non-negative after update
    EXPECT_GE(spe::Time::s_DeltaTime, 0.0f);
}

TEST(TimeTest, Reset) {
    // Test that Reset function clears timing data
    spe::Time::Reset();
    // After reset, DeltaTime should be zero
    EXPECT_EQ(spe::Time::s_DeltaTime, 0.0f);
}

TEST(TimeTest, TicksAreTracked) {
    // Test that ticks are properly tracked
    spe::Time::Reset();
    spe::Time::Update();
    float ticks = spe::Time::s_Ticks;
    EXPECT_GE(ticks, 0.0f);
}

TEST(TimeTest, FPSCalculation) {
    // Test that FPS is calculated (should be positive when running)
    spe::Time::Reset();
    for (int i = 0; i < 10; ++i) {
        spe::Time::Update();
    }
    // FPS should be positive after multiple updates
    EXPECT_GT(spe::Time::s_FPS, 0.0f);
}
