// Пример функции хэширования для строк
std::string RabinFingerPrint(const std::string& subchunk, size_t index) {
    // Простая хэш-функция для примера
    std::hash<std::string> hasher;
    return std::to_string(hasher(subchunk + std::to_string(index)));
}