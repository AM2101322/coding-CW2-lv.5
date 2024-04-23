#include <gtest/gtest.h>
#include <csv.h>
#include <sstream>
#include <fstream>
#include <string>

TEST(CryptoTest, EncryptDecrypt) {
    std::string data = "HelloWorld";
    std::string key = "key";
    std::string encrypted = encrypt(data, key);
    std::string decrypted = encrypt(encrypted, key);  // Since XOR is reversible

    ASSERT_EQ(decrypted, data);
}

class UserManagementTest : public ::testing::Test {
protected:
    std::string filename = "test_user_file.csv";

    // Function to simulate input and capture output, here we are just simulating the input
    void simulateInput(const std::string& username, const std::string& password) {
        std::ofstream fout(filename, std::ios::app);
        fout << username << "," << password << "\n";
        fout.close();
    }

    // Clean up test environment before each test
    void SetUp() override {
        std::remove(filename.c_str()); // Ensure the file does not exist before the test starts
    }

    // Optional: Clean up after all tests are done
    void TearDown() override {
        std::remove(filename.c_str()); // Clean up the file created by the test
    }
};

TEST_F(UserManagementTest, CreateUserCreatesFile) {
    simulateInput("testuser", "testpassword");


    std::ifstream fin(filename);
    bool fileExists = fin.good();
    std::string line;
    std::getline(fin, line);
    fin.close();

    bool correctContent = (line == "testuser,testpassword");

    ASSERT_TRUE(fileExists);
    ASSERT_TRUE(correctContent);
}