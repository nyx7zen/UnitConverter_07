# 결함 목록 — UnitConverter_07 (C++)

| 항목 | 내용 |
|------|------|
| 문서 버전 | 1.0 |
| 작성일 | 2026-05-21 |
| 브랜치 | `red` |
| 빌드 상태 | ✅ Green (cmake --build build 통과) |
| 테스트 상태 | 37/37 FAIL("RED") — 의도된 RED 단계 상태 |

---

## 현재 결함 요약

> RED 단계 기준: `FAIL("RED")` 는 구현 전 의도적 실패 마커.
> 실제 결함(기대값 vs 실제값 불일치)은 GREEN 단계 구현 이후 기록합니다.

**현재 실제 결함 수: 0건**

---

## GREEN 단계 진입 예상 결함 (사전 분석)

| DEF-ID | Severity | TC-ID | 변환 타입 | 재현 절차 | 기대값 | 실제값 | 근본 원인 | 수정 요약 |
|--------|----------|-------|-----------|-----------|--------|--------|-----------|-----------|
| DEF-001 | Critical | TC-B-01 | meter→feet | `FAIL("RED")` → `convert()` 호출로 교체 후 빌드 | `8.202100` | 링크 오류 | `convert()` 함수 미정의 | `convert()` 함수 추가 (if-else 체인) |
| DEF-002 | Critical | TC-A-02 | 입력 파싱 | `FAIL("RED")` → `parseAndConvert()` 호출 교체 | `invalid_argument` | `return 1` (예외 없음) | main()이 예외 대신 종료코드 사용 | `parseAndConvert()` 분리 + 예외 throw |
| DEF-003 | Critical | TC-A-03 | 음수 검증 | `"meter:-1.0"` 입력 | `invalid_argument` 발생 | 음수 변환 결과 출력 | 음수 입력 검증 로직 없음 | `value < 0` 시 예외 throw |
| DEF-004 | Critical | TC-A-04 | 없는 단위 | `"parsec:1.0"` 입력 | `invalid_argument("unknown unit: parsec")` | `return 1` | unknown unit 예외 미발생 | if-else else에 예외 추가 |
| DEF-005 | Major | TC-B-04 | convertAll | `convertAll("meter", 1.0)` 호출 | map 반환 | 링크 오류 | `convertAll()` 함수 미정의 | `convertAll()` 함수 추가 |
| DEF-006 | Major | TC-B-05 | registerUnit | `registerUnit("cubit", 0.4572)` 호출 | 등록 후 변환 성공 | 링크 오류 | `registerUnit()` 함수 미정의 | 동적 등록 Map 구조 추가 |
| DEF-007 | Minor | TC-A-06 | 출력 포맷 | `"meter:2.5"` 입력 후 출력 확인 | `"2.5 meter = 8.202100 feet"` | `"2.5 meter = 8.2021 feet"` | `std::cout` 소수점 6자리 미지정 | `std::setprecision(6) << std::fixed` 추가 |
| DEF-008 | Major | TC-B-06 | loadConfig | `loadConfig("valid.json")` 호출 | 비율 정상 로드 | 링크 오류 | `loadConfig()` 함수 미정의 | JSON 파싱 구현 (nlohmann/json 등) |

---

## 수정 완료 항목

| DEF-ID | 수정일 | 수정 브랜치 | 검증 TC | 결과 |
|--------|--------|-------------|---------|------|
| — | — | — | — | — |

---

## GREEN 단계 수정 가이드

### 최소 수정 순서 (TC 우선순위 P1 → P2 → P3)

```
1. convert() 함수 추출       → TC-B-01, TC-B-02, TC-B-03 통과
2. parseAndConvert() 분리    → TC-A-02, TC-A-03, TC-A-04, TC-A-05 통과
3. 출력 포맷 소수점 6자리   → TC-A-06 통과
4. 경계값 처리 확인         → TC-A-07 (value=0) 통과
5. convertAll() 추가         → TC-B-04 통과
6. registerUnit() 추가       → TC-B-05 통과
7. loadConfig() 추가         → TC-B-06, TC-B-07 통과
```

### 금지 사항 (GREEN 단계)

- 테스트 약화·삭제 금지
- `FAIL("RED")` → 하드코딩 숫자로 통과 금지 (`return 8.20210;`)
- 비율 `3.28084` / `1.09361` 인라인 하드코딩 금지 → 상수 사용
- 한 커밋에 모든 RED 동시 해결 금지

---

## 회귀 보호 체크리스트

- [ ] convert() 추가 후 TC-B-01~03 PASS, 나머지 TC 영향 없음 확인
- [ ] parseAndConvert() 추가 후 TC-A-02~05 PASS, 기존 PASS TC 영향 없음 확인
- [ ] 전체 TC PASS 후 Golden Master 기준 출력 생성
