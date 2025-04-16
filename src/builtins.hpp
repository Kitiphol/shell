#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <vector>
#include <string>

namespace Builtins {

    bool isBuiltin(const std::vector<std::string>& tokens);

    bool execute(const std::vector<std::string>& tokens);
    
}

#endif
