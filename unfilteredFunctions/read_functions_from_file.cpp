// Функция для чтения функций из файла
std::vector<std::string> read_functions_from_file(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> functions;
    bool in_function = false;
    std::vector<std::string> function_lines;

    while (std::getline(file, line)) {
        // Удаление форматирования
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

        // Объединение построчных данных
        if (starts_with(line, "static") || starts_with(line, "public") || 
            starts_with(line, "dynamic") || starts_with(line, "void") || 
            starts_with(line, "float") || starts_with(line, "double") ||
            starts_with(line, "int") || starts_with(line, "class")) {
            in_function = true;
            function_lines.clear();
        }

        if (in_function) {
            function_lines.push_back(line);
        }

        if (ends_with(line, "}")) {
            in_function = false;
            std::string function = std::accumulate(function_lines.begin(), function_lines.end(), std::string(),
                                                   [](const std::string& a, const std::string& b) -> std::string {
                                                       return a + (a.length() > 0 ? "\n" : "") + b;
                                                   });
            functions.push_back(function);
        }
    }

    return functions;
}