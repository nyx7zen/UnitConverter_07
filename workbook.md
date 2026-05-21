# PART 2: Code Review 역량 강화와 AI 활용 — Cursor_ai_exercises_3

## UnitConverter Cursor AI 실습 완전 가이드

C++ · Java · Python · Dual-Track TDD · Refactoring · QA

| 항목 | 내용 |
|---|---|
| **버전** | v5.0 (파일 기반 재작성판) |
| **프로젝트** | UnitConverter_07 |
| **저장소** | https://github.com/nyx7zen/UnitConverter_07 |
| **PART 2** | Phase 1~6 — C++ |
| **PART 3** | Dual-Track TDD — RED · GREEN |
| **PART 4** | Dual-Track Refactoring |
| **PART 5** | QA·커버리지 · 최종 보고서 |
| **PART 6** | Git 브랜치 전략 |
| **작성일** | 2026년 5월 20일 |

---

## 전체 구성 안내

본 문서는 UnitConverter_07 (C++/Java/Python) 프로젝트를 기반으로, PART 2(Phase 1~6)부터 PART 6(Git 브랜치 전략)까지 Cursor AI 활용 실습의 모든 단계를 언어별로 작성한 통합 가이드입니다.

| **파트** | **구성** | **핵심 내용** |
|---|---|---|
| PART 2 | Phase 1~6 (UnitConverter_07) | 문제정의→BCE→.cursorrules→Epic/Gherkin→PRD→README/To-Do |
| PART 3 ★ | Dual-Track TDD | RED(실패 테스트) + GREEN(최소 구현) |
| PART 4 ★ | Dual-Track Refactoring | 외부 계약 불변 / GREEN 유지 / 구조만 개선 |
| PART 5 ★ | QA·커버리지 분석 | 커버리지 점검 + 최종 작업 보고서 자동 생성 |
| PART 6 ★ | Git 브랜치 전략 | red, green, refactoring 흐름 |

> **📌 공통 비즈니스 로직 (3개 언어 동일)**
>
> - 입력 형식: `단위:값`  (예: `meter:2.5`)
> - 변환 비율 (meter 허브 기준):
>   - 1 meter = 3.28084 feet
>   - 1 meter = 1.09361 yard
>   - feet/yard 간 비율은 meter 기반으로 계산
> - 출력 예시:
>   - `2.5 meter = 8.202100 feet`
>   - `2.5 meter = 2.734025 yard`

---

# PART 2 · C++ 버전 — UnitConverter_07_C++ — Phase 1 ~ 6

---

## Phase 1 — 방향·Mom Test·문제 정의 (구현 금지)

### 예시 A — 관찰·Why·문제 정의

```
문제 정의 전문가 — C++

당신은 문제 정의 전문가입니다.
워크스페이스: @UnitConverter_07
⚠️ 코드 작성·클래스 설계·알고리즘·빌드 스크립트는 하지 마십시오.

STEP 1 — Observation
1) 단위 변환기(C++)를 사용하는 사람이 무엇 때문에 어려움을 겪는가?
2) 왜 meter/feet/yard 변환인가? 어떤 학습 맥락인가?
3) OCP/SRP, 입력 검증, 설정 외부화, 동적 단위 등록, 출력 포맷과 연결하라.

STEP 2 — Why #1: 정확한 변환 결과(1 meter=3.28084 feet)가 왜 중요한가?
STEP 3 — Why #2: 수동 계산이 아닌 프로그램(+테스트)인 이유?
STEP 4 — Why #3: 계약(입력 형식·허용 단위·실패 이유)을 먼저 고정하려는 이유?

STEP 5 — 문제 정의
1) 표면 문제 1~2문장
2) 개선된 정의 3~7문장
3) 핵심 Invariant 5개 이내 (측정·검증 가능)
4) 훈련하려는 사고 능력 3개

출력: 구조화된 Markdown만. 코드·파일 경로 금지.
```

### 예시 B — Mom Test 인터뷰 질문 10개

```
Mom Test 질문

UnitConverter_C++ 프로젝트를 "고객 인터뷰" 관점에서 준비한다.
현재 코드: 입력(std::string (단위:값)) → if-else 체인 변환 → 콘솔 출력

제품이 아니라 학습자/사용자의 행동을 드러내는 Mom Test 질문 10개.
각 질문: (1) 과거 구체 경험  (2) 실제 행동  (3) 돈/시간/리스크 연결
금지: 기능 나열형 질문만으로 끝내기
마지막에 "진짜 위험 가정" 3개를 bullet로.
구현·코드 금지.
```

---

## Phase 2 — BCE·계약·RED·통합 (구현 금지)

### 예시 A — 전체 설계 패키지

```
BCE 설계 전문가 — C++

Dual-Track(UI/경계 vs Domain) + BCE 관점 설계 전문가로 행동하라.
대상: @UnitConverter_07
현재 코드 상태: 단일 main() 함수, if-else 체인, OCP/SRP 미적용
제약: 구현 코드 작성 금지. 설계·계약·테스트 목록·통합 계획만.

# 1) Entity(Domain) 설계
  1.1 개념 목록과 SRP (ConversionRule, UnitRegistry, Converter)
  1.2 Invariants (예: meter 허브 비율 불변, 중복 단위 등록 금지)
  1.3 유스케이스 (단위 변환, 동적 단위 등록, 설정 파일 로드)
  1.4 Domain API (시그니처 수준, 본문 X) + 실패 조건
  1.5 Domain 단위 테스트 설계 (RED 우선)

# 2) Boundary 설계
  2.1 시나리오: 입력(단위:값) → 검증 → 변환 → 출력
  2.2 외부 계약: Input schema / Output schema / Error schema
  2.3 Boundary 계약 테스트 (Domain Mock 가정)
  2.4 에러 메시지 규칙 (음수·잘못된 형식·없는 단위·중복 단위)

# 3) Data 설계
  3.1 목적 (설정 외부화 — JSON/YAML 로드)
  3.2 인터페이스 계약 (loadRatios/saveRatios 이름만)
  3.3 InMemory vs File(JSON/YAML) 비교 + 추천 1개
  3.4 Data 레이어 테스트

# 4) Integration & Verification
  4.1 의존성 방향 포함 흐름
  4.2 통합 테스트 시나리오 (정상 2+, 실패 3+)
  4.3 회귀 보호 규칙
  4.4 커버리지 목표 (Domain / Boundary / Data 수치)
  4.5 Traceability Matrix

모호어 금지. 모든 규칙은 테스트로 검증 가능하게.
```

### 예시 B — RED 테스트 제목 25~40개 (Catch2)

```
RED 테스트 목록

Catch2를 가정하고 "구현 없이" RED 단계 테스트 케이스 제목 25~40개.
분류:
  파싱: "단위:값" 형식 검증, ':'없음, 빈 문자열, 공백
  음수·0: value < 0 정책, value == 0 허용 여부
  미지원 단위: unknown unit → 오류 메시지
  비율 정확도: 1meter→3.28084feet (소수점 허용 오차)
  동적 단위 등록: "1 cubit = 0.4572 meter" 등록 후 변환
  출력 직렬화: JSON/CSV/표 각 최소 3개씩

각 테스트 옆에 보호하는 Invariant 이름 한 줄.
코드 작성 금지.
```

---

## Phase 3 — .cursorrules 다단계

### 1단계 — 뼈대

```
뼈대 생성

@UnitConverter_07용 .cursorrules YAML 뼈대.
최상위 키만, 값은 비움:
project, code_style, architecture, tdd_rules, testing, forbidden, file_structure, ai_behavior
각 키 앞에 80자 폭 # 구분선 주석.
YAML만 출력.
```

### 2단계 — tdd_rules만

```
tdd_rules 채우기

tdd_rules 블록만 채워라.
C++ UnitConverter 기준:
red_phase / green_phase / refactor_phase 각각에
description, rules(목록), must_not(목록).
YAML만 출력.
```

### 3단계 — 검토만 (수정 금지)

```
검토

완성한 .cursorrules 전체를 검토하라.
수정 제안 금지. 보고만:
YAML 문법 오류 / 누락 섹션 / tdd_rules vs forbidden 충돌 / Cursor가 지키기 어려운 규칙
```

### 4단계 — 전체 채우기

```
완성 YAML

C++ C++17 이상.
code_style: clang-format, 네이밍·헤더/소스 분리 원칙
architecture: boundary/control/entity 정의 + 의존성 방향
testing: Catch2 고정, AAA, fixture 규칙, 커버리지 기준
forbidden:
  std::cout 디버그 남발 / 매직 넘버 하드코딩 / catch(...) 무분별 삼키기 /
  헤더 내 구현 포함 / 전역 상태 남용
  (각각 pattern / reason / alternative)
file_structure: boundary/ control/ entity/ tests/ 트리 주석
ai_behavior: 레이어 위반 금지, 테스트 먼저, 리팩터는 green 이후

설명 없이 YAML 전체만.
```

---

## Phase 4 — Epic → Journey → Story → Gherkin → 체크리스트

### 예시 A — 한 번에 산출물 세트

```
요구사항 서술 패키지 — C++

@UnitConverter_07를 위한 요구사항 서술 패키지.
구현·코드 금지.

Level 1 Epic
  제목: "확장 가능한 C++ 단위 변환 학습 시스템"
  목적 4줄, 성공 기준 측정 가능하게 (커버리지, 계약 테스트, 회귀 정책)

Level 2 User Journey (1개 정본)
  Persona: C++/클린 아키텍처 학습자
  단계 5~7: 문제 인식→계약 정의→도메인 분리→Dual-Track TDD→회귀 보호
  각 단계 Pain / Opportunity 1줄

Level 3 User Stories (최소 6개)
  ST-01: 입력 검증 (음수·잘못된 형식·없는 단위)
  ST-02: meter 허브 기반 정확한 변환 (비율: 3.28084 / 1.09361)
  ST-03: 레지스트리 OCP — 새 단위 추가 시 기존 코드 무변경
  ST-04: 출력 포맷 선택 (JSON / CSV / 표)
  ST-05: 설정 파일(JSON/YAML) 로드 실패 처리
  ST-06: 동적 단위 등록 ("1 cubit = 0.4572 meter")
  각 Story에 Acceptance Criteria는 체크 가능한 bullet

Level 4 Gherkin Feature 1개
  Background: 1 meter = 3.28084 feet, 1 meter = 1.09361 yard
  Scenario: happy path — "meter:2.5" 입력 → 올바른 변환 출력
  Scenario: 잘못된 형식 — ":"없는 입력
  Scenario: unknown unit — "parsec:1.0" 입력
  Scenario: 설정 파일 형식 오류 — 잘못된 JSON/YAML

Level 5 체크리스트
  이 저장소 범위 항목만 / Epic→Journey→Story→Gherkin 정합성 표

Markdown 표와 체크리스트 적극 사용.
```

### 예시 B — Journey + 감정 맵

```
스토리보드

UnitConverter_C++ 사용자 여정을 스토리보드 형식으로.
Stage: Awareness / Entry / Action / Validation / Outcome
각 Stage마다 Action, Thinking, Emotion, Pain, Opportunity 1줄 이상.
목표: 알고리즘이 아닌 계약·테스트·레이어 분리 학습임을 명시.
코드 금지.
```

### 예시 C — Gherkin 시나리오 8개

```
Gherkin 보강

README의 1 meter=3.28084 feet, 1 meter=1.09361 yard를 전제로 Gherkin 8개.
Given-When-Then은 영어 키워드 유지.
반드시 포함:
  - 소수 파싱 실패 ("meter:abc")
  - 음수 입력 정책 (정책 문장 먼저 고정 후 시나리오)
  - 출력에 원 입력 단위/값 보존 (예: "2.5 meter = ...")
  - 동적 단위 등록 후 변환 ("1 cubit = 0.4572 meter")
구현 금지.
```

### 예시 D — Level 5 체크리스트 정화

```
체크리스트 정화

Epic/Journey/Story/Gherkin과 대조해 Level 5 완성도 체크리스트.
저장소 범위 밖 항목 금지.
각 항목: "누가 무엇을 어떻게 검증하면 통과인지" 한 줄.
표: 영역 / 검증 방법 / 통과 기준 / 추적 ID(Story 번호)
코드 금지.
```

---

## Phase 5 — PRD 작성 ★

> **💡 배치 근거:** Phase 4(Epic→Gherkin)가 완성된 뒤 그 산출물을 바탕으로 PRD를 작성합니다. Epic/Story/Gherkin이 검증된 요구사항을 공식 PRD로 정리하여 구현 전 방향을 확정합니다. 이후 Phase 6(README & To-Do)의 입력 자료로 활용됩니다.

### 예시 A — PRD 전체 패키지

```
PRD 전체 패키지 — C++

당신은 PRD 전문가입니다.
워크스페이스: @UnitConverter_07
⚠️ 코드·클래스 설계·빌드 스크립트는 작성하지 마십시오.
전제: Phase 4 Epic/Story/Gherkin을 기반으로 작성하라.

# 1. 프로젝트 개요
  1.1 한 줄 목적문 (What/Who/Why)
  1.2 배경 및 문제 정의 (현재 코드: 단일 main, if-else 체인)
  1.3 목표 3~5개 (측정 가능)
  1.4 비목표 2~3개

# 2. 사용자 및 이해관계자
  2.1 타깃 사용자 (페르소나 1개)
  2.2 주요 시나리오 2~3개 (Phase 4 Journey 기반)

# 3. 기능 요구사항
  3.1 기능 목록 (필수/권장/선택 구분)
      - 필수: 입력 검증, meter 허브 변환, 콘솔 출력
      - 권장: OCP 레지스트리, 동적 단위 등록
      - 선택: JSON/CSV/표 출력, 설정 외부화
  3.2 기능별 입출력 계약 (문자열 계약 수준, 코드 X)
  3.3 제약 사항 (입력 형식, 허용 단위, 음수 정책, 미지원 단위 처리)
  → Phase 4 Gherkin의 Background Given과 반드시 일치

# 4. 비기능 요구사항
  4.1 기술 스택: C++17, CMake, Catch2
  4.2 아키텍처 원칙 (OCP/SRP, BCE 레이어, 의존성 방향)
  4.3 테스트 커버리지 목표 (Domain/Boundary/Data 수치)
  4.4 확장성 원칙 (새 단위 추가 시 기존 코드 변경 최소화)

# 5. 데이터 요구사항
  5.1 단위 비율 상수 (meter 허브: feet=3.28084, yard=1.09361)
  5.2 설정 외부화 방식 (JSON/YAML 로드)
  5.3 동적 단위 등록 계약 ("1 cubit = 0.4572 meter" 형식 명시)

# 6. 출력 요구사항
  6.1 콘솔 기본 포맷 ("2.5 meter = 8.202100 feet")
  6.2 JSON 출력 스키마
  6.3 CSV 출력 스키마
  6.4 표 출력 스키마

# 7. 성공 지표
  7.1 인수 기준 3~5개 (체크박스, 테스트 가능)
  7.2 회귀 보호 규칙 (계약 변경 금지 정책)

# 8. 용어 정의 (Glossary) — 5개 이상

출력: 구조화된 Markdown만. 모호어("적절히") 금지.
```

---

## Phase 6 — README.md & To-Do 리스트 작성 ★

> **💡 배치 근거:** Phase 5(PRD) 완성 후 PRD를 입력 자료로 삼아 README와 To-Do를 작성합니다. README는 현재 단순 코드가 왜 이렇게 설계 개선되는지를 PRD 기반으로 설명하고, To-Do는 PRD 기능 요구사항·인수 기준을 작업 단위로 분해하여 추적합니다.

### 예시 A — README.md 전체 생성

```
README.md 생성 — C++

워크스페이스: @UnitConverter_07
저장소: https://github.com/nyx7zen/UnitConverter_07
전제: Phase 5 PRD를 기반으로 작성하라.
⚠️ 코드 구현 추가 금지.

# UnitConverter (C++)
한 줄 설명 — PRD 1.1 목적문 활용

## 목차 (자동 링크 포함)

## 개요 (Overview)
- 이 프로젝트가 해결하는 문제 (PRD 1.2 배경 기반)
- 주요 학습 목표 (OCP/SRP, BCE, TDD)
- 현재 코드의 문제점과 개선 방향

## 빠른 시작 (Quick Start)
- 사전 조건 (C++ 버전, 빌드 도구)
- 빌드 & 실행:
  g++ -o UnitConverter UnitConverter.cpp
  ./UnitConverter
- 예시 입출력: meter:2.5 → 결과

## 지원 단위 및 비율
표: 단위명 / 식별자 / meter 기준 비율 / 출처
(1 meter = 3.28084 feet, 1 meter = 1.09361 yard)

## 입력 형식 계약
- 정상: 단위:값 예시 3개
- 비정상: 음수·잘못된형식·없는단위 케이스 3개 + 에러 메시지

## 아키텍처
- BCE 레이어 다이어그램 (Mermaid 또는 ASCII)
- 의존성 방향 설명
- 새 단위 추가 방법 (단계별, 코드 최소화)

## 테스트 실행
cmake -S . -B build && cmake --build build
ctest --test-dir build -V

## 설정 파일 (JSON/YAML)
- 위치 및 형식 예시
- 동적 단위 등록: "1 cubit = 0.4572 meter"

## 출력 포맷
콘솔 / JSON / CSV 각 예시 블록 포함

## 생성형 AI 활용 Activities (6시간)
README의 Activities 섹션 내용을 반영하라.

## 기여 가이드 / ## 라이선스 (MIT)

Markdown만. 표·코드 블록·Mermaid 다이어그램 적극 활용.
```

### 예시 B — To-Do 리스트 생성

```
To-Do 리스트 — C++

전제: Phase 5 PRD 기능 요구사항·인수 기준 기반
⚠️ 코드 작성 금지. 작업 목록 문서만.

## 🔴 필수 (Must-Have) — v1.0 차단 항목
- [ ] 입력 파싱 분리 (parse_input / parseInput) │ ST-01 │ 단위 테스트 통과
- [ ] meter 허브 변환 로직 (convert) │ ST-02 │ 비율 오차 1e-5 이내
- [ ] 입력 검증 (음수·잘못된형식·없는단위) │ ST-01 │ 예외 메시지 일치
- [ ] OCP 레지스트리 구조 │ ST-03 │ 새 단위 추가 시 기존 코드 무변경

## 🟡 권장 (Should-Have)
- [ ] 동적 단위 등록 ("1 cubit = 0.4572 meter") │ ST-06 │ 등록 후 변환 가능
- [ ] 설정 파일(JSON/YAML) 로드 │ ST-05 │ 파일 없을 때 기본값 사용

## 🟢 선택 (Nice-to-Have) — v2.0 후보
- [ ] JSON/CSV/표 출력 포맷 선택 │ ST-04 │ 3가지 포맷 정상 출력

## 🔵 기술 부채
- [ ] 단일 main() 함수 분리 │ if-else 체인 제거 │ BCE 레이어로 분리
- [ ] 매직 넘버 3.28084/1.09361 상수화 │ 하드코딩 제거 │ 설정 파일 이관

## ✅ 완료 항목 / ## 📋 회귀 방지 체크리스트 / ## 🗓️ 마일스톤
```

### 예시 C — README ↔ PRD 정합성 검토

```
README vs PRD 검토

완성된 README.md와 Phase 5 PRD를 비교하라.
보고만 (수정 금지):
1) PRD에 있으나 README에 빠진 항목
2) README에 있으나 PRD에 없는 항목 (범위 초과 위험)
3) 입출력 계약 불일치 (특히 비율 3.28084 / 1.09361)
4) 테스트 커버리지 목표 불일치
bullet만.
```

---

## Phase 요약 — UnitConverter (C++)

| **Phase** | **역할** |
|---|---|
| **Phase 1** | C++ 프로젝트의 진짜 문제·가정·Mom Test만 고정 (코드 차단) |
| **Phase 2** | BCE·계약·RED 목록·통합·추적 매트릭스 산출물만 |
| **Phase 3** | .cursorrules를 뼈대→TDD→검토→완성으로 반복 적용 |
| **Phase 4** | Epic→Journey→Story→Gherkin→체크리스트로 요구 추적 |
| **Phase 5 ★** | Phase 4 완료 후 PRD로 방향·목표·계약을 공식 문서로 확정 |
| **Phase 6 ★** | PRD 기반 README & To-Do 리스트로 문서화·작업 추적 마무리 |

---

# PART 3~5 · C++ — Dual-Track TDD · GREEN · Refactoring · QA — UnitConverter_C++

---

## P-04 RED 단계 — Dual-Track 실패 테스트 설계 (C++)

| **목적** | Track A(Boundary) + Track B(Logic) 실패 테스트 동시 설계 |
|---|---|
| **산출물** | Catch2 테스트 파일 스켈레톤 (FAIL("RED") 한 줄) |
| **브랜치** | `red` (A_01에서 분기) |

> ⚠ 구현 코드 작성 금지 / GREEN·REFACTOR 단계 진입 금지
> ⚠ 모든 테스트는 현재 실패 상태여야 함 / Domain은 Mock으로 가정

### Track A / Track B 이중 구조

| **구분** | **Track A — UI / Boundary** | **Track B — Domain / Logic** |
|---|---|---|
| 테스트 대상 | 입력 계약 검증 테스트 | 도메인 로직 단위 테스트 |
| 주요 항목 | "meter:2.5" 정상 / ":"없음 예외 / 음수 예외 / 없는단위 예외 / 반환 포맷 검증 | convert(meter,2.5,feet)=8.202 / convertAll() / registerUnit() / loadConfig() |
| 실행 결과 | 모두 FAIL (구현 없음) | 모두 FAIL (구현 없음) |

---

### 과제 1 — 브랜치 전략 수립

```
깃 브랜치 전략 — C++

@@UnitConverter_07 프로젝트를 구현하기 위한 깃 브랜치 전략을 알려줘.
개발방법론은 Dual-Track TDD 이고, 다음의 순서로 진행할 거야.
- Dual-Track UI + Logic TDD (RED 단계)
- GREEN 단계 (테스트 통과)
- Dual-Track Refactoring
```

**추천 브랜치 구조:**

```
main
└── A_01                     ← 통합 브랜치 (main에서 생성)
      ├── spec                 → PR → A_01 머지 후 삭제
      ├── red                  → PR → A_01 머지 후 삭제
      ├── green                → PR → A_01 머지 후 삭제
      ├── refactoring          → PR → A_01 머지 후 삭제
      └── feature/new_feature  → PR → A_01 머지 후 삭제
최종: A_01 → main  (모든 단계 완료 후 릴리스 PR)
```

---

### 과제 1-A — 샘플 예제 선택 & 테스트 플랜 (RED 단계 준비)

> **💡 목적:** Dual-Track RED 전체 설계를 시작하기 전, PRD와 README에서 샘플 예제 1개를 선택하여 테스트 플랜을 먼저 수립합니다.

#### Step 1 — PRD·README에서 샘플 예제 선택

```
샘플 예제 선택 — C++

@@UnitConverter_07/README.md 와 PRD 문서를 참고하여
테스트 플랜 작성을 위한 샘플 예제 1개를 선택해줘.

선택 기준:
1. README의 기본 요구사항에 명시된 핵심 변환 기능
2. 비즈니스 로직(1 meter = 3.28084 feet, 1 meter = 1.09361 yard)을 직접 검증
3. 정상 경로(happy path)와 경계값 모두 포함 가능한 기능

선택한 예제에 대해 다음을 명시하라:
- 선택한 기능 이름 (예: meter → feet 변환)
- README 요구사항 번호 (예: 기본 요구사항 4번)
- 입력 예시: "meter:2.5"
- 기대 출력: "2.5 meter = 8.202100 feet"
- 이 예제를 선택한 이유 2줄

코드 작성 금지. 선택 결과 문서만 출력.
```

#### Step 2 — 테스트 플랜 작성 (test_plan.md)

```
테스트 플랜 작성 — C++

[P] 시니어 QA 리드입니다.
[C] C++17, CMake, Catch2
[T] 선택한 샘플 예제(meter→feet 변환)를 기반으로 테스트 계획서를 작성해줘.
    포함 항목:
    - Catch2 기반 단위 테스트 범위/우선순위
    - 경계값 케이스 목록:
        · value = 0 (영값 변환)
        · value = 매우 큰 수 (오버플로 위험)
        · value < 0 (음수 입력 정책)
        · 소수점 파싱 실패 ("meter:abc")
        · ":"없는 입력 (형식 오류)
        · 없는 단위 ("parsec:1.0")
    - 예외/특이 케이스 목록
    - 커버리지 목표 (Domain 95%+ / Boundary 85%+)
    - gcov / lcov 측정 전략
[F] Markdown 문서. test_plan.md로 저장.
```

#### Step 3 — README에 RED 단계 To-Do 리스트 추가

```
README — RED 단계 To-Do 리스트 추가 — C++

@@UnitConverter_07/README.md 파일에 아래 섹션을 추가해줘.
기존 내용은 유지하고, "## RED 단계 To-Do 리스트" 섹션만 새로 삽입.

## RED 단계 To-Do 리스트
> 이 체크리스트는 test_plan.md 기반으로 생성되었습니다.
> 각 항목은 RED(실패 테스트 작성) 완료 시 체크합니다.

### Track A — UI / Boundary 테스트
- [ ] TC-A-01: 정상 입력 "meter:2.5" → 변환 결과 반환 (Happy Path)
- [ ] TC-A-02: ":" 없는 입력 → std::invalid_argument 발생
- [ ] TC-A-03: 음수 입력 "meter:-1.0" → std::invalid_argument 발생
- [ ] TC-A-04: 없는 단위 "parsec:1.0" → std::invalid_argument 발생
- [ ] TC-A-05: 소수점 파싱 실패 "meter:abc" → std::invalid_argument 발생
- [ ] TC-A-06: 출력 포맷에 원 입력 단위·값 보존 ("2.5 meter = ...")
- [ ] TC-A-07: value=0 경계값 처리 확인

### Track B — Domain / Logic 테스트
- [ ] TC-B-01: convert("meter", 2.5, "feet") == 8.20210 (오차 1e-5)
- [ ] TC-B-02: convert("meter", 1.0, "yard") == 1.09361 (오차 1e-5)
- [ ] TC-B-03: convert("feet", 1.0, "meter") == 0.30480 (역변환)
- [ ] TC-B-04: convertAll("meter", 1.0) → 모든 등록 단위 변환 반환
- [ ] TC-B-05: registerUnit("cubit", 0.4572) 후 변환 가능
- [ ] TC-B-06: loadConfig(유효한 경로) → 비율 정상 로드
- [ ] TC-B-07: loadConfig(없는 경로) → 기본값(3.28084/1.09361) 유지

### 커버리지 목표
- [ ] Domain Logic: 95%+ (gcov / lcov)
- [ ] Boundary Layer: 85%+
- [ ] 전체 TOTAL: 90%+

### 결함 목록 연결
- [ ] defect_list.md 생성 및 발견 결함 기록
- [ ] 모든 결함 수정 후 회귀 테스트 통과 확인
```

#### Step 4 — 테스트 케이스 작성 (아이템 타입별 최소 5개)

```
테스트 케이스 작성 — C++

[P] 테스트 설계에 강한 시니어 C++ QA입니다.
[C] C++17, CMake, Catch2
[T] 아이템 타입별 최소 5개 테스트를 작성해줘.
    대상 케이스:
    - 정상 변환: meter→feet, meter→yard, feet→meter (역변환)
    - 경계값: value=0 / 매우 큰 수 / 소수점 6자리 정확도
    - 예외: 잘못된 형식 / 음수 / 없는 단위
    - 동적 등록: registerUnit 후 변환
    - 설정 로드: JSON/YAML 정상·실패 케이스
    형식 규칙:
    - Catch2 사용
    - Given-When-Then 주석 구조
    - 각 테스트에 변환 비율 명시 (1 meter = 3.28084 feet)
    - 테스트 이름: test_[변환타입]_[조건]_[기대결과] 형식
[F] 완성된 테스트 코드.
    cmake -S . -B build && cmake --build build가 Green이 되게 작성.
```

**Catch2 테스트 예시:**

```cpp
#include <catch2/catch_test_macros.hpp>

TEST_CASE("meter를 feet으로 변환한다", "[domain]") {
    REQUIRE( convert("meter", 1.0, "feet") == Approx(3.28084) );
}
```

주요 단언 매크로:
- `REQUIRE`: 조건 실패 시 즉시 테스트 중단
- `CHECK`: 실패해도 테스트 계속 실행
- `REQUIRE_THROWS`: 특정 코드 블록이 예외를 던지는지 검증

**UnitConverter 프로젝트에서의 위치:**

```
UnitConverter_07/
├── UnitConverter.cpp      ← 프로덕션 코드
└── tests/
    └── test_converter.cpp ← Catch2 테스트 파일
```

RED 단계에서는 테스트 파일에 이렇게 씁니다:

```cpp
TEST_CASE("meter를 feet으로 변환한다", "[domain]") {
    FAIL("RED");  // 구현 전, 일부러 실패시키는 상태
}
```

GREEN 단계에서 `FAIL("RED")`를 실제 구현으로 채우는 방식

#### Step 5 — 테스트 실행 & 결함 분석

```
5-A 실패 로그 기반 결함 분석 — C++

[P] 디버깅과 결함 분석에 능한 C++ QA 엔지니어입니다.
[T] (여기에 cmake -S . -B build && cmake --build build 실패 로그를 붙여넣을 것)
    1) 테스트 실패의 기대/실제 차이 요약
       예: expected 8.202100 but got 0.000000
    2) 버그 위치 특정 (파일명:줄번호)
       예: UnitConverter.cpp:27
    3) 결함 심각도 분류 및 근거:
       Critical   — 변환 결과가 완전히 틀린 경우
       Major      — 경계값에서 오차 초과
       Minor      — 출력 포맷 불일치
       Info       — 코드 스타일/주석 문제
    4) 최소 변경 수정 방안 (C++ 스타일)
       - if-else 체인 유지 최소 수정 (GREEN 통과 우선)
       - Item/기본 구조체 수정 금지
[F] 수정 diff 제안 + cmake -S . -B build && cmake --build build Green 확인 절차
```

```
5-B 결함 목록 문서화 — C++

[P] QA 리드입니다.
[C] 현재까지 발견된 테스트 실패/결함들을 문서화합니다.
[T] 발견된 결함을 defect_list.md에 정리해줘.
    항목 형식:
    │ ID │ Severity │ 변환 타입 │ 재현 절차 │ 기대값 │ 실제값 │ 근본 원인 │ 수정 요약 │
    예시:
    │ DEF-001 │ Critical │ meter→feet │ "meter:2.5" 입력 │ 8.202100 │ 0.000000 │
    │ if-else 체인에서 feet 분기 누락 │ meterValue * 3.28084 추가 │
[F] Markdown. defect_list.md로 저장.
    README의 "RED 단계 To-Do 리스트 > 결함 목록 연결" 체크박스도 업데이트.
```

---

### 과제 2 — Dual-Track RED 전체 설계

```
Dual-Track RED 전체 설계 — C++

당신은 Dual-Track UI + Logic TDD 전문가입니다.
프로젝트: @UnitConverter_07
현재 코드 상태: 단일 main() 함수, if-else 체인, 예외 처리 없음
중요: 구현 코드 작성 금지 / GREEN 단계 금지 / REFACTOR 금지
오직 RED 테스트만 작성하십시오.

프로젝트 조건:
- 입력: std::string (단위:값) (예: "meter:2.5")
- 변환: 1 meter = 3.28084 feet, 1 meter = 1.09361 yard
- 출력: "2.5 meter = 8.202100 feet" 형식
- 예외: std::invalid_argument

TRACK A — UI / Boundary RED:
1. "meter:2.5" 정상 입력 → 변환 결과 반환
2. ":" 없는 입력 → 예외 발생 (잘못된 형식)
3. 음수 값 입력 ("meter:-1.0") → 예외 발생
4. 없는 단위 ("parsec:1.0") → 예외 발생
5. 반환 포맷에 원 입력 단위·값 보존 확인
6. JSON 출력 요청 시 올바른 스키마 반환
각 테스트에: 테스트 이름 / Given / When / Then / 보호하는 계약

TRACK B — Domain / Logic RED:
1. convert(fromUnit, value, toUnit) — meter↔feet 정확도 (1e-5 이내)
2. convert(fromUnit, value, toUnit) — meter↔yard 정확도
3. convertAll(fromUnit, value) — 모든 등록 단위로 변환 반환
4. registerUnit(name, ratio_to_meter) — 새 단위 등록 후 변환 가능
5. loadConfig(path) — JSON/YAML 설정 파일 로드 후 비율 적용
6. loadConfig(invalid_path) — 파일 없을 때 기본값 유지
각 테스트: 실패 상태 명확히 설명 / 보호 Invariant 명시

출력 형식:
# UI RED Tests — Test ID / Given/When/Then / Invariant
# Logic RED Tests — Test ID / Scenario / Invariant
```

---

### 과제 3 — 테스트 스켈레톤 (Catch2)

```
테스트 스켈레톤

Catch2 테스트 스켈레톤을 작성하라.
파일/클래스 이름만 적고, 메서드 본문은 FAIL("RED") 한 줄만.
RED 원칙으로만 작성하십시오.

예시:
TEST_CASE("convert_meter_to_feet_returns_correct_ratio","[domain]") {
  FAIL("RED");
}
```

**환경 설정 및 테스트 실행:**

```bash
# CMake + Catch2 설정
cmake -S . -B build -DENABLE_TESTING=ON
cmake --build build
ctest --test-dir build -V
```

---

## P-05/P-06 GREEN 단계 (C++)

| **목적** | 선택한 RED 테스트 1묶음만 통과 / 기타 범위 확장 금지 |
|---|---|
| **브랜치** | `green` (A_01에서 분기) |
| **커밋 단위** | RED 1묶음 선택 → 최소 구현 → 테스트 통과 → 커밋 |

> ⚠ 테스트 약화·삭제 금지 / REFACTOR 이번 커밋에서 금지 / 매직 넘버 3.28084/1.09361 상수화 필수
> ⚠ 하드코딩 추가 금지 / 한 커밋에 "모든 RED 해결" 금지 / 도메인에 UI/출력 코드 혼입 금지

### GREEN TDD Cycle 흐름

| **단계** | **작업** | **결과물** |
|---|---|---|
| ① RED 선택 | 실패하는 테스트 1묶음 선택 (예: convert meter→feet) | 선택된 테스트 ID |
| ② 최소 구현 | 선택된 RED만 통과하는 최소 코드 | 프로덕션 코드 변경 |
| ③ 테스트 실행 | `cmake -S . -B build && cmake --build build` | PASS 결과 |
| ④ 커밋 | Conventional Commit 메시지로 커밋 | 커밋 해시 |
| ⑤ 반복 | 다음 RED 테스트로 이동 | 다음 사이클 |

### 과제 GREEN-A — 브랜치 준비 (C++)

```bash
# RED → A_01 PR 머지 완료 후
git checkout A_01
git pull origin A_01
git checkout -b green
git push --set-upstream origin green
# 현재 브랜치 확인
# git branch  →  * green
```

### 과제 GREEN-B — 첫 번째 AC 통과 (C++)

```
GREEN 첫 번째 AC — C++

[단계] TDD GREEN만 수행한다. REFACTOR는 하지 않는다.
프로젝트: @UnitConverter_07
대상 테스트: TC-B-01
  파일: tests/test_converter.cpp
  내용: REQUIRE( convert("meter", 2.5, "feet") == Approx(8.20210).epsilon(1e-5) );

PRD §3.2 비즈니스 로직:
  1 meter = 3.28084 feet
  1 meter = 1.09361 yard
  (비율은 상수로 추출 — 인라인 하드코딩 금지)

할 일:
1. cmake -S . -B build && cmake --build build 실행 → TC-B-01 FAIL 확인
2. convert() 함수 최소 구현 (meter→feet 분기만)
3. cmake -S . -B build && cmake --build build 재실행 → TC-B-01 PASS 확인

금지:
- 다른 TC (TC-B-02~07, TC-A-01~07) 동시 구현
- REFACTOR 이번 커밋에서 금지
- 비율 3.28084 인라인 사용 금지 (상수 정의 후 사용)

산출물:
- TC-B-01이 통과하는 최소 프로덕션 코드
- 커밋 메시지 제안: feat(green): implement convert meter to feet (Approx 1e-5)
```

### 과제 GREEN-C — Dual-Track 병렬 GREEN (C++)

| **커밋 순서** | **Track** | **대상 TC** | **구현 내용** | **커밋 메시지** |
|---|---|---|---|---|
| 1번째 | Track B | TC-B-01 | convert("meter",v,"feet") 최소 구현 | `feat(green): meter to feet` |
| 2번째 | Track A | TC-A-02 | ":" 없는 입력 → 예외 반환 | `feat(green): validate missing colon` |
| 3번째 | Track B | TC-B-02 | convert("meter",v,"yard") 추가 | `feat(green): meter to yard` |
| 4번째 | Track A | TC-A-03 | 음수 입력 → 예외 반환 | `feat(green): validate negative value` |
| 5번째 | Track B | TC-B-03 | convert("feet",v,"meter") 역변환 | `feat(green): feet to meter reverse` |
| 6번째 | Track A | TC-A-04 | 없는 단위 → 예외 반환 | `feat(green): validate unknown unit` |
| 7번째 | Track B | TC-B-04~05 | convertAll() + registerUnit() | `feat(green): convertAll and registerUnit` |
| 8번째 | Track A | TC-A-01,06,07 | Happy path + 포맷 + 경계값 | `feat(green): boundary happy path` |
| 9번째 | Track B | TC-B-06~07 | loadConfig() 정상·실패 | `feat(green): loadConfig with fallback` |

```
Dual-Track GREEN 반복 사이클 — C++

당신은 Dual-Track UI(Boundary) + Logic(Domain) TDD 실무 전문가입니다.
프로젝트: @UnitConverter_07
이번 커밋에서 처리할 묶음을 1개 선택하고 아래 순서로 진행하라:

Step 1) 대상 TC 확인 (위 커밋 순서표 참고)
Step 2) cmake -S . -B build && cmake --build build → 대상 TC가 FAIL인지 확인
Step 3) 대상 TC만 통과하는 최소 구현 작성
Step 4) cmake -S . -B build && cmake --build build 재실행 → 대상 TC PASS, 나머지 영향 없음 확인
Step 5) 커밋

출력 형식:
1) 이번 커밋 목표 (Track A 또는 B, TC ID)
2) 추가/수정한 코드 범위
3) 테스트 결과 (PASS/FAIL 목록)
4) 커밋 메시지 (Conventional Commit)
5) 다음 커밋 후보 TC
```

### 과제 GREEN-D — 전체 GREEN 확인 및 PR (C++)

```
전체 GREEN 확인 — C++

모든 TC(TC-A-01~07, TC-B-01~07)가 통과한 후 전체 테스트를 실행하라.

cmake -S . -B build && cmake --build build
ctest --test-dir build -V

커버리지 측정:
gcov UnitConverter.cpp

확인 항목:
- 모든 TC PASS (0 failures)
- Domain Logic 커버리지 ≥ 95%
- Boundary Layer 커버리지 ≥ 85%
- 비율 상수 3.28084 / 1.09361이 인라인으로 남아 있지 않음
- main() 함수에 변환 로직이 남아 있지 않음 (Domain으로 분리됨)

이후: green → A_01 PR 생성 → 리뷰 → 머지
```

### GREEN 프롬프트 例1 — 간결 버전 (C++)

```
GREEN 최소 구현

[단계] TDD의 GREEN만 수행한다. REFACTOR는 하지 않는다.
프로젝트: @UnitConverter_07
전제: 현재 워킹 트리에 RED 실패 테스트가 있다.
먼저 테스트가 의도한 이유로 실패하는지 확인한 뒤 작업한다.

할 일:
1. 실패하는 테스트를 통과시키기 위한 최소한의 코드만 추가·수정한다.
2. 범위 밖 요구사항(다른 AC, 출력 포맷, 리팩터링)은 하지 않는다.
3. GREEN 완료 후 해당 테스트 재실행하여 통과를 확인한다.

금지: 테스트 약화·삭제 / 의미 없는 하드코딩 / 3.28084·1.09361 인라인 상수

산출물:
- 변경된 프로덕션 코드만
- 짧은 요약: 어떤 함수를 최소로 채웠는지 2~3문장
```

### GREEN 프롬프트 例2 — Dual-Track 상세 버전 (C++)

```
Dual-Track GREEN 상세

당신은 Dual-Track UI(Boundary) + Logic(Domain) TDD 실무 전문가입니다.
프로젝트: @UnitConverter_07 / 이번 작업: "하나의 커밋" 단위

중요 규칙:
- 순서 준수: RED 선택 → GREEN 최소 구현 → 테스트 실행 → 통과 확인 → 커밋
- 1개 RED 테스트 묶음만 처리
- REFACTOR는 이번 커밋에서 금지
- 비율 상수 3.28084 / 1.09361은 인라인 하드코딩 금지 (상수 또는 설정 사용)
- 도메인 레이어에 UI/출력 코드 혼입 금지

고정 계약:
- 입력: std::string (단위:값) — "단위:값" 형식
- 출력: "값 단위 = 변환값 변환단위" 형식
- 예외: std::invalid_argument

Step 1) 이번 커밋에서 선택할 RED 테스트 1묶음:
  [TRACK B] convert(meter, 2.5, feet) → 8.202100 (오차 1e-5)
  [TRACK A] ":" 없는 입력 → std::invalid_argument 발생
  선택 이유를 2줄로 설명하십시오.
Step 2) GREEN 최소 구현 (Dual-Track 병렬)
Step 3) 테스트 실행 및 통과 확인

출력 형식:
1) 이번 커밋 목표  2) 선택한 RED 테스트  3) GREEN 최소 구현 범위
4) 실행한 테스트 목록  5) 결과 요약  6) 커밋 메시지 (Conventional Commit)
7) 변경 파일 목록  8) 다음 커밋 RED 후보 2개

주의: REFACTOR 금지 / 한 커밋에 모든 RED 해결 금지
```

---

## P-06-GM Golden Master 자동화 — 회귀 안전장치 (C++)

| **목적** | UnitConverter 출력 기반 Golden Master 회귀 테스트 구축 및 CI 연동 |
|---|---|
| **산출물** | test_golden_master.cpp + golden_master_expected.txt |
| **브랜치** | `refactoring` (A_01에서 분기) |
| **타이밍** | GREEN 단계 완료 후, Refactoring(P-07/P-08) 시작 직전 구축 |

> **💡 Golden Master란?** 현재 시스템의 실제 출력(stdout)을 "기준 파일"로 저장해두고, 코드 변경(Refactoring) 후에도 출력이 동일한지 자동으로 비교하는 회귀 테스트 기법.
>
> UnitConverter에서의 역할:
> - ① "meter:2.5" 등 대표 입력의 출력 전체를 기준 파일에 저장
> - ② Refactoring(if-else→Registry 교체 등) 후에도 출력 불변 자동 검증
> - ③ 비율 상수(3.28084/1.09361) 오기입·오삭제를 조기 탐지
> - ④ CI에서 PR 머지 전 자동 실행 → 회귀 차단

### Golden Master 설계 개요

| **항목** | **내용** |
|---|---|
| 기준 출력 파일 | `tests/golden_master_expected.txt` |
| 실제 출력 파일 | `tests/golden_master_actual.txt` |
| 출력 캡처 방식 | stdout 리디렉션: `./UnitConverter < input.txt > actual.txt` |
| 파일 비교 방식 | `std::ifstream`으로 파일 읽어 문자열 비교 |
| 테스트 마킹 | `TEST_F(GoldenMasterTest, UnitConverter_meter_2_5)` |
| CI 실행 명령어 | `cmake --build build && ctest --test-dir build -R GoldenMaster` |

### Golden Master 입력 시나리오 (기준 출력)

| **입력** | **기준 출력 (expected)** | **검증 포인트** |
|---|---|---|
| "meter:2.5" | `2.5 meter = 8.202100 feet`<br>`2.5 meter = 2.734025 yard` | 비율 3.28084·1.09361 동시 검증 |
| "feet:1.0" | `1.0 feet = 0.304800 meter`<br>`1.0 feet = 0.333333 yard` | 역변환(feet→meter) 정확도 |
| "yard:1.0" | `1.0 yard = 0.914400 meter`<br>`1.0 yard = 3.000000 feet` | 역변환(yard→meter·feet) 정확도 |
| "meter:0.0" | `0.0 meter = 0.000000 feet`<br>`0.0 meter = 0.000000 yard` | 경계값 0 출력 형식 |

### 과제 GM-1 — 기준 출력 파일 생성 및 보관

```
GM-1 기준 파일 생성 — C++

[P] 회귀 테스트(Approval/Golden Master) 설계 전문가입니다.
[C] C++17, CMake, Catch2
[T] UnitConverter 출력 기반 Golden Master 기준 파일을 생성해줘.
    1) 기준 출력(tests/golden_master_expected.txt) 생성/보관 전략
       입력 시나리오 4개:
         "meter:2.5" / "feet:1.0" / "yard:1.0" / "meter:0.0"
       캡처 방식: stdout 리디렉션: ./UnitConverter < input.txt > actual.txt
       생성 후: git add tests/golden_master_expected.txt  (버전 관리 필수)
    2) approve 패턴 적용:
       - 기준 파일 없으면 → 현재 출력을 기준으로 자동 생성
       - 기준 파일 있으면 → actual vs expected 문자열 비교
       - 불일치 시: diff 출력 후 테스트 FAIL
    3) 기준 파일 구조:
       [meter:2.5]
       2.5 meter = 8.202100 feet
       2.5 meter = 2.734025 yard
       ---
       [feet:1.0]
       ...
[F] tests/golden_master_expected.txt 파일 내용 + 생성 스크립트
```

### 과제 GM-2 — 테스트 코드 구현

```
GM-2 테스트 코드 — C++

[P] 회귀 테스트(Approval/Golden Master) 설계 전문가입니다.
[C] C++17, CMake, Catch2
[T] UnitConverter Golden Master 테스트 코드를 작성해줘.
    구현 요건:
    1) 출력 캡처: stdout 리디렉션: ./UnitConverter < input.txt > actual.txt
    2) 파일 비교: std::ifstream으로 파일 읽어 문자열 비교
    3) 테스트 마킹: TEST_F(GoldenMasterTest, UnitConverter_meter_2_5)
    4) 빌드/실행:
       cmake --build build
       ctest --test-dir build -R GoldenMaster
    5) (선택) ApprovalTests.cpp 라이브러리 적용 가능

    테스트 케이스 (4개):
    - GM-TC-01: "meter:2.5" 출력 == 기준 파일 [meter:2.5] 섹션
    - GM-TC-02: "feet:1.0"  출력 == 기준 파일 [feet:1.0]  섹션
    - GM-TC-03: "yard:1.0"  출력 == 기준 파일 [yard:1.0]  섹션
    - GM-TC-04: "meter:0.0" 출력 == 기준 파일 [meter:0.0] 섹션

    실패 시 출력:
    --- expected
    +++ actual
    @@ 라인별 diff 표시 @@
[F] test_golden_master.cpp + golden_master_expected.txt 완성 코드
```

**코드 스켈레톤 예시 — C++:**

```cpp
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

class GoldenMasterTest : public ::testing::Test {};

TEST_F(GoldenMasterTest, meter_2_5_matches_golden_master) {
  // Given: 기준 출력 파일 로드
  std::ifstream expected_file("tests/golden_master_expected.txt");
  std::string expected((std::istreambuf_iterator<char>(expected_file)),
                        std::istreambuf_iterator<char>());
  // When: UnitConverter 실행 후 출력 캡처
  std::string actual = captureOutput("meter:2.5");  // 구현 필요
  // Then: 출력이 기준과 동일한지 확인
  EXPECT_EQ(expected, actual);
}
```

### 과제 GM-3 — README Golden Master To-Do 체크리스트 추가

```
GM-4 README 업데이트 — C++

@@UnitConverter_07/README.md 파일에 아래 섹션을 추가해줘.
"## RED 단계 To-Do 리스트" 아래에 새 섹션으로 삽입.

## Golden Master 회귀 안전장치
> Refactoring 시작 전 구축. GREEN 완료 후 즉시 적용.

### 기준 파일 생성
- [ ] GM-01: golden_master_expected.txt 생성 (meter:2.5 기준 출력)
- [ ] GM-02: feet:1.0 / yard:1.0 / meter:0.0 시나리오 추가
- [ ] GM-03: git add tests/golden_master_expected.txt (버전 관리 포함)

### 테스트 코드
- [ ] GM-04: test_golden_master.cpp + golden_master_expected.txt 작성
- [ ] GM-05: approve 패턴 적용 (파일 없으면 생성, 있으면 비교)
- [ ] GM-06: CMake: add_test(NAME GoldenMaster COMMAND UnitConverter_test) → PASS 확인

### CI 연동
- [ ] GM-07: .github/workflows/golden_master.yml 작성
- [ ] GM-08: PR 머지 차단 (required status check) 설정
- [ ] GM-09: Refactoring 후 Golden Master 재실행 → PASS 확인
```

---

## P-07/P-08 Dual-Track Refactoring (C++)

| **목적** | 외부 계약 불변 / GREEN 유지 / 구조만 개선 |
|---|---|
| **브랜치** | `refactoring` (A_01에서 분기) |
| **커밋 단위** | 리팩토링 목표 1~2개 선택 → 수행 → 회귀 확인 → 커밋 |

### 과제 REF-A — 브랜치 준비 (C++)

```bash
# GREEN → A_01 PR 머지 완료 후
git checkout A_01
git pull origin A_01
git checkout -b refactoring
git push --set-upstream origin refactoring

# 시작 전 반드시 기준선 확인
cmake -S . -B build && cmake --build build  →  모두 PASS 확인
```

### Step 1 — 기준선 확인 [Ask 모드] (C++)

```
[Ask 모드] 기준선 확인

아직 코드는 수정하지 말고 분석만 해줘.
@UnitConverter.cpp 를 보고 아래 두 가지만 먼저 답해줘:
1. 현재 테스트 파일(test_*.cpp)이 존재하는지 확인하고,
   없다면 어느 파일에 대한 테스트가 필요한지 알려줘
2. 테스트 없이 리팩토링을 시작하면 안 되는 이유를 한 줄로 설명해줘
```

### Step 2 — 코드 스멜 탐지 [Ask 모드] (C++)

```
[Ask 모드] 코드 스멜 분석

@UnitConverter.cpp 의 코드 스멜을 분석해줘.
코드는 절대 수정하지 말고, 문제 있는 부분만 아래 표 형식으로:
│ 파일명 │ 줄번호 │ 스멜 종류 │ 문제 설명 │ 우선순위 │

점검 항목:
- 단일 main() 함수에 파싱·검증·변환·출력이 혼재 (SRP 위반)
- if-else 체인 (새 단위 추가 시 기존 코드 수정 필요, OCP 위반)
- 매직 넘버 3.28084 / 1.09361 하드코딩
- 예외 처리 없음 또는 불완전한 예외 처리
- 긴 함수 (20줄 초과)
- 사용하지 않는 변수·함수
```

### Step 3 — ECB 책임 분리 분석 [Ask 모드] (C++)

```
[Ask 모드] ECB 분석

@UnitConverter.cpp 를 ECB(Entity-Control-Boundary) 패턴으로 분석해줘.
코드는 수정하지 말고 다음만 답해줘:
1. 현재 main() 함수에서 ECB 역할이 어떻게 혼재하는가?
   - Entity: 변환 비율 데이터 (3.28084, 1.09361)
   - Control: 변환 로직 (if-else 체인)
   - Boundary: 입력 파싱, 출력 포맷
2. 어느 코드를 어느 클래스/모듈로 이동해야 하는가?
3. 새 단위 추가 시 변경이 최소화되는 구조를 제안하라.
```

### Step 4 — 종합 리팩토링 계획서 [Ask 모드] (C++)

```
[Ask 모드] 리팩토링 계획서

앞서 분석한 내용을 바탕으로 리팩토링 계획서를 작성해줘.
코드는 아직 수정하지 말고:

## 리팩토링 대상 목록 (우선순위 순)
│ 순번 │ 대상 │ 문제 │ 적용 기법 │ 우선순위 │

## 리팩토링 후보 목록
R-U1: 입력 파싱을 InputParser로 분리
R-U2: 예외 메시지·코드 상수화
R-U3: 출력 포맷 생성을 OutputFormatter로 분리
R-L1: 변환 비율을 ConversionRule Value Object로 교체
R-L2: if-else 체인을 UnitRegistry(Map/Dictionary)로 교체
R-L3: 매직 넘버 3.28084/1.09361 → 상수 또는 설정 파일
R-L4: convert() 함수 추출 (meter 허브 변환 로직 단일화)

## 테스트 선행 필요 항목
## 리팩토링 후 검증 방법 (회귀 테스트 실행 명령어)
```

---

## P-08 Dual-Track Refactoring 실행 (C++)

| **ID** | **Track A — UI / Boundary** | **Track B — Domain / Logic** |
|---|---|---|
| R-U1 | 입력 파싱을 InputParser로 분리 | R-L1: 변환 비율을 ConversionRule VO로 교체 |
| R-U2 | 예외 메시지·코드 상수화 | R-L2: if-else → UnitRegistry (Map 기반) |
| R-U3 | 출력 포맷을 OutputFormatter로 분리 | R-L3: 3.28084/1.09361 → 상수/설정 파일 |
| | | R-L4: convert() 함수 추출 (meter 허브 단일화) |

| **ID** | **Track** | **리팩토링 목표** | **커밋 메시지** |
|---|---|---|---|
| R-L1 | Domain | if-else 체인 → UnitRegistry (Map/Dict 기반) | `refactor(domain): replace if-else with UnitRegistry` |
| R-L2 | Domain | 3.28084/1.09361 → METER_TO_FEET/METER_TO_YARD 상수 | `refactor(domain): extract conversion ratio constants` |
| R-L3 | Domain | convert() 함수 추출 (meter 허브 단일 경로) | `refactor(domain): extract convert() with meter hub` |
| R-L4 | Domain | convertAll() 분리 (Registry 순회) | `refactor(domain): extract convertAll() from main` |
| R-U1 | Boundary | InputParser 분리 (파싱·검증 책임 분리) | `refactor(boundary): extract InputParser` |
| R-U2 | Boundary | 예외 메시지 상수화 (ErrorCode 단일화) | `refactor(boundary): extract error message constants` |
| R-U3 | Boundary | OutputFormatter 분리 (출력 포맷 책임 분리) | `refactor(boundary): extract OutputFormatter` |

```
Dual-Track REFACTOR — 커밋 1개 단위 리팩토링 계속

당신은 Dual-Track UI + Logic TDD 및 리팩토링 전문가입니다.
프로젝트: @UnitConverter_07
이번 작업: 커밋 1개 단위의 Dual-Track REFACTOR만 수행

절대 규칙:
- 새 기능 추가 금지
- 외부 계약(입력 "단위:값" 형식, 출력 "값 단위 = 변환값 단위", 예외 타입) 변경 금지
- 테스트 삭제·완화 금지
- 먼저 테스트 실행 → 리팩토링 → 테스트 재실행 (항상 GREEN 유지)
- 3.28084 / 1.09361 인라인 하드코딩 추가 금지
- Dual-Track 분리:
    Boundary(UI Track): 파싱·검증·포맷 책임만
    Domain(Logic Track): 순수 변환 규칙·계산 책임만

커밋 단위 절차:
Step 0) 전체 테스트 실행 → GREEN 상태 확인
Step 1) 리팩토링 목표 1~2개 선택 (가장 작은 단위)
Step 2) 보호 테스트 점검·보강
Step 3) Dual-Track 리팩토링 수행 (최소 변경)
Step 4) 전체 테스트 재실행 → 모두 통과 확인
Step 5) 커밋 준비 산출물 작성

출력 형식:
1) 이번 커밋 리팩토링 목표 (선택 항목 ID)
2) 변경 범위 요약 (UI Track / Logic Track 분리)
3) 변경 전 문제점 → 변경 후 개선점
4) 수정된 파일 목록
5) 테스트 실행 결과 요약
6) 위험 요소 및 롤백 포인트
7) 커밋 메시지 제안 (Conventional Commit)
   예: refactor(domain): extract UnitRegistry and ConversionRule VO
```

### 과제 — 최종 회귀 확인 및 PR (C++)

```
최종 회귀 확인 — C++

모든 리팩토링 커밋 완료 후 전체 테스트와 커버리지를 확인하라.

cmake -S . -B build && cmake --build build
ctest --test-dir build -V
gcov UnitConverter.cpp

확인 항목:
- 모든 TC PASS (TC-A-01~07, TC-B-01~07)
- Golden Master 테스트도 PASS (출력 불변 확인)
- if-else 체인 제거 완료 (UnitRegistry 교체)
- 매직 넘버 3.28084/1.09361 인라인 없음
- Domain(변환 로직)과 Boundary(파싱·출력)가 분리됨
- Domain 커버리지 ≥ 95% / Boundary ≥ 85%

이후: refactoring → A_01 PR 생성 → 리뷰 → 머지
```

---

## P-09/P-10 QA·커버리지·최종 보고서 (C++)

### 커버리지 기준표

| **레이어** | **목표** | **경고 기준** | **측정 명령어** |
|---|---|---|---|
| Domain Logic | 95%+ | < 90% | gcov / lcov |
| Boundary Layer | 85%+ | < 80% | gcov / lcov |
| 전체 (TOTAL) | 90%+ | < 85% | gcov / lcov |

### P-09 커버리지 점검 프롬프트

```
P-09 커버리지 점검

프로젝트: @UnitConverter_07
현재 테스트 커버리지를 점검해줘.

- 실행: gcov UnitConverter.cpp
- 커버되지 않은 줄 중 Invariant 관련 분기 우선 식별
  (특히: meter↔feet/yard 비율 변환 분기, 음수 입력 처리, 없는 단위 처리)
- Domain Logic 95% 미달 시: 누락 테스트 케이스 제안 (구현 코드 추가 금지)
- Boundary 85% 미달 시: 누락 계약 테스트 케이스 제안

출력: │ 파일 │ Stmts │ Miss │ Cover │ 미달 원인 │ 추가 테스트 제안 │
```

### P-10 최종 작업 보고서 생성

```
P-10 최종 보고서

지금까지 작업한 내용을 Report 폴더에 보고서로 내보내줘.
프로젝트: @UnitConverter_07

포함 항목:
1. 작업 개요 (브랜치·날짜·작업자)
2. 완료된 To-Do 항목 요약 (Phase 6 To-Do 기준)
3. RED 단계 결과 (작성한 테스트 목록 + 실패 확인 여부)
4. GREEN 단계 결과 (통과한 테스트 + 커밋 메시지)
5. Refactoring 결과 (선택 항목·변경 파일·회귀 테스트 통과 여부)
6. 커버리지 현황 (레이어별 수치)
7. 미완료 항목 및 다음 단계 제안
8. 발견된 이슈 및 해결 방법
9. 생성형 AI 활용 회고 (도움이 된 순간 / 한계 / TC 작성 팁)

파일명 규칙: Report/YYMMDDn_UnitConverter_C++_[단계]_Report.md
```

---

## 보너스 — 신규 기능 추가 통합 구현 (C++)

> feature/new_feature 브랜치 · Item/기존 구조 수정 금지 · OCP 준수

### 보너스 목적 및 배경 (C++)

> **핵심 시험 과제:** 기존 변환 로직(구조체/클래스/함수) 수정 없이 신규 단위 규칙 추가
>
> **실용 예제:**
> - feature/new_feature 과제 = 동적 단위 등록 기능 추가
> - 규칙: "1 cubit = 0.4572 meter"를 `registerUnit("cubit", 0.4572)` 호출만으로 등록 가능
> - 등록된 단위는 다른 모든 등록 단위와 양방향 변환 가능
> - 기존 meter/feet/yard 변환 결과가 변하지 않아야 함 (Golden Master 통과)

> **⚠️ 제약 조건:**
> - 제약 1: Item 구조체(기존 변환 로직) 수정 금지
> - 제약 2: quality(변환 비율) 최소 0 유지 — 음수 비율 등록 금지
> - 제약 3: 기존 TC-A-01~07, TC-B-01~07 회귀 테스트 모두 PASS 유지
> - 제약 4: Golden Master 출력 불변 (meter/feet/yard 기존 결과 변경 금지)

### 과제 BONUS-A — 브랜치 준비 (C++)

```bash
git checkout A_01
git pull origin A_01
git checkout -b feature/new_feature
git push --set-upstream origin feature/new_feature

# 시작 전 기준선 확인
cmake -S . -B build && cmake --build build  →  모두 PASS
# gcov / lcov  →  Domain 95%+ / Boundary 85%+
```

### 과제 BONUS-B — RED: 신규 기능 테스트 먼저 작성 (C++)

```
BONUS RED — C++

프로젝트: @UnitConverter_07
신규 기능 RED 테스트를 작성하라. (구현 금지)

기능 설명:
  registerUnit("cubit", 0.4572)  // 1 cubit = 0.4572 meter
  convert("cubit", 1.0, "meter") == 0.4572 (오차 1e-5)
  convert("meter", 1.0, "cubit") == 1.0/0.4572 ≈ 2.1872 (오차 1e-5)
  convert("cubit", 1.0, "feet")  == 0.4572 * 3.28084 ≈ 1.4997 (오차 1e-5)

제약:
  - Item 구조체(기존 변환 로직) 수정 금지
  - 음수 비율 등록 시 예외 발생 (비율 > 0 강제)

테스트 케이스 (최소 5개):
  BT-01: registerUnit("cubit", 0.4572) 후 cubit→meter 변환
  BT-02: registerUnit 후 meter→cubit 역변환
  BT-03: registerUnit 후 cubit→feet 교차 변환
  BT-04: 음수 비율 등록 → std::invalid_argument 발생
  BT-05: registerUnit 후 convertAll("cubit", 1.0) → 전 단위 반환
  BT-06: 기존 meter→feet 결과 불변 (회귀 보호)

테스트 마킹: TEST_CASE("감쇠 단위 변환", "[bonus]") { REQUIRE(...); }
산출물: src/DecayingUnit.cpp + tests/test_decaying_unit.cpp (실패 상태)
```

### 과제 BONUS-C — GREEN: OCP 준수 최소 구현 (C++)

```
BONUS GREEN — C++

[단계] TDD GREEN만 수행. REFACTOR는 하지 않는다.
프로젝트: @UnitConverter_07

구현 접근법: OCP: 기존 if-else 수정 없이 UnitRegistry에 새 항목만 등록

할 일:
1. cmake --build build && ctest --test-dir build 실행 → BT-01~06 FAIL 확인
2. registerUnit() 메서드/함수 최소 구현:
   - 내부 UnitRegistry(Map/Dict)에 단위명·비율 저장
   - convert()는 Registry를 조회하여 meter 허브 경유 변환
   - Item 구조체(기존 변환 로직) 수정 금지
3. cmake --build build && ctest --test-dir build 재실행 → BT-01~06 PASS 확인
4. 기존 회귀 테스트 전체 PASS 확인

산출물:
- BT-01~06이 통과하는 최소 구현 코드
- 기존 TC-A/TC-B 전체 PASS 유지 확인
- 커밋 메시지: feat(feature): add registerUnit with OCP registry
```

### 과제 BONUS-D — 커버리지 & Golden Master 확인 후 PR (C++)

```
BONUS 최종 확인 — C++

신규 기능 구현 완료 후 전체 테스트, 커버리지, Golden Master를 확인하라.

1) 전체 테스트:
   cmake -S . -B build && cmake --build build
   ctest --test-dir build -V

2) 커버리지:
   gcov UnitConverter.cpp

3) Golden Master (출력 불변 확인):
   ctest -R bonus

확인 항목:
- BT-01~06 PASS (신규 기능)
- TC-A-01~07, TC-B-01~07 PASS (기존 회귀)
- Golden Master PASS (meter/feet/yard 출력 불변)
- registerUnit("cubit", -1.0) → std::invalid_argument 발생 확인
- Domain 커버리지 ≥ 95% / Boundary ≥ 85%

이후: feature/new_feature → A_01 PR 생성 → 리뷰 → 머지
최종: A_01 → main PR → 릴리스 태그 v1.0.0
```

---

# PART 6 — Git 브랜치 전략

---

## 브랜치 전략 개요

본 전략은 main에서 통합 브랜치(A_01)를 생성하고, 모든 작업 브랜치를 A_01에서 분기하여 A_01로 머지하는 단계별 흐름입니다.

**브랜치 트리 구조:**

```
main
└── A_01                     ← 통합 브랜치 (main에서 생성)
      ├── spec                 → PR → A_01 머지 후 삭제
      ├── red                  → PR → A_01 머지 후 삭제
      ├── green                → PR → A_01 머지 후 삭제
      ├── refactoring          → PR → A_01 머지 후 삭제
      └── feature/new_feature  → PR → A_01 머지 후 삭제
최종: A_01 → main  (모든 단계 완료 후 릴리스 PR)
```

## 브랜치 역할 및 머지 방향

| **브랜치** | **부모** | **역할** | **머지 방향** |
|---|---|---|---|
| main | - | 릴리스 브랜치. 검증 완료 코드만 존재. | ← A_01 (최종 릴리스 시) |
| A_01 | main | 통합 브랜치. 모든 작업 브랜치의 기준점. | ← spec / red / green / refactoring / feature/* |
| spec | A_01 | 명세(Spec) 작성 — PRD · Gherkin · 계약 문서 | → PR → A_01 |
| red | A_01 | RED 단계 — 실패 테스트 스켈레톤 작성 | → PR → A_01 |
| green | A_01 | GREEN 단계 — 최소 구현으로 테스트 통과 | → PR → A_01 |
| refactoring | A_01 | Refactoring 단계 — 외부 계약 불변 / 구조 개선 | → PR → A_01 |
| feature/new_feature | A_01 | 신규 기능 추가 (동적 단위 등록 등) | → PR → A_01 |

## 단계별 브랜치 생성 명령어

### ① A_01 브랜치 생성 (최초 1회)

```bash
git checkout main
git checkout -b A_01
git push --set-upstream origin A_01
```

### ② spec 브랜치 — 명세 작성

```bash
git checkout A_01
git checkout -b spec
git push --set-upstream origin spec

# 작업: PRD / Gherkin / 계약 문서 / .cursorrules 작성
# 완료 후 A_01로 PR
git add . && git commit -m "spec: add PRD and Gherkin scenarios"
git push origin spec
# GitHub: spec → A_01 방향으로 PR 생성 → 리뷰 → 머지
```

### ③ red 브랜치 — 실패 테스트 작성

```bash
git checkout A_01
git pull origin A_01
git checkout -b red
git push --set-upstream origin red

# 작업: Dual-Track RED 테스트 스켈레톤 작성 (구현 코드 금지)
git add . && git commit -m "test(red): add failing test skeletons for all ACs"
git push origin red
# GitHub: red → A_01 방향으로 PR 생성 → 리뷰 → 머지
```

### ④ green 브랜치 — 최소 구현

```bash
git checkout A_01
git pull origin A_01
git checkout -b green
git push --set-upstream origin green

# 작업: RED 테스트 1묶음씩 통과하는 최소 구현 (커밋 단위)
# 커밋 예시:
git commit -m "feat(green): implement convert meter to feet"
# 완료 후 A_01로 PR
git push origin green
```

### ⑤ refactoring 브랜치 — 구조 개선

```bash
git checkout A_01
git pull origin A_01
git checkout -b refactoring
git push --set-upstream origin refactoring

# 작업: 외부 계약 불변 / GREEN 유지 / 구조만 개선 (커밋 1개 단위)
# 커밋 예시:
git commit -m "refactor(domain): extract UnitRegistry from main()"
git push origin refactoring
```

### ⑥ feature/new_feature 브랜치 — 신규 기능

```bash
git checkout A_01
git pull origin A_01
git checkout -b feature/new_feature
git push --set-upstream origin feature/new_feature

# 커밋 예시:
git commit -m "feat: add dynamic unit registration"
git push origin feature/new_feature
```

### ⑦ 최종 릴리스 — A_01 → main

```bash
git checkout main
git pull origin main
# GitHub: A_01 → main 방향으로 릴리스 PR 생성 → 리뷰 → 머지

# 릴리스 태그 부여
git tag -a v1.0.0 -m "Release v1.0.0: UnitConverter with OCP/SRP"
git push origin v1.0.0
```

## PR 방향 요약

| **순서** | **작업 브랜치** | **→ PR 대상** | **커밋 메시지 컨벤션** |
|---|---|---|---|
| ① | spec → A_01 | 리뷰 후 머지 | `spec:` / `docs:` |
| ② | red → A_01 | 리뷰 후 머지 | `test(red):` |
| ③ | green → A_01 | 리뷰 후 머지 | `feat(green):` |
| ④ | refactoring → A_01 | 리뷰 후 머지 | `refactor(domain):` / `refactor(boundary):` |
| ⑤ | feature/* → A_01 | 리뷰 후 머지 | `feat:` |
| ⑥ | A_01 → main | 최종 릴리스 | `chore: release v1.0.0` |

## 브랜치 전략 수립 프롬프트

```
브랜치 전략 수립 (Cursor AI)

@워크스페이스 프로젝트를 구현하기 위한 깃 브랜치 전략을 알려줘.
개발방법론은 Dual-Track TDD 이고, 다음의 순서로 진행할 거야.

브랜치 구조:
- main 에서 A_01 을 만든다.
- A_01 에서 spec 을 만들어 작업 후 A_01 로 머지한다.
- A_01 에서 red 를 만들어 작업 후 A_01 로 머지한다.
- A_01 에서 green 을 만들어 작업 후 A_01 로 머지한다.
- A_01 에서 refactoring 을 만들어 작업 후 A_01 로 머지한다.
- A_01 에서 feature/new_feature 를 만들어 작업 후 A_01 로 머지한다.
- 모든 작업 완료 후 A_01 을 main 으로 머지한다.

각 브랜치에 대해:
1. 생성 명령어
2. 작업 범위 및 커밋 메시지 컨벤션
3. PR 방향 (작업브랜치 → A_01) 확인 방법
4. 머지 후 브랜치 삭제 여부
```

## GitHub PR 및 리뷰 절차

> **PR 작업 순서 (매 브랜치 공통)**
> 1. 작업 브랜치에서 커밋 완료 후 `git push origin <브랜치명>`
> 2. GitHub에서 **Compare & pull request** 클릭
> 3. PR 방향 확인: `<작업브랜치> → A_01`  (반드시 A_01이 base여야 함)
> 4. 리뷰어 추가 (Settings > Collaborators > Add people)
> 5. 코드 리뷰 완료 후 **Merge pull request**
> 6. 머지 완료 후 작업 브랜치 삭제 (Delete branch 버튼)
>
> ⚠️ PR 방향이 항상 "작업브랜치 → A_01" 인지 확인하십시오.
> ⚠️ main 으로 직접 PR 하지 마십시오. 반드시 A_01 경유.
> ⚠️ 리뷰 없는 머지는 금지입니다.

## 전체 브랜치 흐름 요약

| **단계** | **브랜치** | **주요 작업** | **완료 기준** |
|---|---|---|---|
| 0. 통합 브랜치 생성 | main → A_01 | A_01 생성 및 원격 Push | A_01 브랜치 원격 확인 |
| 1. 명세 작성 | A_01 → spec → A_01 | PRD / Gherkin / .cursorrules 작성 | PR 리뷰 통과 후 A_01 머지 |
| 2. RED 단계 | A_01 → red → A_01 | 실패 테스트 스켈레톤 작성 | 모든 테스트 FAIL 확인 후 머지 |
| 3. GREEN 단계 | A_01 → green → A_01 | 최소 구현으로 테스트 통과 | 모든 RED 테스트 PASS 후 머지 |
| 4. Refactoring | A_01 → refactoring → A_01 | 외부 계약 불변 / 구조 개선 | 회귀 테스트 PASS 후 머지 |
| 5. 신규 기능 | A_01 → feature/* → A_01 | 추가 기능 구현 | 기능 테스트 PASS 후 머지 |
| 6. 릴리스 | A_01 → main | 최종 릴리스 PR | 릴리스 태그 부여 |

---

## 전체 실습 흐름 한눈에 보기

| **파트** | **단계** | **산출물** | **핵심 금지 사항** |
|---|---|---|---|
| PART 2 | Phase 1 — 문제 정의 | Mom Test / Invariant | 코드·설계 작성 |
| PART 2 | Phase 2 — BCE 설계 | 설계 패키지 / RED 목록 | 구현 코드 |
| PART 2 | Phase 3 — .cursorrules | .cursorrules YAML | 코드 작성 |
| PART 2 | Phase 4 — Epic/Gherkin | Story / Gherkin / 체크리스트 | 구현 코드 |
| PART 2 | Phase 5 — PRD | PRD 문서 (§1~8) | 코드·설계 |
| PART 2 | Phase 6 — README/To-Do | README.md / TODO.md | 구현 코드 추가 |
| PART 3 | P-04 RED (브랜치: red) | 실패 테스트 스켈레톤 | 구현/GREEN/REFACTOR |
| PART 3 | P-05/06 GREEN (브랜치: green) | 최소 구현 / 테스트 통과 | REFACTOR / 하드코딩 |
| PART 4 | P-07/08 Refactoring (브랜치: refactoring) | 구조 개선 / 회귀 테스트 통과 | 계약 변경 / 기능 추가 |
| PART 5 | P-09/10 QA | 커버리지 보고서 / 최종 보고서 | 커버리지 기준 미달 |
| PART 6 | Git 전략 (main→A_01→*→A_01→main) | 브랜치 구조 / PR 흐름 | 리뷰 없는 머지 / main 직접 PR |
