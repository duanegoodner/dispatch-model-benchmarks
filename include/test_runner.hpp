#pragma once

#include <vector>
#include <string>

namespace runtime_polymorphism {
    void RunTests(size_t iterations, const std::vector<std::string>& labels);
}

namespace plain_templates {
    void RunTests(size_t iterations, const std::vector<std::string>& labels);
}

namespace crtp_polymorphism {
    void RunTests(size_t iterations, const std::vector<std::string>& labels);
}

namespace concepts_polymorphism {
    void RunTests(size_t iterations, const std::vector<std::string>& labels);
}
