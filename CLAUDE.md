# UnitConverter_07 — Claude 작업 지침

## 프로젝트 개요

- **저장소:** https://github.com/nyx7zen/UnitConverter_07
- **개발 방법론:** Dual-Track TDD (RED → GREEN → Refactoring)
- **기술 스택:** C++17 · CMake · Catch2 · g++
- **참고 문서:** workbook.md (단계별 프롬프트 및 브랜치 전략 포함)
- **비즈니스 로직:** meter 허브 기준 환산 (1 meter = 3.28084 feet, 1 meter = 1.09361 yard)

---

## 브랜치 전략

```
main
└── B_01                      ← 통합 브랜치
      ├── spec                 → PR → B_01 머지 후 삭제
      ├── red                  → PR → B_01 머지 후 삭제
      ├── green                → PR → B_01 머지 후 삭제
      ├── refactoring          → PR → B_01 머지 후 삭제
      └── feature/new_feature  → PR → B_01 머지 후 삭제
최종: B_01 → main (릴리스 PR)
```

---

## 실행 정책 (필수 준수)

1. **절대로 workbook.md의 단계별 프롬프트를 먼저 또는 자동으로 실행하지 않는다.**
2. 사용자가 workbook.md에서 프롬프트를 복사해 대화창에 붙여넣고 실행 요청할 때만 실행한다.
3. **명령어(git, cmake, 파일 수정 등)를 실행하기 전에 반드시 "다음 단계를 진행해도 될까요?" 라고 먼저 물어본다. 자동으로 실행하지 않는다.**
4. 단계 완료 후, 또는 사용자가 "상태" / "현재 상태를 알려주세요" / "현재 상황과 진행할 항목을 알려주세요"라고 요청하면 아래 3가지를 순서대로 표시한다.

**① 브랜치 진행 현황**
```
main ──●
       └── B_01 ──●
                  ├── spec      ──● ✅ 머지 완료
                  ├── red       ──● 🔄 작업 중 / ✅ 완료
                  ├── green          ❌ 미생성
                  ├── refactoring    ❌ 미생성
                  └── feature/*      ❌ 미생성
```

상태 아이콘 기준:
- ✅ 머지 완료
- 🔄 작업 중 (브랜치 생성됨, 진행 중)
- ❌ 미생성

**② workbook.md 진행 현황** — workbook.md의 P-01~P-10 각 단계 진행 상태를 아래 형식으로 표시한다.
```
P-01 spec      ✅  P-02 RED-A      ✅  P-03 RED-B     ✅
P-04 GREEN-A   ✅  P-05 GREEN-B    🔄  P-06 GREEN-C   ❌
...
```

**③ git 상태 요약** — 현재 브랜치, 직전 커밋, 스테이징된 파일 목록(상태·파일명)

**④ 진행할 항목** — 다음에 해야 할 작업을 번호 순서로 표시

| 순서 | 작업 | 방법 |
|------|------|------|
| ① | 작업명 | Claude 실행 / GitHub 웹 직접 |
| ② | 작업명 | … |

---

## 아키텍처 — BCE 레이어 (from .cursorrules)

### 레이어 정의

| 레이어 | 역할 | 의존 방향 |
|--------|------|-----------|
| **entity** | Domain: UnitRegistry, LengthConverter, UnitRatio, 불변식 | 없음 (최하위) |
| **control** | Use case: ConvertController, RegisterController, StartupLoader | entity, data (interface 경유) |
| **boundary** | CLI 입출력, 파싱, 렌더(table/json/csv), 에러 표면화 | control |
| **data** | 설정·단위 정의 로드(JSON). DTO만 entity에 주입 | 없음 |

### 의존성 방향 (위반 금지)

```
boundary → control → entity
control  → data (interface)
entity   → boundary | control | data  ← 금지
data     → entity 구현 의존            ← 금지 (DTO 변환은 control)
```

### 확장 원칙

- **새 단위:** registry 등록만, converter 분기 추가 금지
- **새 출력:** boundary renderer 추가, entity 무변경

### 파일 구조

```
UnitConverter_07/
├── entity/                 # Domain: registry, converter, ratio, errors
│   ├── UnitRegistry.h / .cpp
│   └── LengthConverter.h / .cpp
├── control/                # Use cases
│   ├── ConvertController.h
│   └── RegisterController.cpp
├── boundary/               # CLI, parser, renderers, error map
│   ├── CliApp.cpp
│   ├── InputParser.cpp
│   └── TableRenderer.cpp
├── data/                   # JSON config reader
│   └── JsonUnitDefinitionReader.cpp
├── tests/
│   ├── entity/             # [entity] Catch2
│   ├── boundary/           # [boundary] mock entity
│   ├── data/
│   └── integration/        # end-to-end
├── config/
│   └── units.json
└── main.cpp                # wiring only, logic 금지
```

---

## TDD 규칙 (from .cursorrules)

### RED 단계

**규칙**
- 실패하는 테스트를 먼저 추가한다 (구현 코드 작성 금지)
- Domain 테스트는 I/O·파일·stdout 없이 순수 로직만 검증한다
- 허브 변환(meter/feet/yard), 음수 거절, 미지원 단위를 RED 우선 순서로 작성한다
- Boundary 테스트는 Domain을 mock/stub하여 계약(에러 문구·exit code·포맷)만 검증한다
- 비율 기대값은 README 상수(3.28084, 1.09361)와 epsilon 1e-9로 고정한다

**금지**
- 테스트 없이 프로덕션 코드 먼저 작성
- RED 단계에서 전체 통합·파일 로드·JSON 파서 한꺼번에 구현
- Domain에 cout, fstream, argv 파싱 삽입
- 실패 테스트를 @skip, DISABLED, 주석 처리로 숨기기

### GREEN 단계

**규칙**
- RED를 통과하는 가장 단순한 구현(하드코딩 포함)을 허용한다
- 순서: Domain GREEN → Boundary mock GREEN → Data → Integration
- 커밋 전 해당 레이어 태그 필터 테스트 전부 실행한다

**금지**
- GREEN을 위해 테스트 기대값·epsilon·메시지 문자열 변경
- 통과하지 않은 테스트 삭제·완화
- 여러 RED를 한 번에 풀기 위해 범위 밖 리팩토링·기능 추가
- feet↔yard 직접 상수로 우회 구현 (meter 허브만)
- Boundary 없이 Domain에서 stdout으로 결과 출력

### Refactoring 단계

**규칙**
- 전체 테스트 GREEN 상태에서만 구조 개선
- 리팩토링 전후 동일 테스트 스위트 실행, diff 없음 확인
- 중복 비율·분기를 UnitRegistry/LengthConverter로 추출

**금지**
- RED/GREEN 없이 대규모 구조 변경
- 리팩토링 중 새 요구(포맷·등록·설정) 동시 추가
- 공개 Domain API 시그니처를 테스트 갱신 없이 변경
- 커버리지 목표(Domain 95%, Boundary 90%, Data 85%) 미달 무시

---

## 테스트 기준 (from .cursorrules)

| 항목 | 내용 |
|------|------|
| 프레임워크 | Catch2 v3 |
| 패턴 | AAA (Arrange-Act-Assert = Given-When-Then) |
| epsilon | **1.0e-9** (비율 정확도) |
| 태그 | `[entity][ratio]`, `[boundary][parse]`, `[data][config]`, `[integration]` |
| Golden 출력 | 에러 메시지·table/json/csv 문자열 완전 일치 |

### 커버리지 목표

| 레이어 | 라인 | 브랜치 |
|--------|------|--------|
| entity | 95% | 90% |
| boundary | 90% | 85% |
| data | 85% | 80% |
| control | 80% | 75% |

### 테스트 픽스처

- entity 테스트: `UnitRegistryFixture(seedDefaults)`
- boundary 테스트: `MockLengthConverter` 고정 스냅샷
- integration: 임시 config JSON + stdin 문자열

---

## 코딩 규칙 (from .cursorrules)

### 네이밍

| 대상 | 규칙 |
|------|------|
| 타입·클래스 | PascalCase |
| 함수·변수 | camelCase |
| 상수 | `kPascalCase` 또는 `constexpr inline` |
| 파일명 | PascalCase (주요 타입과 일치) |
| 네임스페이스 | lower_snake 또는 프로젝트 접두어 |

### 포맷

- 줄 길이 100 권장, 들여쓰기 4 spaces
- 커밋 전 clang-format 적용
- 헤더에서 `using namespace std` 금지
- include 순서: 자기 헤더 → 표준 → 서드파티 → 프로젝트

### Forbidden 패턴

| 패턴 | 이유 | 대안 |
|------|------|------|
| `std::cout / std::cerr` 디버그 다수 삽입 | Boundary 계약·golden 오염 | `Catch2 CAPTURE`, 로거 인터페이스 |
| 비율 숫자 리터럴 scattered (3.28084 등) | 단일 진실 원천 위반 | `constexpr kFeetPerMeter`, config JSON |
| `catch(...) {}` without rethrow | 실패 원인 은닉 | `catch (const std::exception& e)` |
| 헤더 내 non-trivial 함수 정의 | ODR·컴파일 의존 증가 | `.h 선언 + .cpp 정의` |
| 전역 mutable registry / singleton | 테스트 격리 불가 | UnitRegistry 인스턴스를 fixture에 주입 |
| entity에서 `#include <iostream>` | BCE entity 순수성 위반 | boundary adapter가 stream 소유 |
| feet-yard 직접 곱셈 상수 환산 | meter 허브 불변식 위반 | metersPerUnit 비율로 hub 경유 |

---

## AI 행동 규칙 (from .cursorrules)

### 레이어 규칙

- entity에 I/O·파싱·포맷·파일 로드 **금지**
- boundary가 entity/data를 직접 mutate **금지** (control 경유)
- main.cpp는 조립(wiring)만

### 출력 규칙

- **한 턴에 RED 1~3개 또는 GREEN 1개 범위 우선**
- 설계 계약과 다른 에러 문구·출력 생성 금지
- 레이어 위반 코드를 편의상 한 파일에 몰아넣기 금지
- 테스트 삭제·완화로 GREEN 만들기 금지

---

## 파일 네이밍 규칙

### Prompting 폴더 — 프롬프트 및 실행 결과 기록

```
Prompting/{브랜치명}_{순번}_{내용}.md
```

예시: `spec_01_BCE_설계.md` · `red_01_샘플예제_선택.md` · `green_01_GREEN_첫번째AC.md`

### Report 폴더 — 보고서 기록

```
Report/{날짜}_{브랜치명}_{보고서종류}.md
```

예시: `260521_spec_PRD.md` · `260521_red_test_plan.md` · `260522_green_coverage.md`

---

## 사용자 정보

- Git/GitHub 작업이 처음인 초보자
- 교육 목적 실습 (학습 효과를 위해 Claude의 선제 실행 금지)
- 브랜치 작업 순서: 브랜치 생성 → 코드 작업 → PR 생성/리뷰 → 머지
