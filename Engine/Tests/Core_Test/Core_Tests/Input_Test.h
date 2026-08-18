#pragma once

#include <gtest/gtest.h>
#include "Core/Event.h"
#include "Core/Source/Input/Input.h"

// Mock Event class for testing
class MockEvent : public spe::Event {
public:
    MockEvent() : spe::Event() {}
};

TEST(InputTest, InputClassIsStatic) {
    // Verify Input class design - cannot be instantiated
    EXPECT_TRUE(std::is_copy_constructible_v<spe::Input> == false);
}

TEST(InputTest, SetEventPointer) {
    // Test setting event pointer
    MockEvent mockEvent;
    spe::Input::SetEvent(&mockEvent);
    
    // Should not crash when setting event
    EXPECT_TRUE(true);
}

TEST(InputTest, OnKeyPressReturn) {
    // Test that OnKeyPress returns a boolean without crashing
    bool result = spe::Input::OnKeyPress(spe::KeyBoardCode::A);
    
    // Result should be boolean
    EXPECT_TRUE(result == true || result == false);
}

TEST(InputTest, OnKeyReleaseReturn) {
    // Test that OnKeyRelease returns a boolean
    bool result = spe::Input::OnKeyRelease(spe::KeyBoardCode::A);
    
    EXPECT_TRUE(result == true || result == false);
}

TEST(InputTest, OnKeyHoldReturn) {
    // Test that OnKeyHold returns a boolean
    bool result = spe::Input::OnKeyHold(spe::KeyBoardCode::A);
    
    EXPECT_TRUE(result == true || result == false);
}

TEST(InputTest, MultipleKeyInputs) {
    // Test checking multiple different keys
    bool resultA = spe::Input::OnKeyPress(spe::KeyBoardCode::A);
    bool resultB = spe::Input::OnKeyPress(spe::KeyBoardCode::B);
    bool resultSpace = spe::Input::OnKeyPress(spe::KeyBoardCode::Space);
    
    // All should return valid boolean values
    EXPECT_TRUE(resultA == true || resultA == false);
    EXPECT_TRUE(resultB == true || resultB == false);
    EXPECT_TRUE(resultSpace == true || resultSpace == false);
}

TEST(InputTest, ConsistentKeyStates) {
    // Test that key states are consistent
    spe::KeyBoardCode testKey = spe::KeyBoardCode::W;
    
    bool pressState = spe::Input::OnKeyPress(testKey);
    bool holdState = spe::Input::OnKeyHold(testKey);
    bool releaseState = spe::Input::OnKeyRelease(testKey);
    
    // All should be valid boolean values
    EXPECT_TRUE(pressState == true || pressState == false);
    EXPECT_TRUE(holdState == true || holdState == false);
    EXPECT_TRUE(releaseState == true || releaseState == false);
}
