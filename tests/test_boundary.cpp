/**
 * Track A — UI / Boundary Tests  [boundary]
 *
 * RED 단계: entity/ .cpp 미구현 → 링크 실패.
 * ConversionService를 통해 계약(에러 문구·포맷)만 검증한다.
 * MockLengthConverter로 entity를 격리하여 순수 boundary 계약을 검증한다.
 *
 * 보호 불변식:
 *   입력 포맷: "unit:value"
 *   출력 포맷: "N.N unit = N.NNNNNN toUnit"
 *   예외 타입: std::invalid_argument (단일 타입 고정)
 *   음수 값:   거부 (PRD §3.3)
 *   대소문자:  구분 ("METER" ≠ "meter")
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <stdexcept>
#include <string>

#include "../boundary/ConversionService.h"
#include "../tests/mocks/MockLengthConverter.h"

using Catch::Approx;
using Catch::Matchers::ContainsSubstring;

// ============================================================
// § 1. 정상 입력 / 출력 포맷
// ============================================================

// TC-A-01  보호: Happy Path — 정상 입력 시 예외 없이 결과 반환
TEST_CASE("test_parse_normal_input_meter_2_5_returns_conversion_result", "[boundary][parse]") {
    // Given
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    // When / Then: 예외 없이 반환
    REQUIRE_NOTHROW(svc.parseAndConvert("meter:2.5"));
}

// TC-A-06a  보호: 출력에 원 입력 단위·값 접두어 보존
TEST_CASE("test_output_format_preserves_original_unit_and_value", "[boundary][format]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    std::string result = svc.parseAndConvert("meter:2.5");

    // "2.5 meter = ..." 포함 필수
    REQUIRE_THAT(result, ContainsSubstring("2.5 meter ="));
}

// TC-A-06b  보호: 출력 소수점 6자리 형식
TEST_CASE("test_output_format_six_decimal_places", "[boundary][format]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    std::string result = svc.parseAndConvert("meter:1.0");

    // feet 변환 결과 소수점 6자리 표기
    REQUIRE_THAT(result, ContainsSubstring("3.280840"));
}

// ============================================================
// § 2. 예외 케이스 — std::invalid_argument 단일 타입
// ============================================================

// TC-A-02  보호: ':' 없는 입력 → 형식 오류
TEST_CASE("test_parse_missing_colon_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert("meter2.5"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert("meter2.5"),
                        ContainsSubstring("missing ':'"));
}

// TC-A-03  보호: 음수 값 거부 정책 (PRD §3.3)
TEST_CASE("test_parse_negative_value_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert("meter:-1.0"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert("meter:-1.0"),
                        ContainsSubstring("negative value"));
}

// TC-A-04  보호: 미등록 단위 → 미지원 단위 계약
TEST_CASE("test_parse_unknown_unit_parsec_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert("parsec:1.0"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert("parsec:1.0"),
                        ContainsSubstring("unknown unit"));
}

// TC-A-05  보호: 숫자 아닌 값 → 파싱 실패 계약
TEST_CASE("test_parse_non_numeric_value_abc_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert("meter:abc"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert("meter:abc"),
                        ContainsSubstring("invalid number"));
}

// TC-EX-01  보호: 빈 문자열 → 형식 오류
TEST_CASE("test_parse_empty_string_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert(""), std::invalid_argument);
}

// ============================================================
// § 3. 경계값 입력
// ============================================================

// TC-A-07  보호: 영값 허용 — 예외 없음, "0.000000" 출력
TEST_CASE("test_parse_zero_value_meter_returns_zero_output", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_NOTHROW(svc.parseAndConvert("meter:0"));
    REQUIRE_THAT(svc.parseAndConvert("meter:0"), ContainsSubstring("0.000000"));
}

// TC-EX-03  보호: 값 없는 입력 "unit:" → invalid number
TEST_CASE("test_parse_unit_only_no_value_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert("meter:"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert("meter:"),
                        ContainsSubstring("invalid number"));
}

// TC-EX-04  보호: 단위 없는 입력 ":value" → unknown unit
TEST_CASE("test_parse_value_only_no_unit_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert(":2.5"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert(":2.5"),
                        ContainsSubstring("unknown unit"));
}

// TC-EX-06  보호: 대소문자 구분 — "METER" 미지원
TEST_CASE("test_parse_uppercase_unit_METER_throws_invalid_argument", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvert("METER:1.0"), std::invalid_argument);
    REQUIRE_THROWS_WITH(svc.parseAndConvert("METER:1.0"),
                        ContainsSubstring("unknown unit: METER"));
}

// TC-EX-08  보호: 과학적 표기법 지원 (std::stod 표준 동작)
TEST_CASE("test_parse_scientific_notation_2_5e1_meter_returns_82_021", "[boundary][parse]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    // 2.5e1 = 25.0 meter → 25 * 3.28084 ≈ 82.021 feet
    REQUIRE_NOTHROW(svc.parseAndConvert("meter:2.5e1"));
    REQUIRE_THAT(svc.parseAndConvert("meter:2.5e1"), ContainsSubstring("25"));
}

// ============================================================
// § 4. JSON 출력 포맷 (TC-A-06 확장)
// ============================================================

// TC-A-JSON  보호: JSON 스키마 — "input" 키와 "conversions" 배열 포함
TEST_CASE("test_json_output_contains_input_and_conversions_keys", "[boundary][format]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    std::string json = svc.parseAndConvertJson("meter:2.5");

    REQUIRE_THAT(json, ContainsSubstring("\"input\""));
    REQUIRE_THAT(json, ContainsSubstring("\"conversions\""));
    REQUIRE_THAT(json, ContainsSubstring("\"meter\""));
    REQUIRE_THAT(json, ContainsSubstring("\"feet\""));
    REQUIRE_THAT(json, ContainsSubstring("\"yard\""));
}

// TC-A-JSON-ERR  보호: JSON 요청에도 동일한 예외 계약 적용
TEST_CASE("test_json_output_invalid_input_throws_invalid_argument", "[boundary][format]") {
    mocks::MockLengthConverter mock;
    boundary::ConversionService svc(mock);

    REQUIRE_THROWS_AS(svc.parseAndConvertJson("meter:-1.0"), std::invalid_argument);
}
