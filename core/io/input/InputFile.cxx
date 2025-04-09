#include "InputFile.h"
nlohmann::json bm::file::File::json() const {
    FILE* f = fopen(this->path().c_str(), "r");
    return nlohmann::json::parse(f);
}