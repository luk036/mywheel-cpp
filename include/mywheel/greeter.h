#pragma once

#include <string>

namespace mywheel {

    /**  Language codes to be used with the Py2Cpp class */
    enum class LanguageCode { EN, DE, ES, FR };

    /**
     * @brief A class for saying hello in multiple languages
     */
    class MyWheel {
        std::string name;

      public:
        /**
         * @brief Creates a new MyWheel
         * @param name the name to greet
         */
        MyWheel(std::string name);

        /**
         * @brief Creates a localized string containing the greeting
         * @param lang the language to greet in
         * @return a string containing the greeting
         */
        std::string greet(LanguageCode lang = LanguageCode::EN) const;
    };

}  // namespace mywheel
