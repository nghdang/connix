#include "gtest/gtest.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"
#include "ConnixCore/Infrastructure/Configuration/FileReader.hpp"

using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

class FileReaderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_validFilePath = "temp_valid_file.txt";
        std::ofstream ofs(m_validFilePath);
        ofs << "Hello, Connix FileReader!" << std::endl;
        ofs.close();

        m_unreadableFilePath = "temp_unreadable_file.txt";
        std::ofstream unreadableOfs(m_unreadableFilePath);
        unreadableOfs << "secret" << std::endl;
        unreadableOfs.close();
        std::filesystem::permissions(m_unreadableFilePath,
                                     std::filesystem::perms::none,
                                     std::filesystem::perm_options::replace);

        m_tempDirPath = "temp_test_dir";
        std::filesystem::create_directory(m_tempDirPath);
    }

    void TearDown() override
    {
        std::remove(m_validFilePath.c_str());
        std::filesystem::permissions(m_unreadableFilePath,
                                     std::filesystem::perms::all,
                                     std::filesystem::perm_options::replace);
        std::remove(m_unreadableFilePath.c_str());
        std::filesystem::remove(m_tempDirPath);
    }

    std::string m_validFilePath;
    std::string m_unreadableFilePath;
    std::string m_tempDirPath;
};

TEST_F(FileReaderTest, ReadValidFileSuccess)
{
    FileReader reader;
    std::string content = reader.readAll(m_validFilePath);
    EXPECT_EQ(content, "Hello, Connix FileReader!\n");
}

TEST_F(FileReaderTest, ReadNonExistentFileThrowsFileNotFound)
{
    FileReader reader;
    std::string nonExistentPath = "this_file_does_not_exist_at_all.txt";

    try
    {
        reader.readAll(nonExistentPath);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), ConfigurationErrorCode::FILE_NOT_FOUND);
        EXPECT_NE(std::string(ex.what()).find("File does not exist:"),
                  std::string::npos);
    }
}

TEST_F(FileReaderTest, ReadDirectoryThrowsFileReadFailed)
{
    FileReader reader;

    try
    {
        reader.readAll(m_tempDirPath);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), ConfigurationErrorCode::FILE_READ_FAILED);
        EXPECT_NE(std::string(ex.what()).find("Path is not a regular file:"),
                  std::string::npos);
    }
}

TEST_F(FileReaderTest, ReadUnreadableFileThrowsFileReadFailed)
{
    FileReader reader;

    try
    {
        reader.readAll(m_unreadableFilePath);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), ConfigurationErrorCode::FILE_READ_FAILED);
        EXPECT_NE(std::string(ex.what()).find("Failed to open file:"),
                  std::string::npos);
    }
}

TEST(ConfigurationExceptionTest, CheckErrorCodeAndMessage)
{
    ConfigurationException ex(ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED,
                              "schema error");
    EXPECT_EQ(ex.getErrorCode(),
              ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    EXPECT_STREQ(ex.what(), "schema error");
}

} // namespace UnitTest
} // namespace ConnixCore
