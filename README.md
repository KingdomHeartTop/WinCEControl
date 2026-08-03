# WinCEControl

**Universal remote management tool for Windows CE devices**

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Platform](https://img.shields.io/badge/platform-Windows%20CE-red)](https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types)

## 🎯 What is WinCEControl?

WinCEControl is a powerful cross-device utility for managing any Windows CE-powered device via USB/serial cable. Whether you're working with:

- 🗺️ **GPS Navigators** (Garmin, TomTom, Pioneer)
- 📦 **Industrial terminals** (Honeywell, Zebra, Datalogic)
- 📊 **Data collectors** and **scanners**
- 📱 **Legacy PDAs** and **portable devices**

This tool gives you **full control** over the device's file system, processes, registry, and system shell.

## 🚀 Key Features

- 🔌 **Universal device detection** via RAPI (works with any CE device)
- 📂 **Full file system access** (browse, copy, delete, create directories)
- 📦 **Install/Uninstall applications** (.CAB packages)
- 🔄 **Replace system shell** (launcher) on any device
- 🗑️ **Force-kill processes** and manage running apps
- 🖥️ **Device information** (OS version, CPU architecture, memory)
- ⚠️ **Works on ARM, MIPS, SH4, x86** architectures

## ⚠️ Important Notice

> **WARNING:** This tool can permanently damage your device if used improperly. Always:
> - **Backup** system files before modifying
> - **Test** on non-critical devices first
> - **Understand** what each function does

## 🔧 Requirements

- **Windows PC** with ActiveSync or Windows Mobile Device Center installed
- **USB cable** compatible with your CE device
- **Visual Studio 2019/2022** with Windows CE SDK (for compilation)

## 📦 Installation

```bash
git clone https://github.com/KingdomHeartTop/WinCEControl.git
cd WinCEControl
# Open WinCEControl.sln in Visual Studio and build
