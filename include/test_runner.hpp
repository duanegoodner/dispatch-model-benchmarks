#pragma once

// #include <iostream>
// #include <vector>
#include "runtime_polymorphism.hpp"
#include "crtp_polymorphism.hpp"
#include "concepts_polymorphism.hpp"
#include "benchmark_utils.hpp"

namespace runtime_polymorphism {
    void RunTests(size_t iterations);
}

namespace plain_templates {
    void RunTests(size_t iterations);
}

namespace crtp_polymorphism {
    void RunTests(size_t iterations);
}

namespace concepts_polymorphism {
    void RunTests(size_t iterations);
}
