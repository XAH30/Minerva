bool contains(std::string const & text, std::string const & substr)
{
   if (substr.length() > text.length()) return false;
 
   auto it = std::search(
      text.begin(), text.end(),
      substr.begin(), substr.end(),
      [](char ch1, char ch2) { 
         return std::toupper(ch1) == std::toupper(ch2); });
 
   return it != text.end();
}