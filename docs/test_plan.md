# 테스트 계획서 — UnitConverter_07 (C++)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성자 | 시니어 QA 리드 |
| 작성일 | 2026-05-21 |
| 기준 브랜치 | `red` (B_01에서 분기) |
| 기술 스택 | C++17 · CMake · Catch2 |
| 참조 문서 | PRD.md · requirements.md · `.cursorrules` |

---

## 1. 개요

### 1.1 테스트 목적

샘플 예제 **`meter:2.5` → feet 변환** 을 기준으로 Dual-Track TDD RED 단계의 테스트 계획을 수립한다.
모든 테스트는 구현 없이 **의도적으로 실패(`FAIL("RED")`)** 상태로 작성하며, GREEN 단계에서 최소 구현으로 순차 통과시킨다.

### 1.2 선택된 샘플 예제

| 항목 | 내용 |
|------|------|
| 기능 이름 | meter → feet 변환 (핵심 허브 비율 검증) |
| 입력 | `"meter:2.5"` |
| 기대 출력 | `"2.5 meter = 8.202100 feet"` |
| 핵심 비율 | `1 meter = 3.28084 feet` (오차 ε ≤ 1e-5) |
| 역변환 비율 | `1 feet = 0.30480 meter` (오차 ε ≤ 1e-5) |
| 참조 | PRD §3.2, README Quick Start |

### 1.3 테스트 구조 (Dual-Track)

```
테스트 스위트
├── Track A — UI / Boundary  [boundary] 태그
│     입력 파싱 계약, 포맷 검증, 예외 발생 조건
└── Track B — Domain / Logic  [domain] 태그
      변환 정확도, 레지스트리, 설정 로드
```

---

## 2. SUT (System Under Test) — 현재 코드 상태

```
UnitConverter.cpp  ← 단일 main() 함수
  - 입력 파싱: std::getline + find(':')
  - 변환: if-else 체인 (meter / feet / yard)
  - 출력: std::cout
  - 예외: 없음 (return 1 종료)
```

**문제점 (RED 단계가 존재하는 이유):**

| 문제 | 설명 |
|------|------|
| SRP 위반 | 파싱·검증·변환·출력이 main() 한 함수에 혼재 |
| OCP 위반 | 새 단위 추가 시 if-else 체인 직접 수정 필요 |
| 예외 없음 | `std::invalid_argument` 대신 `return 1` 처리 |
| 매직 넘버 | `3.28084` / `1.09361` 인라인 하드코딩 |
| 테스트 없음 | Catch2 테스트 파일 부재 |

---

## 3. 테스트 범위 및 우선순위

### 3.1 우선순위 기준

| 우선순위 | 기준 |
|----------|------|
| P1 — 필수 | PRD 비즈니스 로직 직접 검증 / GREEN 차단 |
| P2 — 중요 | 경계값·예외 처리 / 회귀 위험 |
| P3 — 선택 | 확장 기능 / Nice-to-Have |

### 3.2 Track A — UI / Boundary 테스트 목록

| TC-ID | 테스트명 | 우선순위 | 보호 계약 |
|-------|----------|----------|-----------|
| TC-A-01 | `"meter:2.5"` 정상 입력 → 변환 결과 반환 | P1 | Happy Path |
| TC-A-02 | `":"` 없는 입력 → `std::invalid_argument` 발생 | P1 | 입력 형식 계약 |
| TC-A-03 | 음수 입력 `"meter:-1.0"` → `std::invalid_argument` 발생 | P1 | 음수 거부 정책 |
| TC-A-04 | 없는 단위 `"parsec:1.0"` → `std::invalid_argument` 발생 | P1 | 미지원 단위 계약 |
| TC-A-05 | 소수점 파싱 실패 `"meter:abc"` → `std::invalid_argument` 발생 | P1 | 파싱 실패 계약 |
| TC-A-06 | 출력 포맷에 원 입력 단위·값 보존 (`"2.5 meter = ..."`) | P2 | 출력 포맷 계약 |
| TC-A-07 | `value = 0` 경계값 → 결과 `0.000000` 출력 | P2 | 영값 허용 정책 |

### 3.3 Track B — Domain / Logic 테스트 목록

| TC-ID | 테스트명 | 우선순위 | 보호 계약 |
|-------|----------|----------|-----------|
| TC-B-01 | `convert("meter", 2.5, "feet") == 8.20210` (ε=1e-5) | P1 | 핵심 비율 불변식 |
| TC-B-02 | `convert("meter", 1.0, "yard") == 1.09361` (ε=1e-5) | P1 | 핵심 비율 불변식 |
| TC-B-03 | `convert("feet", 1.0, "meter") == 0.30480` (역변환) | P1 | 역변환 정확도 |
| TC-B-04 | `convertAll("meter", 1.0)` → 모든 등록 단위 변환 반환 | P2 | 전체 변환 계약 |
| TC-B-05 | `registerUnit("cubit", 0.4572)` 후 변환 가능 | P2 | OCP 등록 계약 |
| TC-B-06 | `loadConfig(유효한 경로)` → 비율 정상 로드 | P3 | 설정 로드 계약 |
| TC-B-07 | `loadConfig(없는 경로)` → 기본값 유지 (3.28084 / 1.09361) | P3 | 폴백 정책 |

---

## 4. 경계값 케이스 목록

### 4.1 `value = 0` — 영값 변환

| 항목 | 내용 |
|------|------|
| 입력 | `"meter:0"` 또는 `"meter:0.0"` |
| 기대 결과 | `0.0 meter = 0.000000 feet` (변환 결과 0, 예외 없음) |
| 정책 | **허용** — 0은 유효한 측정값 |
| 위험 | 출력 포맷의 소수점 6자리 표기 (`%.6f`) 누락 여부 |
| 대응 TC | TC-A-07 |

```cpp
TEST_CASE("convert_meter_zero_value_returns_zero_feet", "[domain]") {
    FAIL("RED");
    // REQUIRE( convert("meter", 0.0, "feet") == Approx(0.0).epsilon(1e-5) );
}
```

### 4.2 `value = 매우 큰 수` — 오버플로 위험

| 항목 | 내용 |
|------|------|
| 입력 | `"meter:1e308"` (double 최댓값 근접) |
| 기대 결과 | `std::numeric_limits<double>::infinity()` 또는 `std::invalid_argument` |
| 정책 | **TBD** — PRD §3.3에 미명시. RED 단계에서 정책 고정 필요 |
| 위험 | `3.28084 * 1e308` → `inf` 발생, 출력 형식 깨짐 |
| 권고 | `value > 1e15` 입력 시 `std::invalid_argument("value overflow")` 던지기 |
| 대응 TC | TC-A-EX-01 (추가 케이스) |

```cpp
TEST_CASE("convert_meter_overflow_value_throws", "[boundary]") {
    FAIL("RED");
    // REQUIRE_THROWS_AS( parseAndConvert("meter:1e308"), std::invalid_argument );
}
```

### 4.3 `value < 0` — 음수 입력 정책

| 항목 | 내용 |
|------|------|
| 입력 | `"meter:-1.0"`, `"feet:-3.28"` |
| 기대 결과 | `std::invalid_argument("negative value not allowed")` |
| 정책 | **거부** — 길이 단위는 음수 불허 (PRD §3.3 명시) |
| 위험 | 음수 허용 시 출력이 물리적으로 무의미한 결과 |
| 대응 TC | TC-A-03 |

```cpp
TEST_CASE("convert_negative_value_throws_invalid_argument", "[boundary]") {
    FAIL("RED");
    // REQUIRE_THROWS_AS( parseAndConvert("meter:-1.0"), std::invalid_argument );
}
```

### 4.4 소수점 파싱 실패 — `"meter:abc"`

| 항목 | 내용 |
|------|------|
| 입력 | `"meter:abc"`, `"meter:1.2.3"`, `"meter: "` |
| 기대 결과 | `std::invalid_argument("invalid number: abc")` |
| 정책 | `std::stod` 실패 시 `std::invalid_argument` 재throw |
| 현재 코드 문제 | `catch(...)` 로 `return 1` 처리 — 테스트 불가능한 구조 |
| 대응 TC | TC-A-05 |

```cpp
TEST_CASE("parse_non_numeric_value_throws_invalid_argument", "[boundary]") {
    FAIL("RED");
    // REQUIRE_THROWS_AS( parseAndConvert("meter:abc"), std::invalid_argument );
}
```

### 4.5 `":"` 없는 입력 — 형식 오류

| 항목 | 내용 |
|------|------|
| 입력 | `"meter2.5"`, `"meter"`, `""` (빈 문자열) |
| 기대 결과 | `std::invalid_argument("invalid format: missing ':'")` |
| 정책 | 구분자 `:` 필수. 없으면 즉시 예외 |
| 현재 코드 문제 | `return 1` 처리 — Catch2로 검증 불가 |
| 대응 TC | TC-A-02 |

```cpp
TEST_CASE("parse_missing_colon_throws_invalid_argument", "[boundary]") {
    FAIL("RED");
    // REQUIRE_THROWS_AS( parseAndConvert("meter2.5"), std::invalid_argument );
}
```

### 4.6 없는 단위 — `"parsec:1.0"`

| 항목 | 내용 |
|------|------|
| 입력 | `"parsec:1.0"`, `"kilometer:1.0"`, `"METER:1.0"` (대소문자) |
| 기대 결과 | `std::invalid_argument("unknown unit: parsec")` |
| 정책 | 레지스트리 미등록 단위는 즉시 예외. 대소문자 구분 |
| 현재 코드 문제 | `return 1` 처리 — 테스트 불가 |
| 대응 TC | TC-A-04 |

```cpp
TEST_CASE("parse_unknown_unit_throws_invalid_argument", "[boundary]") {
    FAIL("RED");
    // REQUIRE_THROWS_AS( parseAndConvert("parsec:1.0"), std::invalid_argument );
}
```

---

## 5. 예외 / 특이 케이스 목록

| TC-ID | 케이스 | 입력 예시 | 기대 동작 | 근거 |
|-------|--------|-----------|-----------|------|
| TC-EX-01 | 빈 문자열 | `""` | `invalid_argument` | 형식 계약 |
| TC-EX-02 | 공백만 | `"   "` | `invalid_argument` | 형식 계약 |
| TC-EX-03 | 단위만, 값 없음 | `"meter:"` | `invalid_argument("invalid number")` | 파싱 실패 |
| TC-EX-04 | 값만, 단위 없음 | `":2.5"` | `invalid_argument("unknown unit: ")` | 미지원 단위 |
| TC-EX-05 | 콜론 두 개 | `"meter:2.5:extra"` | 첫 번째 `:` 기준 파싱 → 정상 처리 or 예외 (정책 TBD) |
| TC-EX-06 | 대소문자 단위 | `"METER:1.0"` | `invalid_argument("unknown unit: METER")` | 대소문자 구분 |
| TC-EX-07 | 정수 입력 | `"meter:3"` | `3.0 meter = 9.842520 feet` (double 처리) | 정수 → double |
| TC-EX-08 | 과학적 표기법 | `"meter:2.5e1"` | `25.0 meter = 82.021000 feet` (허용) | stod 지원 |
| TC-EX-09 | 소수점 6자리 정확도 | `convert("meter",1.0,"feet")` | `3.280840` (6자리) | 출력 포맷 계약 |
| TC-EX-10 | 자기 자신 변환 | `convert("meter",1.0,"meter")` | `1.000000` | 항등 변환 |

---

## 6. 커버리지 목표

### 6.1 레이어별 목표

| 레이어 | 파일 대상 | 라인 커버리지 | 브랜치 커버리지 | 측정 태그 |
|--------|-----------|--------------|----------------|-----------|
| Domain (Entity) | `LengthConverter.cpp`, `UnitRegistry.cpp`, `ConversionRule.cpp` | **≥ 95%** | **≥ 90%** | `[domain]`, `[entity]` |
| Boundary | `InputParser.cpp`, `OutputFormatter.cpp` | **≥ 90%** | **≥ 85%** | `[boundary]` |
| Data | `ConfigLoader.cpp` | **≥ 85%** | **≥ 80%** | `[data]` |
| Control | `Converter.cpp` (오케스트레이터) | **≥ 80%** | **≥ 75%** | `[control]` |
| **TOTAL** | — | **≥ 90%** | **≥ 85%** | — |

### 6.2 커버리지 미달 시 처리

| 상황 | 조치 |
|------|------|
| Domain 라인 < 95% | 미커버 브랜치 추가 TC 작성 후 머지 차단 |
| Boundary 라인 < 90% | 예외 경로 케이스 보강 |
| 전체 < 90% | PR 머지 차단 — 리뷰어 서명 불가 |

---

## 7. gcov / lcov 측정 전략

### 7.1 CMake 커버리지 플래그 설정

```cmake
# CMakeLists.txt 에 추가
option(ENABLE_COVERAGE "Enable gcov coverage" OFF)

if(ENABLE_COVERAGE)
    target_compile_options(UnitConverter PRIVATE --coverage -O0 -g)
    target_link_options(UnitConverter PRIVATE --coverage)
    target_compile_options(UnitConverter_test PRIVATE --coverage -O0 -g)
    target_link_options(UnitConverter_test PRIVATE --coverage)
endif()
```

### 7.2 커버리지 측정 절차

```bash
# Step 1 — 커버리지 플래그로 빌드
cmake -S . -B build -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Step 2 — 테스트 실행 (gcda 파일 생성)
ctest --test-dir build -V

# Step 3 — gcov 원시 데이터 수집
cd build
gcov -r ../UnitConverter.cpp

# Step 4 — lcov HTML 리포트 생성
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/catch2/*' --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_report

# Step 5 — 브라우저에서 확인
# open coverage_report/index.html  (Linux/macOS)
# start coverage_report/index.html  (Windows)
```

### 7.3 레이어별 측정 명령어

```bash
# Domain만 측정
lcov --extract coverage_filtered.info '*/entity/*' '*/control/*' \
     --output-file domain_coverage.info

# Boundary만 측정
lcov --extract coverage_filtered.info '*/boundary/*' \
     --output-file boundary_coverage.info

# 요약 출력
lcov --summary coverage_filtered.info
```

### 7.4 최소 커버리지 강제 (CI)

```bash
# 전체 라인 커버리지 수치 추출 후 임계값 검사
COVERAGE=$(lcov --summary coverage_filtered.info 2>&1 \
           | grep "lines" | awk '{print $2}' | tr -d '%')
if (( $(echo "$COVERAGE < 90" | bc -l) )); then
    echo "❌ Coverage $COVERAGE% < 90% threshold"
    exit 1
fi
echo "✅ Coverage $COVERAGE% passed"
```

---

## 8. 테스트 파일 구조

```
UnitConverter_07/
├── UnitConverter.cpp          ← 프로덕션 코드 (단일 main, RED 시작점)
├── CMakeLists.txt
└── tests/
    ├── test_boundary.cpp      ← Track A: [boundary] 태그
    ├── test_domain.cpp        ← Track B: [domain] 태그
    └── test_data.cpp          ← [data] 태그 (loadConfig)
```

### 8.1 테스트 스켈레톤 예시

```cpp
// tests/test_domain.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

// Track B — TC-B-01
TEST_CASE("convert_meter_2_5_to_feet_returns_8_20210", "[domain]") {
    // Given: 1 meter = 3.28084 feet
    // When:  convert("meter", 2.5, "feet")
    // Then:  result == Approx(8.20210).epsilon(1e-5)
    FAIL("RED");
}

// Track B — TC-B-02
TEST_CASE("convert_meter_1_0_to_yard_returns_1_09361", "[domain]") {
    FAIL("RED");
}

// Track B — TC-B-03
TEST_CASE("convert_feet_1_0_to_meter_returns_0_30480", "[domain]") {
    FAIL("RED");
}
```

```cpp
// tests/test_boundary.cpp
#include <catch2/catch_test_macros.hpp>

// Track A — TC-A-02
TEST_CASE("parse_missing_colon_throws_invalid_argument", "[boundary]") {
    FAIL("RED");
}

// Track A — TC-A-03
TEST_CASE("parse_negative_value_throws_invalid_argument", "[boundary]") {
    FAIL("RED");
}
```

---

## 9. 테스트 실행 명령어

```bash
# 전체 빌드 및 테스트
cmake -S . -B build -DENABLE_TESTING=ON
cmake --build build
ctest --test-dir build -V

# 태그별 실행
./build/UnitConverter_test "[domain]"    # Track B만
./build/UnitConverter_test "[boundary]"  # Track A만

# 실패 목록만 확인
ctest --test-dir build --output-on-failure
```

---

## 10. 완료 기준 (RED → GREEN 진입 조건)

| 기준 | 판정 방법 |
|------|-----------|
| TC-A-01~07, TC-B-01~07 전체 `FAIL("RED")` 상태 확인 | `ctest -V` 14개 FAIL |
| 테스트 파일 git 커밋 완료 | `git log --oneline` 확인 |
| `red` → `B_01` PR 생성 | GitHub PR 링크 |
| 리뷰어 승인 후 머지 | PR Merged 상태 |

---

## 11. 결함 연결

- 결함 발견 시 → `docs/defect_list.md` 에 기록
- 형식: `DEF-{번호}` | Severity | TC-ID | 기대값 | 실제값 | 근본 원인
- README `## RED 단계 To-Do 리스트 > 결함 목록 연결` 체크박스 업데이트 필수
