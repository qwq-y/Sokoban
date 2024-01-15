#ifndef MAPFILE_H
#define MAPFILE_H

#include <string>

class MapFile {
public:
    static std::string calculateChecksum(const std::string& data);
    static std::string readMapFromFile(const std::string& filename);
    static void saveMapToFile(const std::string& mapData, const std::string& filename);
};

#endif
