#include "test_runner.hpp"
#include <iostream>
#include "runtime_polymorphism.hpp"
#include "plain_templates.hpp"
#include "crtp_polymorphism.hpp"
#include "concepts_polymorphism.hpp"

namespace runtime_polymorphism {
    void RunTests(size_t iterations, const std::vector<std::string>& labels) {
        PolyMinimal runtimeMinimal;
        PolyFMA runtimeFMA;
        PolySimple runtimeSimple;
        PolyMedium runtimeMedium;
        PolyExpensive runtimeExpensive;

        std::vector<Base*> instances = {
            &runtimeMinimal,
            &runtimeFMA,
            &runtimeSimple,
            &runtimeMedium,
            &runtimeExpensive
        };

        for (size_t i = 0; i < instances.size(); ++i) {
            TestRuntimePolymorphism(labels[i], iterations, *instances[i]);
        }
    }
}

namespace plain_templates {
    void RunTests(size_t iterations, const std::vector<std::string>& labels) {
        TestPlainTemplates<PolyMinimal>(labels[0], iterations);
        TestPlainTemplates<PolyFMA>(labels[1], iterations);
        TestPlainTemplates<PolySimple>(labels[2], iterations);
        TestPlainTemplates<PolyMedium>(labels[3], iterations);
        TestPlainTemplates<PolyExpensive>(labels[4], iterations);
    }
}

namespace crtp_polymorphism {
    void RunTests(size_t iterations, const std::vector<std::string>& labels) {
        TestCRTPPolymorphism<PolyMinimal>(labels[0], iterations);
        TestCRTPPolymorphism<PolyFMA>(labels[1], iterations);
        TestCRTPPolymorphism<PolySimple>(labels[2], iterations);
        TestCRTPPolymorphism<PolyMedium>(labels[3], iterations);
        TestCRTPPolymorphism<PolyExpensive>(labels[4], iterations);
    }
}

namespace concepts_polymorphism {
    void RunTests(size_t iterations, const std::vector<std::string>& labels) {
        TestConceptsPolymorphism(labels[0], iterations, PolyMinimal());
        TestConceptsPolymorphism(labels[1], iterations, PolyFMA());
        TestConceptsPolymorphism(labels[2], iterations, PolySimple());
        TestConceptsPolymorphism(labels[3], iterations, PolyMedium());
        TestConceptsPolymorphism(labels[4], iterations, PolyExpensive());
    }
}
