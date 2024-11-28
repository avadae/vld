#include "pch.h"
#include <tchar.h>
#include <Windows.h>
#include <string>
#include <iostream>

std::wstring dir;

std::wstring ExePath() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}
TEST(TestWinMain, RunExe)
{
    dir = ExePath();

    PROCESS_INFORMATION processInformation = { 0 };
    STARTUPINFO startupInfo = { 0 };
    startupInfo.cb = sizeof(startupInfo);

    std::wstring exe = dir + _T("\\vld_main.exe");

    // Create the process
    BOOL result = CreateProcess(exe.c_str(), NULL,
        NULL, NULL, FALSE,
        NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
        NULL, NULL, &startupInfo, &processInformation);
    EXPECT_NE(0, result);

    // Successfully created the process.  Wait for it to finish.
    EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(processInformation.hProcess, INFINITE));

    // Get the exit code.
    DWORD exitCode = 0;
    result = GetExitCodeProcess(processInformation.hProcess, &exitCode);
    EXPECT_NE(0, result);

    // Close the handles.
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);
    ASSERT_EQ(2, exitCode);
}