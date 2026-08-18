#include "gtest/gtest.h"
#include "Audio_Test/Audio_Test.h"
#include "Math_Tests/Vector_Test.h"
#include "Core_Tests/Time_Test.h"
#include "Core_Tests/Log_Test.h"
#include "Core_Tests/EngineData_Test.h"
#include "Core_Tests/Input_Test.h"
#include "Core_Tests/Camera_Test.h"


// Main function for Google Test
int main(int argc, char **argv) {
	while (spe::Utility::GetCurrentDir().find("Engine\\Build") != std::string::npos) {
		spe::Utility::SetCurrentDir("../");
	}

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
