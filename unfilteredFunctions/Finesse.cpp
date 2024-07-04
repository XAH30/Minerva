std::set<std::pair<int, int>> Finesse(const std::vector<std::string>& chunks1, const std::vector<std::string>& chunks2, size_t subchunkSize, double similarityThreshold) {
    const int numHashFunctions = 100;  // Количество хэш-функций для мин-хэширования
    const int bands = 20;  // Количество банд для бандинга
    const int rowsPerBand = numHashFunctions / bands;  // Количество строк в каждой банд

    // Создание случайных хэш-функций
    std::vector<std::function<size_t(const std::string&)>> hashFunctions;
    std::mt19937 rng(42);  // Фиксированное значение для воспроизводимости
    for (int i = 0; i < numHashFunctions; ++i) {
        std::uniform_int_distribution<size_t> dist(0, std::numeric_limits<size_t>::max());
        size_t a = dist(rng);
        size_t b = dist(rng);
        hashFunctions.push_back([a, b](const std::string& x) {
            std::hash<std::string> hasher;
            return a * hasher(x) + b;
        });
    }

    // Функция для создания мин-хэшей
    auto createMinHashes = [&](const std::vector<std::string>& chunks) {
        std::vector<std::vector<size_t>> minHashes(chunks.size(), std::vector<size_t>(numHashFunctions, std::numeric_limits<size_t>::max()));
        for (size_t i = 0; i < chunks.size(); ++i) {
            const std::string& chunk = chunks[i];
            const std::vector<std::string>& subchunks = splitIntoSubchunks(chunk, subchunkSize);
            for (size_t j = 0; j < subchunks.size(); ++j) {
                std::string subchunkData = subchunks[j];
                for (int h = 0; h < numHashFunctions; ++h) {
                    size_t hashValue = hashFunctions[h](subchunkData);
                    if (hashValue < minHashes[i][h]) {
                        minHashes[i][h] = hashValue;
                    }
                }
            }
        }
        return minHashes;
    };

    // Создание мин-хэшей для обоих наборов данных
    std::vector<std::vector<size_t>> minHashes1 = createMinHashes(chunks1);
    std::vector<std::vector<size_t>> minHashes2 = createMinHashes(chunks2);

    // Создание банд
    std::unordered_map<std::string, std::vector<int>> bandMap;
    for (size_t i = 0; i < minHashes1.size(); ++i) {
        for (int b = 0; b < bands; ++b) {
            std::string bandHash = "";
            for (int r = 0; r < rowsPerBand; ++r) {
                bandHash += std::to_string(minHashes1[i][b * rowsPerBand + r]) + ",";
            }
            bandMap[bandHash].push_back(i);
        }
    }

    // Поиск кандидатов
    std::set<std::pair<int, int>> similarChunksSet;
    for (size_t i = 0; i < minHashes2.size(); ++i) {
        std::unordered_set<int> candidateChunks;
        for (int b = 0; b < bands; ++b) {
            std::string bandHash = "";
            for (int r = 0; r < rowsPerBand; ++r) {
                bandHash += std::to_string(minHashes2[i][b * rowsPerBand + r]) + ",";
            }
            if (bandMap.find(bandHash) != bandMap.end()) {
                candidateChunks.insert(bandMap[bandHash].begin(), bandMap[bandHash].end());
            }
        }

        // Проверка кандидатов
        for (int candidate : candidateChunks) {
            const std::string& chunk1 = chunks1[candidate];
            const std::string& chunk2 = chunks2[i];
            const std::vector<std::string>& subchunks1 = splitIntoSubchunks(chunk1, subchunkSize);
            const std::vector<std::string>& subchunks2 = splitIntoSubchunks(chunk2, subchunkSize);

            size_t numSimilarSubchunks = 0;
            for (const auto& subchunk1 : subchunks1) {
                for (const auto& subchunk2 : subchunks2) {
                    if (isSimilar(subchunk1, subchunk2, similarityThreshold)) {
                        numSimilarSubchunks++;
                        break;
                    }
                }
            }

            size_t numSubchunksToCompare = static_cast<size_t>(static_cast<double>(subchunks1.size()) * similarityThreshold);
            if (numSimilarSubchunks >= numSubchunksToCompare) {
                similarChunksSet.insert(std::make_pair(candidate, i));
            }
        }
    }

    return similarChunksSet;
}