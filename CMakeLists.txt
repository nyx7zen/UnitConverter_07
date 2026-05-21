cmake_minimum_required(VERSION 3.16)
project(UnitConverter_07 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── 프로덕션 실행 파일 ──────────────────────────────────────
add_executable(UnitConverter UnitConverter.cpp)

# ── 테스트 옵션 ────────────────────────────────────────────
option(ENABLE_TESTING  "Enable Catch2 unit tests"   ON)
option(ENABLE_COVERAGE "Enable gcov/lcov coverage"  OFF)

if(ENABLE_TESTING)

    set(BUILD_SHARED_LIBS OFF)   # Catch2를 정적 라이브러리로 빌드 (DLL 의존성 제거)

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
    )
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
    )
    FetchContent_MakeAvailable(Catch2 nlohmann_json)

    enable_testing()
    include(CTest)

    add_executable(UnitConverter_test
        tests/test_domain.cpp
        tests/test_boundary.cpp
        tests/test_data.cpp
        entity/UnitRegistry.cpp
        entity/LengthConverter.cpp
        boundary/ConversionService.cpp
        boundary/InputParser.cpp
        data/JsonUnitDefinitionReader.cpp
    )

    target_compile_features(UnitConverter_test PRIVATE cxx_std_17)
    target_include_directories(UnitConverter_test PRIVATE ${CMAKE_SOURCE_DIR})
    target_link_libraries(UnitConverter_test PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)

    # catch_discover_tests: CTest에 개별 TC 자동 등록
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    # DISCOVERY_MODE PRE_TEST: 빌드 타임이 아닌 ctest 실행 시 TC 발견
    # Windows(MinGW) 환경에서 ctest 실행 시 PATH에 MinGW bin 디렉터리 추가 필요:
    #   $env:PATH = "C:\mingw64\bin;$env:PATH"; ctest --test-dir build -V
    catch_discover_tests(UnitConverter_test
        DISCOVERY_MODE PRE_TEST
        DL_PATHS "C:/mingw64/bin"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

endif()

# ── 커버리지 (gcov/lcov) ───────────────────────────────────
if(ENABLE_COVERAGE AND ENABLE_TESTING)
    target_compile_options(UnitConverter_test PRIVATE --coverage -O0 -g)
    target_link_options(UnitConverter_test   PRIVATE --coverage)
endif()
