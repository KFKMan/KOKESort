# Check if the build directory exists and delete it
$buildDir = "build"
if (Test-Path $buildDir) {
    Write-Host "Build folder found, deleting..."
    Remove-Item -Recurse -Force $buildDir
    Write-Host "Build folder deleted."
}