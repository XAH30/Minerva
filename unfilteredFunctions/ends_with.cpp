// Функция для проверки, заканчивается ли строка подстрокой
bool ends_with(std::string const& text, std::string const& substr) {
    if (substr.length() > text.length()) return false;
    return std::equal(substr.rbegin(), substr.rend(), text.rbegin(),
                      [](char ch1, char ch2) {
                          return std::toupper(ch1) == std::toupper(ch2); 
                      });
}