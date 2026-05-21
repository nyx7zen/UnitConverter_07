# To-Do 리스트 — UnitConverter (C++)

**기준:** [PRD.md](./PRD.md) §3 기능 요구 · §7 인수·회귀  
**범위:** 저장소 내 CLI 길이 변환 학습 시스템 (NG-01~03 제외)

---

## 🔴 필수 (Must-Have) — v1.0 릴리스 차단 항목

| # | 작업 | 연관 PRD | 완료 기준 (누가 · 무엇 · 통과) |
|---|------|----------|--------------------------------|
| M-01 | [ ] **entity** `UnitRegistry` 시드(meter/feet/yard) + 중복·빈 이름·hub 불변 | FR-03, §5.1, G-01 | **학습자**가 `[entity]` 테스트 실행 → hub 1.0 / feet 3.28084 / yard 1.09361 (ε≤1e-9) **100% PASS** |
| M-02 | [ ] **entity** `LengthConverter` 허브 환산 (단위명 `if` 분기 0건) | FR-01, FR-04, G-05 | **리뷰어** 정적 확인 분기 0건 + **학습자** D-TC 허브·역변환 **PASS** |
| M-03 | [ ] **boundary** convert 파싱 `unit:value` + 4종 stderr exact | FR-02, §3.2, AC-02 | **학습자** golden 테스트 → `meter2.5`/`meter:abc`/`meter:-1`/`mile:1` stderr **완전 일치**, stdout `* = *` **0줄** |
| M-04 | [ ] **control** `ConvertController` — boundary↔entity 연결만 | FR-01, §4.2, G-06 | **학습자** integration 1건 → `meter:2.5` exit 0; control line ≥80% 또는 통합으로 대체 검증 기록 |
| M-05 | [ ] **boundary** table 출력 + PRES 좌변 보존 | FR-05, §6.1, AC-01 | **학습자** 실행 → 3줄 exact: `2.5 meter = 2.5 meter` / `8.2 feet` / `2.7 yard` |
| M-06 | [ ] **CMake** 빌드·`UnitConverter` 실행 타깃 | §4.1 | **학습자** `cmake --build` + 바이너리 실행 exit 0 (README Quick Start) |
| M-07 | [ ] **Catch2** entity 필수 RED→GREEN (허브·음수·unknown) | §3.1, AC-02, REG-01 | **학습자** `[entity]` 전부 PASS; REG-01 기준선(D-TC) **0 failure** |
| M-08 | [ ] **NEG-01** 음수 거절·0 허용 문서·테스트 일치 | §3.3 NEG-01, AC-08 | **학습자** `meter:-1` 거절 + `meter:0` 전 target `0.0` **PASS** |
| M-09 | [ ] **PRD §7.1** AC-01·02·07·08 체크 | AC-01,02,07,08 | **강사** 인수 표 AC-01,02,07,08 **체크 완료** 서명 |

---

## 🟡 권장 (Should-Have) — 품질 향상 항목

| # | 작업 | 연관 PRD | 완료 기준 (누가 · 무엇 · 통과) |
|---|------|----------|--------------------------------|
| S-01 | [ ] **boundary** JSON renderer + `--format=json` | FR-06, FR-10, AC-09, Gherkin #7 | **학습자** `yard:1.09361` JSON → source·conversions 3항 수치 **1.0 / 3.3 / 1.1**; `[boundary][json]` golden ≥3 **PASS** |
| S-02 | [ ] **boundary** CSV renderer + golden | FR-07, FMT-01, AC-09 | **학습자** 헤더 exact + 행 수=단위 수; table과 수치 **동일**; `[boundary][csv]` ≥3 **PASS** |
| S-03 | [ ] **data** `config/units.json` 로드 + startup | FR-08, §5.2, AC-04 | **학습자** 유효 JSON 후 변환 비율 README 일치; `[data]` line ≥85% |
| S-04 | [ ] **data** 설정 실패 exit 2 + stderr exact | FR-08, AC-04 | **학습자** 파일 없음 → `Failed to load config: {path}`; ratio≤0·중복 → §3.2 stderr **exact** |
| S-05 | [ ] **control** `RegisterController` + 등록 파싱 | FR-09, §5.3, AC-05 | **학습자** `1 cubit = 0.4572 meter` 후 `cubit:1` → meter 0.4572 (ε≤1e-9) **PASS** |
| S-06 | [ ] **boundary** 등록 실패 3종 stderr | FR-09 | **학습자** 형식/중복/ratio stderr **완전 일치** 테스트 **PASS** |
| S-07 | [ ] **커버리지** entity/boundary/data 임계치 | §4.3, AC-06, G-01~03 | **학습자** 리포트 제출 → entity ≥95/90, boundary ≥90/85, data ≥85/80 |
| S-08 | [ ] **PRES** feet 입력 table 3줄 exact | AC-03, Gherkin #2 | **학습자** `feet:3.28084` 3줄 exact **PASS** |
| S-09 | [ ] **PRD §7.1** AC-03~06·09 체크 | AC-03~06,09 | **강사** AC-03~06,09 **체크 완료** |

---

## 🟢 선택 (Nice-to-Have) — v2.0 후보

| # | 작업 | 기대 가치 (한 줄) |
|---|------|------------------|
| N-01 | [ ] YAML 설정 reader (FR-11) | JSON 없는 환경에서 동일 스키마 로드 연습 |
| N-02 | [ ] `--format` 잘못된 값 stderr (FR-10) | CLI 옵션 계약 완결성 |
| N-03 | [ ] Gherkin #8 yard hub I-TC 자동화 | feet↔yard 직접 상수 우회 **회귀 자동 차단** |
| N-04 | [ ] config/register 전용 I-TC (PRD §8 미포함 4건) | FR-07·08·09·10 E2E 문서화 |
| N-05 | [ ] clang-format CI hook | 스타일 드리프트 방지 |
| N-06 | [ ] lcov 리포트 README 링크 | 커버리지 제출 표준화 |

---

## 🔵 기술 부채 (Tech Debt)

| # | 문제 | 발생 원인 | 해결 방향 |
|---|------|-----------|-----------|
| TD-01 | [ ] 단일 파일 `UnitConverter.cpp`에 파싱·비율·출력 결합 | 실습 초기 스켈레톤 | entity/control/boundary/data 분리 (PRD §4.2 트리) |
| TD-02 | [ ] 비율 literal 산재 (3.28084, 1.09361) | 하드코딩 습관 | `config/units.json` + `UnitRatio` 단일 출처 |
| TD-03 | [ ] 테스트·CMake 부재 | TDD 전 단계 | Catch2 + ctest; README Quick Start 경로 통일 |
| TD-04 | [ ] stderr/exit 계약 미고정 | README 구버전 | PRD §3.2 golden 전부 Catch2화 |
| TD-05 | [ ] feet↔yard 직접 환산 유혹 | README 예시 편향 | hub-only 테스트 REG + Gherkin #8 |
| TD-06 | [ ] 전역 mutable registry 가능성 | 레거시 패턴 | fixture/control **주입** Registry |

---

## ✅ 완료 항목 (Done)

| # | 완료 내용 | 완료일 | 관련 |
|---|-----------|--------|------|
| D-01 | [x] Phase 5 PRD 작성 (`docs/PRD.md`) | 2026-05-20 | docs: PRD v1.0 |
| D-02 | [x] Phase 6 README 이관 (계약·NEG-01·PRES·출력 예시) | 2026-05-20 | docs: README Phase 6 |
| D-03 | [x] `.cursorrules` (TDD·BCE·forbidden·file_structure) | 2026-05-20 | chore: cursor rules |
| D-04 | [x] 레거시 스켈레톤 동작 참고용 (`UnitConverter.cpp`) | (기존) | baseline only, v1.0 대체 예정 |

---

## 📋 회귀 방지 체크리스트 (PRD §7.2)

**배포·v1.0 태그·과제 제출 전 — 학습자 실행 + 강사 확인**

| # | 확인 항목 | 담당 | 통과 조건 |
|---|-----------|------|-----------|
| R-01 | [ ] **REG-01** 기준선 | 학습자 | D-TC(허브 1m↔feet/yard, `meter:2.5` Domain) **0 failure** |
| R-02 | [ ] **REG-02** golden 동기화 | 리뷰어 | stderr/stdout·에러 문구 변경 시 PRD+테스트+README **동시 diff** |
| R-03 | [ ] **REG-03** 에러 매핑 | 학습자 | DomainError↔exit/message 변경 시 `[boundary]` **전부 PASS** |
| R-04 | [ ] **REG-04** config schema | 학습자 | breaking 시 DT-TC·PRD §5.2 **버전 기록** |
| R-05 | [ ] **REG-05** 전체 스위트 | 학습자 | `ctest` **0 failure** (단위·포맷 추가 후) |
| R-06 | [ ] **계약 테스트** | 학습자 | AC-01~09 해당 항목 **100% PASS** |
| R-07 | [ ] **커버리지** | 학습자 | G-01~03 수치 **미달 없음** (리포트 첨부) |
| R-08 | [ ] **README** | 리뷰어 | Quick Start·입력·출력·설정이 PRD §3.2·§6와 **불일치 0건** |
| R-09 | [ ] **G-05** OCP | 리뷰어 | `LengthConverter` 단위명 분기 **0건** |

---

## 🗓️ 마일스톤

| 마일스톤 | 포함 항목 (PRD) | 목표일 | 상태 | 완료 정의 (누가 서명) |
|----------|-----------------|--------|------|------------------------|
| **M0 문서·계약** | PRD §1~3 계약, README, `.cursorrules` | 2026-05-20 | ✅ Done | 강사: PRD v1.0 존재 |
| **M1 Domain v1.0** | FR-03, FR-04, FR-02(entity부), G-01, REG-01 | +2일 | 🔲 Planned | 학습자: M-01~03,07,08 PASS |
| **M2 Boundary v1.0** | FR-01, FR-02, FR-05, AC-01,02, G-02 | +1일 | 🔲 Planned | 학습자: M-04~05,09 PASS |
| **M3 확장 v1.1** | FR-06~09, FR-08, AC-03~06,09, G-03 | +2일 | 🔲 Planned | 학습자: S-01~09 PASS |
| **M4 릴리스 v1.0** | §7.1 AC 전부, §7.2 REG, G-04~06 | +0.5일 | 🔲 Planned | 강사: 회귀 R-01~09 체크 · v1.0 tag |
| **M5 v2.0 후보** | FR-10, FR-11, N-01~06 | TBD | 🔲 Backlog | PO: Nice-to-have 2건 이상 선택 |

---

## 추적 요약

| PRD FR | Must | Should | Nice |
|--------|------|--------|------|
| FR-01 | M-04,05 | S-01,02 | — |
| FR-02 | M-03,08 | S-06 | — |
| FR-03 | M-01 | S-03 | — |
| FR-04 | M-02 | — | N-03 |
| FR-05 | M-05 | S-08 | — |
| FR-06 | — | S-01 | — |
| FR-07 | — | S-02 | N-04 |
| FR-08 | — | S-03,04 | — |
| FR-09 | — | S-05,06 | N-04 |
| FR-10 | — | (S-01 일부) | N-02 |
| FR-11 | — | — | N-01 |

---

*코드 구현은 본 문서 범위 외. 항목 완료 시 `[ ]` → `[x]` 및 Done 테이블로 이동.*
