#include "duasm.h"

AsmFile::AsmFile(std::string path, bool setup_needed) {
  // Constructor: open up a file
  file.open(path + file_ext);
  if (setup_needed)
    setup();
}

AsmFile::~AsmFile() {
  // Destructor: close down a file
  file.close();
}

void AsmFile::setup() {}

void AsmFile::new_line(unsigned int amount) {
  // Add a new line specified by the amount
  for (int i = 0; i < amount; i++) {
    file << "\n";
  }
}
void AsmFile::add_comment(std::string content) { file << "; " + content; }

void AsmFile::write_to_file(std::string content, bool new_line_needed) {
  // write the content into the file then create a new line(optional)
  if (new_line_needed) {
    file << content;
    new_line(1);
  } else {
    file << content;
  }
}

void AsmFile::create_section(std::string section_name) {
  // creating a section like .data
  file << "section ." + section_name;
  new_line(1);
}
