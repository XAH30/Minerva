// Функция для проверки, начинается ли строка с подстроки
bool starts_with(std::string const& text, std::string const& substr) {
    if (substr.length() > text.length()) return false;
    return std::equal(substr.begin(), substr.end(), text.begin(),
                      [](char ch1, char ch2) {
                          return std::toupper(ch1) == std::toupper(ch2); 
                      });
}