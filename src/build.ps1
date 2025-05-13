param (
    [string]$SourceDir = "..",                     # Source directory
    [string]$BuildDir = "build",                   # Build directory
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",                     # Build configuration
    [switch]$EnableTests = $false,                 # Enable unit tests
    [switch]$RunTests = $false,                    # Run tests (ctest)
    [switch]$CleanBuild = $false,                  # Clean build directory before building
    [switch]$EnableBenchmarks = $false,            # Enable benchmarks
    [switch]$EnableMutationTests = $false,         # Enable mutation tests
    [switch]$RunMutatorTarget = $false,            # Build and run custom mutation target
    [switch]$EnableDocs = $false                   # Enable documentation generation
)

# ─────────────────────────────────────────────────────────────
# Exclusive check: Only one of the test modes can be active
$activeModes = @()
if ($EnableTests)         { $activeModes += "Tests" }
if ($EnableBenchmarks)    { $activeModes += "Benchmarks" }
if ($EnableMutationTests) { $activeModes += "MutationTests" }

if ($activeModes.Count -gt 1) {
    Write-Error "Only one of the following options can be used at the same time: -EnableTests, -EnableBenchmarks, -EnableMutationTests"
    exit 1
}

# ─────────────────────────────────────────────────────────────
# 1) Optionally clean the build directory
if ($CleanBuild -and (Test-Path $BuildDir)) {
    Write-Host "Cleaning build directory: $BuildDir"
    Remove-Item -Recurse -Force $BuildDir
}

# 2) Create build directory if it doesn't exist
if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# 3) Enter the build directory
Push-Location $BuildDir

# 4) If mutation testing is enabled, clear mutation output directory
if ($EnableMutationTests -and (Test-Path "tests\mutated")) {
    Write-Host "Cleaning mutation test output: tests/mutated"
    Remove-Item -Recurse -Force "tests\mutated"
}

# 5) Prepare CMake arguments
$cmakeArgs = @("$SourceDir")
$cmakeArgs += "-DCMAKE_BUILD_TYPE=$Config"

if ($EnableTests)         { $cmakeArgs += "-DUSE_TESTS=ON" }
if ($EnableBenchmarks)    { $cmakeArgs += "-DUSE_BENCHMARKS_R3=ON" }
if ($EnableMutationTests) { $cmakeArgs += "-DUSE_MUTATION_TESTS=ON" }
if ($EnableDocs) { $cmakeArgs += "-DGENERATE_DOCUMENTATION=ON" }


Write-Host "`n-- Configuring with CMake --"
cmake @cmakeArgs

# 6) If requested, build and run a specific target before main build
if ($RunMutatorTarget) {
    Write-Host "`n-- Building and running mutation target --"
    cmake --build . --target run_mutator_with_params_external --config $Config
    # Re-run cmake to refresh configuration after custom target
    cmake @cmakeArgs
}

# 7) Build the project
Write-Host "`n-- Building project --"
cmake --build . --config $Config

# 8) Optionally run tests (ctest)
if ($RunTests) {
    Write-Host "`n-- Running tests --"
    ctest --verbose --output-log "../ctest_output.txt" --build-config $Config
}

if ($EnableDocs) {
    Write-Host "`n-- Generating documentation --"
    cmake --build . --target doc --config $Config
}

# 9) Return to original directory
Pop-Location
