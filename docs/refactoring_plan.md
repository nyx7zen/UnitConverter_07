# Refactoring Plan — UnitConverter_07

**브랜치:** refactoring  
**날짜:** 2026-05-21  
**작성 근거:** Step 1~3 Ask-mode 분석 결과

---

## 1. 리팩토링 목표

| 항목 | 현재 상태 | 목표 상태 |
|------|-----------|-----------|
| main() 크기 | 47줄, 단일 함수에 모든 로직 | 10줄 이내, BCE 조립(wiring)만 |
| 매직 넘버 | 3.28084, 1.09361 하드코딩 | UnitRegistry.seedDefaults() 경유 |
| 예외 처리 | `catch(...)` — 원인 은닉 | `catch(const std::exception& e)` |
| SRP 준수 | I/O·변환·분기 혼재 | boundary/entity/data 역할 분리 |
| 미사용 include | `#include <sstream>` 불필요 | 제거 |

---

## 2. 대상 파일

| 파일 | 작업 |
|------|------|
| `UnitConverter.cpp` | main() 전면 재작성 — BCE wiring으로 교체 |

BCE 레이어(entity, boundary, data)는 이미 완성 — 변경 없음.

---

## 3. 변경 전후 비교

### 현재 main() 구조 (문제점)

```cpp
// main() — 47줄, 복합 책임
// - 직접 단위 분기: if (unit == "feet") ... else if (unit == "yard")
// - 매직 넘버: 3.28084, 1.09361
// - catch(...) {} — 원인 은닉
// - sstream include 미사용
```

### 목표 main() 구조 (BCE wiring)

```cpp
#include "entity/UnitRegistry.h"
#include "entity/LengthConverter.h"
#include "boundary/ConversionService.h"
#include "data/JsonUnitDefinitionReader.h"
#include <iostream>

int main(int argc, char* argv[]) {
    entity::UnitRegistry reg;
    reg.seedDefaults();
    data::loadConfig("config/units.json", reg);

    entity::LengthConverter conv(reg);
    boundary::ConversionService svc(conv);

    std::string input;
    while (std::getline(std::cin, input)) {
        try {
            std::cout << svc.parseAndConvert(input) << '\n';
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}
```

---

## 4. 커밋 계획 (3단계)

| 순서 | 커밋 메시지 | 변경 내용 |
|------|------------|-----------|
| ① | `refactor: wire main() to BCE layers` | main() 전면 재작성, BCE 조립 |
| ② | `refactor: improve exception handling in main` | `catch(...)` → `catch(const std::exception& e)` |
| ③ | `refactor: remove unused sstream include` | `#include <sstream>` 제거 |

> **주의:** 각 커밋 전후 `ctest --test-dir build` 실행 — 41개 테스트 전부 통과 확인 필수

---

## 5. 회귀 방지 체크리스트

- [ ] `[entity][ratio]` 18개 테스트 통과
- [ ] `[boundary][parse]` 15개 테스트 통과
- [ ] `[data][config]` 8개 테스트 통과
- [ ] 총 41개 GREEN 유지
- [ ] main() 로직 외 파일 변경 없음

---

## 6. 검증 명령어

```powershell
# 빌드
cmake --build build --config Debug

# 전체 테스트
ctest --test-dir build --output-on-failure

# 레이어별 필터
ctest --test-dir build -L entity
ctest --test-dir build -L boundary
ctest --test-dir build -L data
```

---

## 7. 완료 기준

- main() ≤ 15줄
- 매직 넘버 0개
- `catch(...)` 0개
- 41/41 테스트 GREEN
- PR: refactoring → B_01 머지
