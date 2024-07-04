// Функция для рекурсивного чтения всех файлов из директории и сбора данных о функциях
std::vector<std::string> read_functions(const std::string& directory) {
    std::vector<std::string> all_functions;

    // Рекурсивное прохождение по всем файлам в директории
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::vector<std::string> functions = read_functions_from_file(entry.path().string());
            all_functions.insert(all_functions.end(), functions.begin(), functions.end());
        }
    }

    return all_functions;
}