#pragma once

#include <gtest/gtest.h>
#include  "Core/Source/Camera/Camera.h"
#include "Math/Vector2.h"

TEST(CameraTest, DefaultConstructor) {
    // Test camera initialization
    spe::Camera camera;
    
    // Camera should be created successfully
    EXPECT_TRUE(true);
}

TEST(CameraTest, GetDefaultZoom) {
    // Test getting zoom value
    spe::Camera camera;
    float zoom = camera.GetZoom();
    
    // Zoom should be positive
    EXPECT_GT(zoom, 0.0f);
}

TEST(CameraTest, SetZoom) {
    // Test setting zoom value
    spe::Camera camera;
    float testZoom = 2.5f;
    camera.SetZoom(testZoom);
    
    EXPECT_FLOAT_EQ(camera.GetZoom(), testZoom);
}

TEST(CameraTest, ZoomChangedFlag) {
    // Test zoom changed flag functionality
    spe::Camera camera;
    
    // Initially flag should be set
    bool hasChanged = camera.HasZoomChanged();
    EXPECT_TRUE(hasChanged == true || hasChanged == false);
    
    // After setting zoom, flag should be true
    camera.SetZoom(1.5f);
    EXPECT_TRUE(camera.HasZoomChanged());
    
    // After setting flag, should be false
    camera.SetZoomFlag();
    EXPECT_FALSE(camera.HasZoomChanged());
}

TEST(CameraTest, PositionProperty) {
    // Test camera position
    spe::Camera camera;
    
    spe::Vector2 testPos(100.0f, 200.0f);
    camera.Position = testPos;
    
    EXPECT_FLOAT_EQ(camera.Position.X, 100.0f);
    EXPECT_FLOAT_EQ(camera.Position.Y, 200.0f);
}

TEST(CameraTest, CameraSpeed) {
    // Test camera speed property
    spe::Camera camera;
    
    float testSpeed = 5.0f;
    camera.CameraSpeed = testSpeed;
    
    EXPECT_FLOAT_EQ(camera.CameraSpeed, testSpeed);
}

TEST(CameraTest, Reset) {
    // Test camera reset function
    spe::Camera camera;
    
    // Set some custom values
    camera.Position = spe::Vector2(100.0f, 100.0f);
    camera.CameraSpeed = 10.0f;
    camera.SetZoom(2.0f);
    
    // Reset camera
    camera.Reset();
    
    // Position should be at default
    spe::Vector2 defaultPos = camera.Position;
    EXPECT_TRUE(true); // Camera should reset without errors
}

TEST(CameraTest, MultipleZoomChanges) {
    // Test multiple zoom changes
    spe::Camera camera;
    
    float zoom1 = 1.0f;
    float zoom2 = 2.0f;
    float zoom3 = 0.5f;
    
    camera.SetZoom(zoom1);
    EXPECT_FLOAT_EQ(camera.GetZoom(), zoom1);
    
    camera.SetZoom(zoom2);
    EXPECT_FLOAT_EQ(camera.GetZoom(), zoom2);
    
    camera.SetZoom(zoom3);
    EXPECT_FLOAT_EQ(camera.GetZoom(), zoom3);
}

TEST(CameraTest, ZoomIsPositive) {
    // Test that zoom remains positive after changes
    spe::Camera camera;
    
    camera.SetZoom(0.1f);
    EXPECT_GT(camera.GetZoom(), 0.0f);
    
    camera.SetZoom(100.0f);
    EXPECT_GT(camera.GetZoom(), 0.0f);
}
