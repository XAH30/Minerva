// Функция для разбиения строки на подстроки фиксированной длины
std::vector<std::string> splitIntoSubchunks(const std::string& str, size_t subchunkSize) {
    std::vector<std::string> subchunks;
    for (size_t i = 0; i < str.size(); i += subchunkSize) {
        subchunks.push_back(str.substr(i, subchunkSize));
    }
    return subchunks;
}