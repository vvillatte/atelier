# ============================================================
# 0. REQUIRE ADMIN
# ============================================================
If (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(`
    [Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "ERROR: Run PowerShell as Administrator." -ForegroundColor Red
    exit 1
}

Write-Host "Starting full language + keyboard cleanup..." -ForegroundColor Cyan

# ============================================================
# 1. REMOVE ALL PROVISIONED LOCAL EXPERIENCE PACKS (LEPs)
# ============================================================
Write-Host "`nRemoving provisioned Local Experience Packs..." -ForegroundColor Yellow

$lep = dism /online /get-provisionedappxpackages |
        Select-String "LanguageExperiencePack"

foreach ($pkg in $lep) {
    $name = ($pkg -split ":")[1].Trim()
    Write-Host "Removing provisioned LEP: $name"
    dism /online /Remove-ProvisionedAppxPackage /PackageName:$name
}

# ============================================================
# 2. REMOVE ALL INSTALLED LEPs FOR ALL USERS
# ============================================================
Write-Host "`nRemoving installed LEPs for all users..." -ForegroundColor Yellow

Get-AppxPackage -AllUsers *LanguageExperiencePack* |
    ForEach-Object {
        Write-Host "Removing installed LEP: $($_.PackageFullName)"
        Remove-AppxPackage -Package $_.PackageFullName -AllUsers
    }

# ============================================================
# 3. SET SYSTEM LANGUAGE TO EN-AU (WITH EN-GB FALLBACK)
# ============================================================
Write-Host "`nSetting system language to English (Australia)..." -ForegroundColor Yellow

# Force AU everywhere
Set-WinUILanguageOverride en-AU
Set-WinSystemLocale en-AU

# Create language list with fallback to en-GB
$LangList = New-WinUserLanguageList en-AU
$LangList.Add("en-GB")
Set-WinUserLanguageList $LangList -Force

# ============================================================
# 4. REMOVE ALL KEYBOARDS (MODERN + LEGACY)
# ============================================================
Write-Host "`nCleaning all keyboard layouts (modern + legacy)..." -ForegroundColor Yellow

# Remove all TIPs from modern language list
$LangList = Get-WinUserLanguageList
foreach ($lang in $LangList) {
    $lang.InputMethodTips.Clear()
}
Set-WinUserLanguageList $LangList -Force

# Remove legacy keyboards (HKCU)
reg delete "HKCU\Keyboard Layout\Preload" /f | Out-Null

# Remove legacy keyboards (System account)
reg delete "HKU\.DEFAULT\Keyboard Layout\Preload" /f | Out-Null

# ============================================================
# 5. ADD ONLY US-INTERNATIONAL KEYBOARD (SYSTEM-WIDE)
# ============================================================
Write-Host "`nAssigning US-International keyboard system-wide..." -ForegroundColor Yellow

# TIP for US-International
$USIntl = "00020409"

# Add to modern language list
$LangList = Get-WinUserLanguageList
foreach ($lang in $LangList) {
    $lang.InputMethodTips.Add("0C09:$USIntl")  # AU + US-Intl TIP
}
Set-WinUserLanguageList $LangList -Force

# Add to legacy user keyboard list
reg add "HKCU\Keyboard Layout\Preload" /v 1 /t REG_SZ /d $USIntl /f | Out-Null

# Add to system account keyboard list
reg add "HKU\.DEFAULT\Keyboard Layout\Preload" /v 1 /t REG_SZ /d $USIntl /f | Out-Null

# ============================================================
# 6. FINAL MESSAGE
# ============================================================
Write-Host "`nAll tasks complete. Please reboot your system." -ForegroundColor Green
