# PRD — UnitConverter_07

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 상태 | Approved for implementation |
| 기준 | Phase 4 Epic · User Stories · Gherkin · Level 5 체크리스트 · `.cursorrules` |
| 범위 | README 기본·품질·추가 요구, 6시간 실습 |

---

# 1. 프로젝트 개요

## 1.1 한 줄 목적문

| 요소 | 내용 |
|------|------|
| **What** | meter 허브 기반 길이 단위 CLI 변환기와 **계약·Catch2·BCE 레이어** 학습 산출물 |
| **Who** | C++17·클린 아키텍처·TDD 수강생(6시간 실습) |
| **Why** | 환산식 암기가 아니라 **테스트로 고정된 계약**과 **확장 가능한 구조**를 검증 가능한 형태로 완성하기 위함 |

## 1.2 배경 및 문제 정의 (관찰)

학습자는 `단위:값` 입력으로 meter·feet·yard 변환을 기대한다. 초기 코드는 **비율(3.28084, 1.09361)·단위 분기·파싱·stdout**이 한 흐름에 묶여 있어, 단위 추가 시 수정 범위를 예측하기 어렵고 잘못된 stderr/출력이 **정상 결과처럼** 보일 수 있다. 실습 성공은 “실행된다”가 아니라 **golden 계약 PASS·커버리지 임계치·회귀 기준선 유지**로 판정한다.

## 1.3 목표 (측정 가능)

| ID | 목표 | 통과 기준 (측정) |
|----|------|------------------|
| G-01 | Domain 불변식 보호 | Catch2 `[entity]` line ≥ 95%, branch ≥ 90%; 허브 환산 ε ≤ 1e-9 테스트 100% PASS |
| G-02 | Boundary 계약 보호 | golden stderr/stdout 완전 일치 100% PASS; `[boundary]` line ≥ 90%, branch ≥ 85% |
| G-03 | Data 설정 계약 보호 | valid/invalid JSON 시나리오 100% PASS; `[data]` line ≥ 85%, branch ≥ 80% |
| G-04 | README 비율·표시 준수 | `meter:2.5` table exact 3줄 (§6.1) |
| G-05 | 확장 시 Converter 안정 | 신규 단위 추가 후 LengthConverter 단위명 `if` 분기 0건 (리뷰어 정적 확인) |
| G-06 | Control 오케스트레이션 | `[control]` line ≥ 80%, branch ≥ 75% |

## 1.4 비목표 (Non-Goal)

| ID | 비목표 | 경계 (포함하지 않음) |
|----|--------|----------------------|
| NG-01 | UI·네트워크 | GUI, 웹, REST/gRPC, 모바일 |
| NG-02 | 도메인 확장 | 길이 외 물리량, 다중 카테고리 엔진 |
| NG-03 | 운영·품질 외 | 인증, 다국어 UI, SLA·부하·보안 감사, 프로덕션 배포·모니터링 |

**범위 안:** CLI convert/register, meter/feet/yard(+동적 등록), JSON 설정, table/json/csv, Catch2·BCE·6시간 Activities.

---

# 2. 사용자 및 이해관계자

## 2.1 타깃 사용자 (페르소나)

| 항목 | 내용 |
|------|------|
| 이름 | 김학습 (가명) |
| 역할 | C++17 입문~중급, 클린 아키텍처·TDD 수강생 |
| 맥락 | 6시간 실습; AI 초안, **계약·테스트는 human+golden 고정** |
| 성공 정의 | 전 Catch2 PASS + G-01~G-06 + Level 5 체크리스트 |
| 실패 신호 | 테스트 삭제/완화, entity I/O, golden 임의 변경 |

## 2.2 이해관계자

| 이해관계자 | 검증 역할 |
|------------|-----------|
| 학습자 | 테스트 실행·커버리지 제출 |
| 강사/리뷰어 | US AC·Gherkin·체크리스트 서명 |
| AI 보조 | 보일러플레이트만; **계약 변경 권한 없음** |

## 2.3 주요 사용 시나리오 (Journey)

| # | 시나리오 | Journey | 성공 신호 |
|---|----------|---------|-----------|
| S-01 | 계약·RED 진입 | Awareness → Entry | RED ≥ 1건 실패 기록 |
| S-02 | Domain·table GREEN | Action → Validation | G-04 3줄 exact; G-01 충족 |
| S-03 | 설정·등록·회귀 | Validation → Outcome | US-05·06 AC; 전 스위트 0 failure |

---

# 3. 기능 요구사항

## 3.1 핵심 기능 (우선순위)

| 우선순위 | ID | 기능 | Story |
|----------|-----|------|-------|
| 필수 | FR-01 | Convert `unit:value` | US-03, US-04 |
| 필수 | FR-02 | 입력 검증 | US-01 |
| 필수 | FR-03 | Registry 시드 + 허브 환산 | US-02, US-03 |
| 필수 | FR-04 | OCP (분기 없는 환산) | US-02 |
| 권장 | FR-05 | Table 출력 + PRES | US-04 |
| 권장 | FR-06 | JSON 출력 | US-04 |
| 권장 | FR-07 | CSV 출력 | US-04 |
| 권장 | FR-08 | JSON 설정 로드 | US-05 |
| 권장 | FR-09 | 동적 단위 등록 | US-06 |
| 선택 | FR-10 | `--format=table\|json\|csv` | US-04 |
| 선택 | FR-11 | YAML 설정 (FR-08 동등 스키마) | US-05 |

**테스트 최소치 (Story AC):** `[boundary][parse]` ≥ 3 PASS; table/json/csv golden 각 ≥ 3 PASS.

## 3.2 입·출력 계약 (문자열)

### FR-01 Convert

| 항목 | 계약 |
|------|------|
| Input | `{unit}:{value}` — unit `[a-zA-Z][a-zA-Z0-9_]*`, value ≥ 0 |
| Success | exit `0`, stderr empty, stdout §6 |
| Formula | `target = source × (sourceM/targetM)`; Domain 검증 ε ≤ 1e-9 |

**Table golden (seed 3단위):**

```
2.5 meter = 2.5 meter
2.5 meter = 8.2 feet
2.5 meter = 2.7 yard
```

**Domain (반올림 전, US-03):** `meter:2.5` → feet ≈ 8.2021, yard ≈ 2.734025 (ε ≤ 1e-9).

### FR-02 검증 에러 (stderr exact)

| 조건 | exit | stderr |
|------|------|--------|
| 형식 오류 | 1 | `Invalid format. Use unit:value (ex: meter:2.5)` |
| 비숫자 value | 1 | `Invalid number: {token}` |
| value < 0 (NEG-01) | 1 | `Value must be non-negative: {value}` |
| unknown unit | 1 | `Unknown unit: {unit}` |

실패 시 stdout 변환 줄(`* = *`) **0줄**.

### FR-08 설정 로드

| 조건 | exit | stderr |
|------|------|--------|
| 파일 없음/읽기 실패 | 2 | `Failed to load config: {path}` |
| ratio ≤ 0 | 2 | `Invalid config: non-positive ratio for unit {unit}` |
| name 중복 | 2 | `Invalid config: duplicate unit name {unit}` |

### FR-09 동적 등록

| 조건 | exit | stderr |
|------|------|--------|
| 형식 오류 | 1 | `Invalid register format. Use 1 unit = ratio meter (ex: 1 cubit = 0.4572 meter)` |
| 중복 | 1 | `Unit already registered: {unit}` |
| ratio ≤ 0 | 1 | `Ratio must be positive: {ratio}` |

**검증:** `1 cubit = 0.4572 meter` 후 `cubit:1` → meter 0.4572 (ε ≤ 1e-9).

### FR-10 포맷 옵션 (선택)

| Input | 동작 |
|-------|------|
| `--format=table\|json\|csv` | 해당 FR-05~07 |
| 그 외 | exit 1; `Invalid format option. Use table, json, or csv` |

## 3.3 제약 (Gherkin Background 일치)

| Given | 값 |
|-------|-----|
| Hub | meter, 1.0 |
| feet | 3.28084 |
| yard | 1.09361 |
| Display | half-up, 1 decimal (boundary only) |
| Domain ε | 1e-9 |

**NEG-01:** value < 0 거절; value = 0 허용, 모든 target display `0.0`.

**Case:** unit 이름 대소문자 구분 (`Meter` ≠ `meter`).

**PRES:** 출력 좌변·JSON `source` = 입력 unit/value 보존; JSON `source.value` 미반올림.

**Hub only:** feet↔yard 직접 상수 금지.

---

# 4. 비기능 요구사항

## 4.1 기술 스택

| 항목 | 요구 |
|------|------|
| 언어 | C++17 이상 |
| 빌드 | CMake 3.16+ (README g++ 예시는 로컬 단순 빌드; **제출·CI는 CMake**) |
| 테스트 | Catch2 v2.x |
| 포맷 | clang-format |

## 4.2 아키텍처

| 레이어 | 책임 |
|--------|------|
| entity | Registry, Converter, Ratio, DomainError |
| control | ConvertController, RegisterController, StartupLoader |
| boundary | Parser, Renderer, CLI, ErrorMap |
| data | JsonUnitDefinitionReader |

**의존:** boundary → control → entity; control → data; entity → 외부 **금지**.

**OCP:** 신규 단위 = registry; 신규 포맷 = boundary renderer.

## 4.3 커버리지

| 레이어 | Line | Branch |
|--------|------|--------|
| entity | ≥ 95% | ≥ 90% |
| boundary | ≥ 90% | ≥ 85% |
| data | ≥ 85% | ≥ 80% |
| control | ≥ 80% | ≥ 75% |

## 4.4 확장성

| 변경 | 위치 | 금지 |
|------|------|------|
| 새 단위 | config / register | converter `if (unit==…)` |
| 새 포맷 | boundary renderer | entity 수정 |
| 비율 | config / named constexpr | 산재 literal |

---

# 5. 데이터 요구사항

## 5.1 비율 상수 (meter 허브)

| unit | metersPerUnit |
|------|---------------|
| meter | 1.0 |
| feet | 3.28084 |
| yard | 1.09361 |

## 5.2 설정 외부화

- 기본: `config/units.json`
- 테스트: InMemory reader (프로덕션 기본값 대체 아님)

```json
{
  "hub": "meter",
  "units": [
    { "name": "meter", "metersPerUnit": 1.0 },
    { "name": "feet", "metersPerUnit": 3.28084 },
    { "name": "yard", "metersPerUnit": 1.09361 }
  ]
}
```

## 5.3 동적 등록

- 형식: `1 {unit} = {ratio} meter`
- 예: `1 cubit = 0.4572 meter`

---

# 6. 출력 요구사항

## 6.1 Table (기본)

- 줄: `{sourceValue} {sourceUnit} = {displayValue} {targetUnit}`
- PRES-01: 좌변 = 사용자 입력
- 반올림: half-up, 1자리
- 대상: registry 전체

## 6.2 JSON

```json
{
  "source": { "unit": "<input>", "value": <parsed> },
  "conversions": [
    { "unit": "<name>", "value": <display rounded> }
  ]
}
```

- `conversions.length` = 등록 단위 수
- 예: `yard:1.09361` → meter 1.0, feet 3.3, yard 1.1

## 6.3 CSV

- 헤더 (exact): `source_unit,source_value,target_unit,target_value`
- 행 수 = 등록 단위 수; source 열 모든 행 동일

## 6.4 포맷 일관 (FMT-01)

동일 입력·registry에서 table/json/csv **수치 집합 동일**.

---

# 7. 성공 지표

## 7.1 인수 기준

- [ ] AC-01: `meter:2.5` table 3줄 exact (Gherkin #1)
- [ ] AC-02: `meter:abc`, `meter2.5`, `meter:-1`, `mile:1` stderr exact, stdout 0줄 (#3–6)
- [ ] AC-03: `feet:3.28084` PRES 3줄 exact (#2)
- [ ] AC-04: config 유효/무효 exit 2 + exact stderr (US-05)
- [ ] AC-05: cubit 등록 + `cubit:1` (US-06)
- [ ] AC-06: G-01~G-03 커버리지
- [ ] AC-07: G-05 분기 0건
- [ ] AC-08: `meter:0` 모든 target display `0.0`
- [ ] AC-09: boundary parse/json/csv golden 각 ≥ 3

## 7.2 회귀 보호

| ID | 규칙 |
|----|------|
| REG-01 | D-TC 기준선(허브 1m↔feet/yard, meter:2.5 Domain) 실패 시 merge 금지 |
| REG-02 | golden 변경 시 B-TC·I-TC·Gherkin 동시 갱신 |
| REG-03 | DomainError↔exit/message 매핑 변경 시 boundary 테스트 일괄 PASS |
| REG-04 | config schema breaking 시 DT-TC 버전 명시 |
| REG-05 | 단위·포맷 추가 후 **전체** Catch2 0 failure |

---

# 8. Gherkin 추적 (Phase 4)

| # | Scenario | FR |
|---|----------|-----|
| 1 | meter:2.5 table | FR-01, FR-05 |
| 2 | feet PRES | FR-05 |
| 3 | meter:abc | FR-02 |
| 4 | meter2.5 | FR-02 |
| 5 | meter:-1 NEG-01 | FR-02 |
| 6 | mile:1 | FR-02 |
| 7 | yard JSON | FR-06 |
| 8 | yard hub | FR-03, FR-04 |

**Gherkin 미포함 (별도 I-TC 권장):** FR-08 config, FR-09 register, FR-07 CSV, FR-10 invalid option.

---

# 9. 용어 정의

| 용어 | 정의 |
|------|------|
| Hub unit | meter; metersPerUnit=1.0; 불변 |
| metersPerUnit | 1 unit = X meter; X > 0 |
| UnitRegistry | 등록 단위 집합; 중복·빈 이름 거절 |
| ConversionSnapshot | 1회 convert의 전체 target→value (Domain) |
| Golden test | stderr/stdout 완전 문자열 일치 |
| NEG-01 | 음수 거절; 0 허용 |
| PRES | 입력 unit/value 출력 보존 |
| BCE | boundary–control–entity–data + 의존 방향 |

---

# 10. 추적 매트릭스

| PRD | Epic SC | Story | Gherkin |
|-----|---------|-------|---------|
| §1 G-01~06 | SC-01~07 | US-01~06 | #1~8 + I-TC |
| §3 FR-01~11 | SC-04~06 | US AC | §8 |
| §7 AC | SC 전부 | US AC | §8 |
| §7 REG | SC-07 | Journey 7 | — |

---

*Phase 6(README) 이관 시 §3.2 stderr exact, NEG-01, PRES, exit 1/2, §5 JSON, §6 스키마, §7 REG 필수 포함.*
