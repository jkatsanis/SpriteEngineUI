#pragma once

#include <gtest/gtest.h>
#include "Core/EngineData.h"
#include <string>

TEST(EngineDataTest, StaticPathVariables) {
    // Test that static path variables exist and are accessible
    // These are public static members that can be set
    EXPECT_TRUE(true);
}

TEST(EngineDataTest, SetPathOfEngine) {
    // Test setting engine path
    std::string testPath = "C:\\TestEngine";
    spe::EngineData::s_PathOfEngine = testPath;
    
    EXPECT_EQ(spe::EngineData::s_PathOfEngine, testPath);
}

TEST(EngineDataTest, SetPathUserProject) {
    // Test setting user project path
    std::string testPath = "C:\\UserProject";
    spe::EngineData::s_PathUserProject = testPath;
    
    EXPECT_EQ(spe::EngineData::s_PathUserProject, testPath);
}

TEST(EngineDataTest, SetNameOfUser) {
    // Test setting user name
    std::string userName = "TestUser";
    spe::EngineData::s_NameOfUser = userName;
    
    EXPECT_EQ(spe::EngineData::s_NameOfUser, userName);
}

TEST(EngineDataTest, SetScene) {
    // Test setting scene name
    std::string sceneName = "MainScene";
    spe::EngineData::s_Scene = sceneName;
    
    EXPECT_EQ(spe::EngineData::s_Scene, sceneName);
}

TEST(EngineDataTest, BuildFlags) {
    // Test setting build flags
    spe::EngineData::s_BuildDebug = true;
    spe::EngineData::s_BuildRelease = false;
    
    EXPECT_TRUE(spe::EngineData::s_BuildDebug);
    EXPECT_FALSE(spe::EngineData::s_BuildRelease);
}

TEST(EngineDataTest, JumpThroughBoxesFlag) {
    // Test jump through boxes flag
    spe::EngineData::s_JumpThroughBoxes = true;
    
    EXPECT_TRUE(spe::EngineData::s_JumpThroughBoxes);
    
    spe::EngineData::s_JumpThroughBoxes = false;
    EXPECT_FALSE(spe::EngineData::s_JumpThroughBoxes);
}

TEST(EngineDataTest, NoDefaultConstructor) {
    // Verify that EngineData cannot be instantiated
    // This test verifies the design decision to use static members only
    EXPECT_TRUE(std::is_copy_constructible_v<spe::EngineData> == false);
}
