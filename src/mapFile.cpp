//
// Created by 86188 on 2024/1/15.
//
#include "mapFile.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>

std::string MapFile::calculateChecksum(const std::string& data) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data.c_str(), data.length());

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &ctx);

    std::ostringstream checksum;
    checksum << std::hex << std::setfill('0');
    for (const auto& byte : result) {
        checksum << std::setw(2) << static_cast<int>(byte);
    }

    return checksum.str();
}

std::string MapFile::readMapFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return "";
    }

    std::string line;
    std::getline(file, line); // Read the identifier line
    if (line != "RecurseBoxFile") {
        std::cerr << "Error: Invalid file format. Expected 'RecurseBoxFile' identifier." << std::endl;
        return "";
    }

    std::string checksumFromFile;

    // 读取储存的校验和
    while (std::getline(file, line)) {
        size_t checksumPos = line.find("Checksum:");
        if (checksumPos != std::string::npos) {
            checksumFromFile = line.substr(checksumPos + 10, MD5_DIGEST_LENGTH * 2);  // 10 is the length of "Checksum: "
            std::cout << "checksumFromFile: " << checksumFromFile << std::endl;
            break;
        }
    }
    if (checksumFromFile.empty()) {
        std::cerr << "Error: Checksum not found in the file." << std::endl;
        return "";
    }

    // 计算当前校验和
    std::stringstream mapData;
    while (std::getline(file, line)) {
        mapData << line << "\n";
    }
    std::string recalculatedChecksum = calculateChecksum(mapData.str());
    std::cout << "recalculatedChecksum: " << recalculatedChecksum << std::endl;

    if (checksumFromFile != recalculatedChecksum) {
        std::cerr << "Error: Checksum verification failed. File may be corrupted." << std::endl;
        return "";
    }

    file.close();
    return mapData.str();
}

void MapFile::saveMapToFile(const std::string& mapData, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "RecurseBoxFile\n";
        file << "Checksum: " << calculateChecksum(mapData) << "\n";
        file << mapData;
        file.close();
        std::cout << "Map saved to " << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
}
