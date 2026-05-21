// Approval / Golden Master regression tests — [golden-master][integration]
//
// Workflow:
//   First run  : expected file missing → writes actual → FAIL ("approve and commit")
//   Next runs  : per-section compare  → FAIL with line diff on mismatch

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../boundary/ConversionService.h"
#include "../entity/LengthConverter.h"
#include "../entity/UnitRegistry.h"

namespace {

const std::string kExpectedPath = "tests/golden_master_expected.txt";
const std::vector<std::string> kInputs = {
    "meter:2.5", "feet:1.0", "yard:1.0", "meter:0.0"
};

// Lifetime: reg → conv → svc (ref chain)
struct App {
    entity::UnitRegistry        reg;
    entity::LengthConverter     conv;
    boundary::ConversionService svc;
    App() : conv(reg), svc(conv) { reg.seedDefaults(); }
};

std::string buildActual(boundary::ConversionService& svc) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < kInputs.size(); ++i) {
        if (i > 0) oss << "---\n";
        oss << "[" << kInputs[i] << "]\n"
            << svc.parseAndConvert(kInputs[i]) << "\n";
    }
    return oss.str();
}

std::map<std::string, std::string> parseExpected(std::istream& in) {
    std::map<std::string, std::string> result;
    std::string line, label, body;
    while (std::getline(in, line)) {
        if (line == "---") {
            if (!label.empty()) { result[label] = body; label.clear(); body.clear(); }
        } else if (line.size() >= 2 && line.front() == '[' && line.back() == ']') {
            if (!label.empty()) result[label] = body;
            label = line.substr(1, line.size() - 2);
            body.clear();
        } else {
            body += line + "\n";
        }
    }
    if (!label.empty()) result[label] = body;
    return result;
}

std::map<std::string, std::string> loadExpected() {
    std::ifstream ef(kExpectedPath);
    if (!ef.is_open()) return {};
    return parseExpected(ef);
}

std::string lineDiff(const std::string& expected, const std::string& actual) {
    std::istringstream es(expected), as(actual);
    std::ostringstream diff;
    diff << "--- expected\n+++ actual\n";
    std::string el, al;
    int line = 1;
    while (static_cast<bool>(std::getline(es, el)) |
           static_cast<bool>(std::getline(as, al))) {
        if (el != al)
            diff << "@ L" << line << ": exp='" << el
                 << "'  act='" << al << "'\n";
        ++line; el.clear(); al.clear();
    }
    return diff.str();
}

} // namespace

// ── Approve: first run → write; subsequent runs → full compare ────────────────

TEST_CASE("golden master — approve and lock output",
          "[golden-master][integration]") {
    App app;
    const std::string actual = buildActual(app.svc);

    std::ifstream ef(kExpectedPath);
    if (!ef.is_open()) {
        std::ofstream out(kExpectedPath);
        INFO("Cannot create: " + kExpectedPath);
        REQUIRE(out.is_open());
        out << actual;
        FAIL("Golden master written to '" + kExpectedPath + "'.\n"
             "Review and commit:\n"
             "  git add tests/golden_master_expected.txt && git commit");
    }

    const std::string expected(std::istreambuf_iterator<char>(ef),
                               std::istreambuf_iterator<char>{});
    if (actual != expected)
        FAIL("Golden master mismatch:\n" + lineDiff(expected, actual));
    SUCCEED();
}

// ── Per-scenario regression: GM-TC-01 ~ GM-TC-04 ─────────────────────────────

TEST_CASE("golden master — per-scenario regression",
          "[golden-master][integration]") {
    const auto sections = loadExpected();
    if (sections.empty()) SKIP();  // run approve test first to generate expected file

    App app;

    SECTION("GM-TC-01 meter:2.5") {
        const std::string actual = app.svc.parseAndConvert("meter:2.5") + "\n";
        const std::string& exp   = sections.at("meter:2.5");
        if (actual != exp) FAIL("GM-TC-01:\n" + lineDiff(exp, actual));
        SUCCEED();
    }
    SECTION("GM-TC-02 feet:1.0") {
        const std::string actual = app.svc.parseAndConvert("feet:1.0") + "\n";
        const std::string& exp   = sections.at("feet:1.0");
        if (actual != exp) FAIL("GM-TC-02:\n" + lineDiff(exp, actual));
        SUCCEED();
    }
    SECTION("GM-TC-03 yard:1.0") {
        const std::string actual = app.svc.parseAndConvert("yard:1.0") + "\n";
        const std::string& exp   = sections.at("yard:1.0");
        if (actual != exp) FAIL("GM-TC-03:\n" + lineDiff(exp, actual));
        SUCCEED();
    }
    SECTION("GM-TC-04 meter:0.0") {
        const std::string actual = app.svc.parseAndConvert("meter:0.0") + "\n";
        const std::string& exp   = sections.at("meter:0.0");
        if (actual != exp) FAIL("GM-TC-04:\n" + lineDiff(exp, actual));
        SUCCEED();
    }
}
