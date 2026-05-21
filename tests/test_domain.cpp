/**
 * Track B — Domain / Logic Tests  [entity]
 *
 * RED 단계: 구현 없음 → 링크 실패 또는 런타임 실패.
 * GREEN 단계에서 entity/ 구현(.cpp)을 추가하여 통과시킨다.
 *
 * 보호 불변식:
 *   kFeetPerMeter  = 3.28084  (ε = 1e-5)
 *   kYardPerMeter  = 1.09361  (ε = 1e-5)
 *   허브 변환: all ↔ all  via meter (직접 곱셈 금지)
 *   음수 값 / 음수 비율: std::invalid_argument
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cmath>
#include <stdexcept>

#include "../entity/UnitRegistry.h"
#include "../entity/LengthConverter.h"

using Catch::Approx;
using Catch::Matchers::ContainsSubstring;
using entity::UnitRegistry;
using entity::LengthConverter;

static constexpr double kEps = 1e-5;

// ============================================================
// § 1. 정상 변환 (Normal Conversion)
// ============================================================

// TC-B-01  보호: 핵심 비율 불변식 (meter→feet)
TEST_CASE("test_meter_to_feet_normal_input_returns_8_20210", "[entity][ratio]") {
    // Given
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    // When
    double result = conv.convert("meter", 2.5, "feet");

    // Then: 2.5 * 3.28084 == 8.20210 (ε=1e-5)
    REQUIRE(result == Approx(8.20210).epsilon(kEps));
}

// TC-B-02  보호: 핵심 비율 불변식 (meter→yard)
TEST_CASE("test_meter_to_yard_normal_input_returns_2_73403", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    double result = conv.convert("meter", 2.5, "yard");

    // 2.5 * 1.09361 == 2.73403
    REQUIRE(result == Approx(2.73403).epsilon(kEps));
}

// TC-B-03  보호: 역변환 정확도 (feet→meter)
TEST_CASE("test_feet_to_meter_reverse_returns_0_30480", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    double result = conv.convert("feet", 1.0, "meter");

    // 1 / 3.28084 == 0.30480
    REQUIRE(result == Approx(0.30480).epsilon(kEps));
}

// TC-B-04  보호: 역변환 정확도 (yard→meter)
TEST_CASE("test_yard_to_meter_reverse_returns_0_91440", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    double result = conv.convert("yard", 1.0, "meter");

    // 1 / 1.09361 == 0.91440
    REQUIRE(result == Approx(0.91440).epsilon(kEps));
}

// TC-B-05  보호: 허브 변환 경로 (feet→yard, meter 경유 필수)
TEST_CASE("test_feet_to_yard_cross_conversion_returns_0_33333", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    double result = conv.convert("feet", 1.0, "yard");

    // 1.0 / 3.28084 * 1.09361 == 0.33333
    REQUIRE(result == Approx(0.33333).epsilon(kEps));
}

// TC-B-06  보호: convertAll이 모든 등록 단위를 반환
TEST_CASE("test_convert_all_meter_1_0_returns_map_with_all_units", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    auto result = conv.convertAll("meter", 1.0);

    REQUIRE(result.count("feet") == 1);
    REQUIRE(result.count("yard") == 1);
    REQUIRE(result.at("feet") == Approx(3.28084).epsilon(kEps));
    REQUIRE(result.at("yard") == Approx(1.09361).epsilon(kEps));
}

// ============================================================
// § 2. 경계값 (Boundary Values)
// ============================================================

// TC-B-BNDRY-01  보호: 영값 허용, 예외 없음
TEST_CASE("test_convert_zero_value_returns_zero_feet", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    REQUIRE_NOTHROW(conv.convert("meter", 0.0, "feet"));
    REQUIRE(conv.convert("meter", 0.0, "feet") == Approx(0.0).epsilon(1e-9));
}

// TC-B-BNDRY-02  보호: 큰 값에서 inf/nan 없음
TEST_CASE("test_convert_large_value_1e10_no_overflow_or_inf", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    double result = conv.convert("meter", 1e10, "feet");

    REQUIRE(result == Approx(3.28084e10).epsilon(1.0));
    REQUIRE(std::isfinite(result));
}

// TC-B-BNDRY-03  보호: 정밀도 소수점 5자리
TEST_CASE("test_convert_precision_six_decimal_places_meter_to_feet", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    REQUIRE(conv.convert("meter", 1.0, "feet") == Approx(3.28084).epsilon(kEps));
}

// TC-B-BNDRY-04  보호: 매우 작은 값 정밀도
TEST_CASE("test_convert_very_small_value_1e_minus_9_meter_to_feet", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    double result = conv.convert("meter", 1e-9, "feet");

    REQUIRE(result == Approx(3.28084e-9).epsilon(1e-14));
}

// TC-B-BNDRY-05  보호: 정수 입력 → double 처리
TEST_CASE("test_convert_integer_input_3_meter_to_feet_returns_9_84252", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    REQUIRE(conv.convert("meter", 3.0, "feet") == Approx(9.84252).epsilon(kEps));
}

// ============================================================
// § 3. 동적 단위 등록 (Dynamic Registration) — TC-B-05
// ============================================================

// TC-REG-01  보호: OCP — 새 단위 등록 후 변환 가능 (코드 수정 없이)
TEST_CASE("test_register_unit_cubit_then_convert_to_meter_returns_0_4572", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    reg.registerUnit("cubit", 0.4572);
    LengthConverter conv(reg);

    REQUIRE(conv.convert("cubit", 1.0, "meter") == Approx(0.4572).epsilon(kEps));
}

// TC-REG-02  보호: 역변환 — 등록된 새 단위 역방향
TEST_CASE("test_register_unit_cubit_then_reverse_meter_to_cubit_returns_2_1872", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    reg.registerUnit("cubit", 0.4572);
    LengthConverter conv(reg);

    // 1 meter = 1 / 0.4572 cubit ≈ 2.1872
    REQUIRE(conv.convert("meter", 1.0, "cubit") == Approx(2.1872).epsilon(1e-4));
}

// TC-REG-03  보호: 교차 변환 — 새 단위 ↔ 기존 단위 (meter 허브 경유)
TEST_CASE("test_register_unit_cubit_then_cross_convert_to_feet_returns_1_4997", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    reg.registerUnit("cubit", 0.4572);
    LengthConverter conv(reg);

    // 1 cubit = 0.4572 m * 3.28084 ft/m = 1.5000 ft
    REQUIRE(conv.convert("cubit", 1.0, "feet") == Approx(1.5000).epsilon(1e-4));
}

// TC-REG-04  보호: 음수 비율 등록 금지
TEST_CASE("test_register_unit_negative_ratio_throws_invalid_argument", "[entity][ratio]") {
    UnitRegistry reg;

    REQUIRE_THROWS_AS(reg.registerUnit("badunit", -1.0), std::invalid_argument);
    REQUIRE_THROWS_WITH(reg.registerUnit("badunit", -1.0),
                        ContainsSubstring("ratio must be positive"));
}

// TC-REG-05  보호: 중복 등록 → 새 비율로 덮어쓰기
TEST_CASE("test_register_unit_duplicate_name_overwrites_ratio", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    reg.registerUnit("cubit", 0.4572);
    reg.registerUnit("cubit", 0.5000);
    LengthConverter conv(reg);

    REQUIRE(conv.convert("cubit", 1.0, "meter") == Approx(0.5000).epsilon(kEps));
}

// ============================================================
// § 4. 예외 — 미지원 단위 변환
// ============================================================

// TC-ERR-01  보호: 미등록 단위 변환 시 즉시 예외
TEST_CASE("test_convert_unknown_from_unit_throws_invalid_argument", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    REQUIRE_THROWS_AS(conv.convert("parsec", 1.0, "meter"), std::invalid_argument);
    REQUIRE_THROWS_WITH(conv.convert("parsec", 1.0, "meter"),
                        ContainsSubstring("unknown unit"));
}

// TC-ERR-02  보호: 음수 값 변환 시 예외
TEST_CASE("test_convert_negative_value_throws_invalid_argument", "[entity][ratio]") {
    UnitRegistry reg;
    reg.seedDefaults();
    LengthConverter conv(reg);

    REQUIRE_THROWS_AS(conv.convert("meter", -1.0, "feet"), std::invalid_argument);
    REQUIRE_THROWS_WITH(conv.convert("meter", -1.0, "feet"),
                        ContainsSubstring("negative value"));
}
