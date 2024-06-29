#include <gtest/gtest.h>

#include "../Include/file.h"   // get file_size
#include "../Include/runner.h" // Du_Main

#include <filesystem>
#include <fstream>

#include <iostream>

std::string read_file(std::string output_name) {
  std::ifstream in(output_name);
  std::size_t size = get_file_size(in);
  std::string result{};

  result.resize(size);

  std::string str(size, '\0');

  if (in.read(&str[0], static_cast<int>(size)))

    result = str;

  in.close();
  return result;
}

void cleanup() {
  for (const auto &entry : std::filesystem::directory_iterator(".")) {
    if (entry.is_regular_file() && entry.path().extension() == ".du") {
      std::filesystem::path FullPathWithoutExtension = entry.path();
      FullPathWithoutExtension.replace_extension("");
      FullPathWithoutExtension += ".actual.txt";

      std::filesystem::remove(FullPathWithoutExtension);
    }
  }
}

TEST(FullTest, FullCodegenTest) {
  std::filesystem::path curr_file = std::filesystem::absolute(__FILE__);
  std::filesystem::path curr_dir = curr_file.parent_path();

  for (const auto &entry : std::filesystem::directory_iterator(curr_dir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".du") {
      std::filesystem::path expectedFile =
          entry.path().stem().string() + "_expected.txt";
      expectedFile = entry.path().parent_path() / expectedFile;
      char const *filename = entry.path().c_str();

      std::string expected_result = read_file(expectedFile);
      std::string actual_result{};

      char const *argv[] = {"DUMMY", filename};

      int argc = 2;

      Du_Main(argc, argv);

      std::string execute_filename =
          entry.path().stem().string() + "_actual.txt";
      std::string execute_it = "./a.out > " + execute_filename;
      std::system(execute_it.c_str());

      actual_result = read_file(execute_filename);
      if (expected_result != actual_result) {
        std::cout << "------------------------------\n";
        std::cout << "error in: " << filename << "\n";
        std::cout << "-------------------------------";
      }
      EXPECT_EQ(expected_result, actual_result);
    }
  }
  cleanup();
}
