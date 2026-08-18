#pragma once

#include <gtest/gtest.h>
#include "Math/Vector2.h"

TEST(Vector2Test, DefaultConstructor) {
    spe::Vector2 v;
    EXPECT_FLOAT_EQ(v.X, 0.0f);
    EXPECT_FLOAT_EQ(v.Y, 0.0f);
}

TEST(Vector2Test, ParameterizedConstructor) {
    spe::Vector2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.X, 3.0f);
    EXPECT_FLOAT_EQ(v.Y, 4.0f);
}

TEST(Vector2Test, EqualityOperator) {
    spe::Vector2 v1(1.0f, 2.0f);
    spe::Vector2 v2(1.0f, 2.0f);
    spe::Vector2 v3(2.0f, 3.0f);
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

TEST(Vector2Test, InequalityOperator) {
    spe::Vector2 v1(1.0f, 2.0f);
    spe::Vector2 v2(2.0f, 3.0f);
    EXPECT_TRUE(v1 != v2);
}

TEST(Vector2Test, AdditionOperator) {
    spe::Vector2 v1(1.0f, 2.0f);
    spe::Vector2 v2(3.0f, 4.0f);
    spe::Vector2 result = v1 + v2;
    EXPECT_FLOAT_EQ(result.X, 4.0f);
    EXPECT_FLOAT_EQ(result.Y, 6.0f);
}

TEST(Vector2Test, SubtractionOperator) {
    spe::Vector2 v1(5.0f, 6.0f);
    spe::Vector2 v2(3.0f, 2.0f);
    spe::Vector2 result = v1 - v2;
    EXPECT_FLOAT_EQ(result.X, 2.0f);
    EXPECT_FLOAT_EQ(result.Y, 4.0f);
}

TEST(Vector2Test, MultiplicationByScalar) {
    spe::Vector2 v(2.0f, 3.0f);
    spe::Vector2 result = v * 2.0f;
    EXPECT_FLOAT_EQ(result.X, 4.0f);
    EXPECT_FLOAT_EQ(result.Y, 6.0f);
}

TEST(Vector2Test, MultiplicationByVector) {
    spe::Vector2 v1(2.0f, 3.0f);
    spe::Vector2 v2(4.0f, 5.0f);
    spe::Vector2 result = v1 * v2;
    EXPECT_FLOAT_EQ(result.X, 8.0f);
    EXPECT_FLOAT_EQ(result.Y, 15.0f);
}

TEST(Vector2Test, LengthCalculation) {
    spe::Vector2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.Length(), 5.0f);
}

TEST(Vector2Test, Normalize) {
    spe::Vector2 v(3.0f, 4.0f);
    v.Normalize();
    EXPECT_NEAR(v.X, 0.6f, 1e-5);
    EXPECT_NEAR(v.Y, 0.8f, 1e-5);
}

TEST(Vector2Test, GetDirection) {
    spe::Vector2 A(1.0f, 2.0f);
    spe::Vector2 B(4.0f, 6.0f);
    spe::Vector2 dir = spe::Vector2::GetDirection(A, B);
    spe::Vector2 expected(3.0f, 4.0f);
    expected.Normalize();
    EXPECT_NEAR(dir.X, expected.X, 1e-5);
    EXPECT_NEAR(dir.Y, expected.Y, 1e-5);
}