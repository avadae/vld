# Visual Leak Detector 

[![Build Status](https://github.com/avadae/vld/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/vld?logo=github&sort=semver)](https://github.com/avadae/vld/releases/latest)

## This personal fork

I maintain this fork to be used by students at [Hogeschool West-Vlaanderen in the bachelor Digital Arts and Entertainment](https://www.digitalartsandentertainment.be/). It is created to keep the vld project up to date with the latest visual studio versions and the latest installer tools. We merge all changes applied to the original Azure fork into this one. Please refer to [https://github.com/Azure/vld](https://github.com/Azure/vld) for the original version.

## Documentation

Read the documentation at [https://github.com/KindDragon/vld/wiki](https://github.com/KindDragon/vld/wiki)

## Updating dbghelp.dll

If you want to update the version of DbgHelp.dll in the project

- [Install the desired Windows SDK version.](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/)
- Copy dbghelp.dll from ```C:\Program Files (x86)\Windows Kits\10\Debuggers\x64``` to ```vld\setup\dbghelp\x64```
- Copy dbghelp.dll from ```C:\Program Files (x86)\Windows Kits\10\Debuggers\x86``` to ```vld\setup\dbghelp\x86```
- Update the version number in the manifest files in those folders
- Important: update the version number in the two dependency manifest files in ```vld\src```
