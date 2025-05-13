$folders = @("build", "dist", "KOKESort")
foreach ($folder in $folders) {
    if (Test-Path $folder) {
        Remove-Item -Recurse -Force $folder
    }
}

Get-ChildItem -Recurse -Directory -Filter "*egg-info*" | ForEach-Object {
    Write-Host "Deleting $($_.FullName)"
    Remove-Item $_.FullName -Recurse -Force
}