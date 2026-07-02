# **README.md — Reset‑LanguageAndKeyboard.ps1**

## **Overview**
`Reset-LanguageAndKeyboard.ps1` is a Windows administration script designed to fully standardise the system language configuration and keyboard layouts across all users.  
It removes transitional and installed Local Experience Packs (LEPs), enforces a deterministic language baseline, and ensures that **US‑International** is the only keyboard layout available system‑wide.

This script is intended for environments where consistent input behaviour is required, such as shared workstations, kiosks, labs, or tightly controlled enterprise deployments.

---

## **Features**
### **1. Remove all Local Experience Packs**
- Removes **provisioned** LEPs (installed for future users)
- Removes **installed** LEPs (installed for current and past users)
- Ensures no residual language packs remain on the system

### **2. Enforce English (Australia) as the system language**
- Sets **en‑AU** as the system locale, UI language, and user language
- Adds **en‑GB** as a fallback display language for missing AU resources

### **3. Remove all keyboard layouts**
- Clears modern TIP‑based keyboard layouts from the Windows language list
- Removes legacy keyboard layouts from:
  - Current user (`HKCU`)
  - System account (`HKU\.DEFAULT`)
- Prevents Windows from resurrecting unwanted layouts

### **4. Enforce US‑International keyboard system‑wide**
- Adds **US‑International (00020409)** as the only keyboard layout
- Applies to:
  - Modern language list
  - Current user legacy keyboard list
  - System account legacy keyboard list
- Ensures consistent accented‑character support for all users

---

## **Why this script exists**
Windows maintains keyboard layouts in **three different subsystems**:

1. **Modern language list (InputMethodTips)**  
2. **Legacy registry keyboard list (HKCU\Keyboard Layout\Preload)**  
3. **System account defaults (HKU\.DEFAULT\Keyboard Layout\Preload)**  

Even if the modern language list is clean, Windows may silently load keyboards from the legacy or system account lists.  
This causes:
- Random keyboard switching  
- Loss of accent key functionality  
- Appearance of unwanted layouts (e.g., US, AU default)  

This script eliminates all three sources and enforces a single, stable keyboard layout.

---

## **Requirements**
- Windows 10 or Windows 11  
- PowerShell running **as Administrator**  
- Internet access **not required**  
- No external modules required  

---

## **Usage**
Run the script from an elevated PowerShell session:

```powershell
.\Reset-LanguageAndKeyboard.ps1
```

The script is **idempotent** — running it multiple times will not cause errors or duplicate entries.

A reboot is required after execution.

---

## **What to expect after running the script**
### ✔ System language set to English (Australia)  
### ✔ Display fallback set to English (United Kingdom)  
### ✔ All LEPs removed  
### ✔ All legacy keyboards removed  
### ✔ Only US‑International keyboard remains  
### ✔ Accent keys work consistently  
### ✔ Windows will not switch back to US or AU default layouts  

---

## **Rollback / Undo**
If you need to restore the default Windows behaviour:

### Restore default language list:
```powershell
$LangList = New-WinUserLanguageList en-US
Set-WinUserLanguageList $LangList -Force
```

### Restore default keyboards:
```powershell
reg delete "HKCU\Keyboard Layout\Preload" /f
reg delete "HKU\.DEFAULT\Keyboard Layout\Preload" /f
```

### Reinstall LEPs (example):
```powershell
winget install Microsoft.LanguageExperiencePacken-US
```

---

## **Deployment Notes**
This script is suitable for:
- Intune (Device Startup Script)
- MDT / SCCM Task Sequences
- Group Policy Startup Scripts
- Golden image preparation
- Shared workstation baselines

If deploying via Intune, ensure:
- Script runs in **system context**
- Script runs **before** user profile creation

---

## **Versioning**
**Version:** 1.0  
**Author:** Vincent  
**Purpose:** System‑wide language and keyboard standardisation  
**Script:** `Reset-LanguageAndKeyboard.ps1`

---
End of document
