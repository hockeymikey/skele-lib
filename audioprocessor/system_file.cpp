//
//  system_file.cpp
//  AudioProcessor
//
//  Created by Aziz Usmanov on 5/24/17.
//
//

#include "system_file.hpp"

CAP::SystemFile::SystemFile(std::string filepath_): CAP::File(), filepath(filepath_) {
    
}

void CAP::SystemFile::close() {
    filestream.close();
}

bool CAP::SystemFile::isOpen() const {
    return filestream.is_open();
}

bool CAP::SystemFile::write(const CAP::AudioBuffer &audioBuffer) {
    auto b = reinterpret_cast<const char *>(audioBuffer.getBuffer());
    filestream.write(b, audioBuffer.size() * sizeof(*audioBuffer.getBuffer()));
    return !filestream.bad();
}

void CAP::SystemFile::open() {
    filestream.open(filepath, std::ofstream::binary | std::ofstream::trunc);
}

std::string CAP::SystemFile::path() const {
    return filepath;
}

CAP::SystemFile::~SystemFile() {
    
}