#pragma once

#include <gtest/gtest.h>
#include "Sprite/Components/Animator/Animator.h"
#include "Sprite/Sprite.h"
#include "Sprite/Components/Light/LightRepository.h"

class AnimatorTest : public ::testing::Test {
protected:
    spe::LightRepository lightRepo;
    std::unique_ptr<spe::Sprite> sprite;
    std::unique_ptr<spe::Animator> animator;

    std::string testResourcePath = "Tests/Core_Test/TestResources/test.png";
    std::string animName = "Walk";

    void SetUp() override {
        sprite = std::make_unique<spe::Sprite>("TestSprite", spe::Vector2(0,0), testResourcePath, lightRepo);

        animator = std::make_unique<spe::Animator>(sprite.get());
        animator->Exist = true;
    }

    void TearDown() override {
        animator.reset();
        sprite.reset();
    }

    void CreateDummyAnimation(const std::string& name) {
        std::vector<spe::KeyFrame> frames;
        // add at least one frame so its a valid animation
        spe::KeyFrame frame;
        frame.path = testResourcePath;
        frame.delay = 100.0f;
        frames.push_back(frame);

        animator->CreateAnimation(name, testResourcePath, frames);
    }
};

TEST_F(AnimatorTest, DefaultConstructor) {
    spe::Animator defaultAnim;
    EXPECT_FALSE(defaultAnim.Exist);
    EXPECT_TRUE(defaultAnim.Animations.empty());
    EXPECT_EQ(defaultAnim.GetAnimationPlaying().Name, "<Unknown>");
    EXPECT_FALSE(defaultAnim.GetAnimationPlaying().IsPlaying);
}

TEST_F(AnimatorTest, CreateAndRemoveAnimation) {
    CreateDummyAnimation(animName);

    EXPECT_FALSE(animator->Animations.empty());
    EXPECT_NE(animator->Animations.find(animName), animator->Animations.end());

    animator->RemoveAnimation(animName);
    EXPECT_TRUE(animator->Animations.empty());
}

TEST_F(AnimatorTest, RemoveNonExistentAnimation) {
    // should not crash
    EXPECT_NO_THROW(animator->RemoveAnimation("NonExistent"));
}

TEST_F(AnimatorTest, PlayAndStop) {
    CreateDummyAnimation(animName);

    animator->Play(animName);
    EXPECT_EQ(animator->GetAnimationPlaying().Name, animName);
    EXPECT_TRUE(animator->GetAnimationPlaying().IsPlaying);

    animator->Stop(animName);
    EXPECT_FALSE(animator->GetAnimationPlaying().IsPlaying);
    EXPECT_EQ(animator->GetAnimationPlaying().Name, "<Unknown>");
}

TEST_F(AnimatorTest, PlayNonExistent) {
    // should not crash and state should remain unchanged
    animator->Play("GhostAnim");
    EXPECT_FALSE(animator->GetAnimationPlaying().IsPlaying);
    EXPECT_EQ(animator->GetAnimationPlaying().Name, "<Unknown>");
}

TEST_F(AnimatorTest, SetName) {
    CreateDummyAnimation("OldName");

    animator->SetName("NewName", "OldName");

    EXPECT_EQ(animator->Animations.find("OldName"), animator->Animations.end());
    EXPECT_NE(animator->Animations.find("NewName"), animator->Animations.end());
    EXPECT_EQ(animator->Animations["NewName"].GetName(), "NewName");
}

TEST_F(AnimatorTest, SetNameNonExistent) {
    // should do nothing and not crash
    animator->SetName("NewName", "GhostAnim");
    EXPECT_TRUE(animator->Animations.empty());
}

TEST_F(AnimatorTest, UpdateLogic) {
    CreateDummyAnimation(animName);
    animator->Play(animName);

    // no way to mock time progression since deltatime is static/global, just verify it doesnt crash
    EXPECT_NO_THROW(animator->Update());
}

TEST_F(AnimatorTest, Reset) {
    CreateDummyAnimation(animName);
    animator->Play(animName);

    animator->Reset();

    EXPECT_TRUE(animator->Animations.empty());
    EXPECT_FALSE(animator->GetAnimationPlaying().IsPlaying);
    EXPECT_EQ(animator->GetAnimationPlaying().Name, "<Unknown>");
}
