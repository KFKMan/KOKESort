$whlDir = "./dist"

$latestFile = Get-ChildItem -Path $whlDir -Filter *.whl |
              Sort-Object LastWriteTime -Descending |
              Select-Object -First 1

if ($latestFile) {
    Write-Host "Latest version: $($latestFile.Name)"
    pip install --force-reinstall "$($latestFile.FullName)"
} else {
    Write-Host "WHL File not found"
}