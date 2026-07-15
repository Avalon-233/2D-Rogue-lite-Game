param(
    [string]$Destination = 'D:\LightRogue_Final_Release_x64'
)

$ErrorActionPreference = 'Stop'

$projectRoot = Split-Path -Parent $PSCommandPath
$releaseDir = Join-Path $projectRoot 'x64\Release'
$resourceDir = Join-Path $releaseDir 'resource'
$runtimeInstaller = 'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Redist\MSVC\14.51.36231\vc_redist.x64.exe'
$runtimeDir = 'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Redist\MSVC\14.51.36231\x64\Microsoft.VC145.CRT'

$requiredFiles = @(
    'LightRogue.exe',
    'sfml-system-3.dll',
    'sfml-window-3.dll',
    'sfml-graphics-3.dll',
    'sfml-audio-3.dll'
)

$requiredRuntimeFiles = @('msvcp140.dll', 'vcruntime140.dll', 'vcruntime140_1.dll')

foreach ($file in $requiredFiles) {
    $path = Join-Path $releaseDir $file
    if (-not (Test-Path -LiteralPath $path -PathType Leaf)) {
        throw "Release dependency is missing: $path"
    }
}

if (-not (Test-Path -LiteralPath $resourceDir -PathType Container)) {
    throw "Resource directory is missing: $resourceDir"
}

if (-not (Test-Path -LiteralPath $runtimeInstaller -PathType Leaf)) {
    throw "Visual C++ Redistributable installer is missing: $runtimeInstaller"
}

foreach ($file in $requiredRuntimeFiles) {
    $path = Join-Path $runtimeDir $file
    if (-not (Test-Path -LiteralPath $path -PathType Leaf)) {
        throw "Visual C++ runtime library is missing: $path"
    }
}

if (Test-Path -LiteralPath $Destination) {
    throw "Destination already exists. Review it manually before replacing: $Destination"
}

New-Item -ItemType Directory -Path $Destination | Out-Null
Copy-Item -LiteralPath (Join-Path $releaseDir 'LightRogue.exe') -Destination $Destination
foreach ($file in $requiredFiles | Where-Object { $_ -ne 'LightRogue.exe' }) {
    Copy-Item -LiteralPath (Join-Path $releaseDir $file) -Destination $Destination
}
Copy-Item -LiteralPath $resourceDir -Destination (Join-Path $Destination 'resource') -Recurse
Copy-Item -LiteralPath $runtimeInstaller -Destination (Join-Path $Destination 'vc_redist.x64.exe')
foreach ($file in $requiredRuntimeFiles) {
    Copy-Item -LiteralPath (Join-Path $runtimeDir $file) -Destination $Destination
}
Copy-Item -LiteralPath (Join-Path $projectRoot 'release_package\InstallRuntime.bat') -Destination $Destination
Copy-Item -LiteralPath (Join-Path $projectRoot 'release_package\RunGame.bat') -Destination $Destination
Copy-Item -LiteralPath (Join-Path $projectRoot 'release_package\README.txt') -Destination $Destination

Get-ChildItem -LiteralPath $Destination -File -Recurse |
    Get-FileHash -Algorithm SHA256 |
    ForEach-Object { "{0} *{1}" -f $_.Hash, $_.Path.Substring($Destination.Length + 1) } |
    Set-Content -LiteralPath (Join-Path $Destination 'SHA256SUMS.txt') -Encoding ascii

Write-Host "Portable Windows x64 package created: $Destination"
