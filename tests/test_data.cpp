/**
 * Data / Config Tests  [data]
 *
 * RED 단계: data/ .cpp 미구현 → 링크 실패.
 * GREEN 단계에서 JsonUnitDefinitionReader.cpp를 추가하여 통과시킨다.
 *
 * 보호 불변식:
 *   폴백 기본값: kFeetPerMeter = 3.28084 / kYardPerMeter = 1.09361
 *   음수 비율:   std::invalid_argument("ratio must be positive")
 *   파일 없음:   예외 없이 기본값 유지 (silent fallback)
 *   JSON 스키마: {"units": {"name": ratio_to_meter}}
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <stdexcept>

#include "../data/JsonUnitDefinitionReader.h"
#include "../entity/UnitRegistry.h"

using Catch::Approx;
using Catch::Matchers::ContainsSubstring;

static constexpr double kEps = 1e-5;

// ============================================================
// § 1. 정상 설정 파일 로드 (TC-B-06)
// ============================================================

// TC-D-01  보호: 유효 JSON → feet 비율 정상 로드
TEST_CASE("test_load_config_valid_json_path_loads_feet_ratio", "[data][config]") {
    // Given
    entity::UnitRegistry reg;
    reg.seedDefaults();

    // When
    data::loadConfig("tests/fixtures/valid_config.json", reg);

    // Then
    REQUIRE(reg.getRatioToMeter("feet") == Approx(3.28084).epsilon(kEps));
}

// TC-D-02  보호: 유효 JSON → yard 비율 정상 로드
TEST_CASE("test_load_config_valid_json_path_loads_yard_ratio", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    data::loadConfig("tests/fixtures/valid_config.json", reg);

    REQUIRE(reg.getRatioToMeter("yard") == Approx(1.09361).epsilon(kEps));
}

// TC-D-03  보호: 사용자 정의 단위 "cubit" JSON → 등록 후 변환 가능
TEST_CASE("test_load_config_valid_json_with_custom_unit_registers_cubit", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    data::loadConfig("tests/fixtures/custom_config.json", reg);

    REQUIRE(reg.hasUnit("cubit"));
    REQUIRE(reg.getRatioToMeter("cubit") == Approx(0.4572).epsilon(kEps));
}

// ============================================================
// § 2. 실패 케이스 — 파일 없음 (TC-B-07)
// ============================================================

// TC-D-04  보호: 파일 없음 → 예외 없이 기본값 폴백 (feet)
TEST_CASE("test_load_config_nonexistent_path_uses_default_feet_ratio", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    // 예외 없이 조용히 폴백
    REQUIRE_NOTHROW(data::loadConfig("tests/fixtures/nonexistent.json", reg));
    REQUIRE(reg.getRatioToMeter("feet") == Approx(3.28084).epsilon(kEps));
}

// TC-D-05  보호: 파일 없음 → 기본값 폴백 (yard)
TEST_CASE("test_load_config_nonexistent_path_uses_default_yard_ratio", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    REQUIRE_NOTHROW(data::loadConfig("tests/fixtures/nonexistent.json", reg));
    REQUIRE(reg.getRatioToMeter("yard") == Approx(1.09361).epsilon(kEps));
}

// ============================================================
// § 3. 실패 케이스 — 잘못된 형식 (Malformed)
// ============================================================

// TC-D-06  보호: 잘못된 JSON → 예외 또는 기본값. 어느 경우든 비율 유효 상태 유지
TEST_CASE("test_load_config_malformed_json_throws_or_uses_defaults", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    bool threw = false;
    try {
        data::loadConfig("tests/fixtures/malformed_config.json", reg);
    } catch (const std::exception&) {
        threw = true;
    }

    if (!threw) {
        // 폴백 선택 시 기본값 유효 상태 유지
        REQUIRE(reg.getRatioToMeter("feet") == Approx(3.28084).epsilon(kEps));
        REQUIRE(reg.getRatioToMeter("yard") == Approx(1.09361).epsilon(kEps));
    }
    // 예외 선택 시 테스트 통과 (어느 정책이든 허용)
    SUCCEED();
}

// TC-D-07  보호: 빈 JSON "{}" → 기본값 폴백
TEST_CASE("test_load_config_empty_file_uses_default_ratios", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    REQUIRE_NOTHROW(data::loadConfig("tests/fixtures/empty_config.json", reg));
    REQUIRE(reg.getRatioToMeter("feet") == Approx(3.28084).epsilon(kEps));
    REQUIRE(reg.getRatioToMeter("yard") == Approx(1.09361).epsilon(kEps));
}

// TC-D-08  보호: 음수 비율 JSON → std::invalid_argument
TEST_CASE("test_load_config_negative_ratio_in_file_throws_invalid_argument", "[data][config]") {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    REQUIRE_THROWS_AS(
        data::loadConfig("tests/fixtures/negative_ratio_config.json", reg),
        std::invalid_argument
    );
    REQUIRE_THROWS_WITH(
        data::loadConfig("tests/fixtures/negative_ratio_config.json", reg),
        ContainsSubstring("ratio must be positive")
    );
}
