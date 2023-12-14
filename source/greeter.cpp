#include <fmt/format.h>
#include <mywheel/greeter.h>

using namespace mywheel;

MyWheel::MyWheel(std::string _name) : name(std::move(_name)) {}

/**
 * The function `greet` returns a greeting message based on the given language code.
 * 
 * @param lang The "lang" parameter is of type "LanguageCode".
 * 
 * @return a formatted greeting message based on the provided language code. The specific greeting
 * message depends on the value of the `lang` parameter. If the `lang` parameter is `LanguageCode::EN`,
 * the function will return a greeting message in English. If the `lang` parameter is
 * `LanguageCode::DE`, the function will return a greeting message in German. If the `lang
 */
std::string MyWheel::greet(LanguageCode lang) const {
    switch (lang) {
        default:
        case LanguageCode::EN:
            return fmt::format("Hello, {}!", name);
        case LanguageCode::DE:
            return fmt::format("Hallo {}!", name);
        case LanguageCode::ES:
            return fmt::format("¡Hola {}!", name);
        case LanguageCode::FR:
            return fmt::format("Bonjour {}!", name);
    }
}
