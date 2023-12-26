#include "config-cxx/Config.h"

#include <fstream>

#include "gtest/gtest.h"

#include "tests/EnvironmentSetter.h"
#include "tests/ProjectRootFinder.h"

using namespace ::testing;
using namespace config;
using namespace config::tests;

namespace
{
const std::string projectRootPath = ProjectRootFinder::getProjectRoot();
const std::string testDirectoryPath = projectRootPath + "/testData1";
const std::string testConfigDirectory = testDirectoryPath + "/config";

const std::string testJson = R"(
{
    "db": {
        "host": "host",
        "port": 1996
    }
}
)";

const std::string defaultJson = R"(
{
    "db": {
        "host": "localhost",
        "port": 3306
    }
}
)";
}

class ConfigTest : public Test
{
public:
    void SetUp() override
    {
        std::filesystem::remove_all(testDirectoryPath);

        std::filesystem::create_directories(testConfigDirectory);

        std::ofstream testConfigFile{testConfigDirectory + "/test.json"};

        testConfigFile << testJson;

        std::ofstream defaultConfigFile{testConfigDirectory + "/default.json"};

        defaultConfigFile << defaultJson;
    }

    void TearDown() override
    {
        if (std::filesystem::exists(testDirectoryPath))
        {
            std::filesystem::remove_all(testDirectoryPath);
        }
    }

    Config config;
};

TEST_F(ConfigTest, returnsKeyPath)
{
    EnvironmentSetter::setEnvironmentVariable("CXX_ENV", "test");

    EnvironmentSetter::setEnvironmentVariable("CXX_CONFIG_DIR", testConfigDirectory);

    const std::string keyPath = "db.host";

    const auto configValue = config.get<std::string>(keyPath);

    ASSERT_EQ(configValue, keyPath);
};

// TEST_F(ConfigTest, returnsKeyPathAsAny)
//{
//     const std::string path = "db.host";
//
//     const auto configValue = config.get<std::string>(path);
//
//     ASSERT_EQ(std::any_cast<std::string>(configValue), path);
// }
