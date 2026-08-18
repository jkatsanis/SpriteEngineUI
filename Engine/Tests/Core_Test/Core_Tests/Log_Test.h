#pragma once

#include <gtest/gtest.h>
#include "Core/Log.h"
#include <sstream>
#include <string>

TEST(LogTest, LogStringBufferInitialization) {
    // Test that log buffer can be initialized
    std::string testBuffer = "";
    spe::Log::SetStringBuffer(&testBuffer);
    // Buffer should be set without crashing
    EXPECT_TRUE(true);
}

TEST(LogTest, LogStringAppending) {
    // Test that LogString appends to buffer
    std::string testBuffer = "";
    spe::Log::SetStringBuffer(&testBuffer);
    
    std::string testMessage = "Test message";
    spe::Log::LogString(testMessage);
    
    // Buffer should contain the logged message
    EXPECT_NE(testBuffer.find(testMessage), std::string::npos);
}

TEST(LogTest, MultipleLogStrings) {
    // Test that multiple log strings are accumulated
    std::string testBuffer = "";
    spe::Log::SetStringBuffer(&testBuffer);
    
    spe::Log::LogString("First message");
    spe::Log::LogString("Second message");
    
    // Both messages should be in the buffer
    EXPECT_NE(testBuffer.find("First message"), std::string::npos);
    EXPECT_NE(testBuffer.find("Second message"), std::string::npos);
}

TEST(LogTest, LogEmptyString) {
    // Test logging empty string doesn't crash
    std::string testBuffer = "";
    spe::Log::SetStringBuffer(&testBuffer);
    
    spe::Log::LogString("");
    
    EXPECT_TRUE(true);
}

TEST(LogTest, LogSpecialCharacters) {
    // Test logging strings with special characters
    std::string testBuffer = "";
    spe::Log::SetStringBuffer(&testBuffer);
    
    std::string specialMessage = "Test!@#$%^&*()";
    spe::Log::LogString(specialMessage);
    
    EXPECT_NE(testBuffer.find(specialMessage), std::string::npos);
}
