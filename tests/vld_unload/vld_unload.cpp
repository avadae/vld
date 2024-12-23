#include "CppUnitTest.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#ifdef _WIN64
static const TCHAR* sVld_dll = _T("vld_x64.dll");
#else
static const TCHAR* sVld_dll = _T("vld_x86.dll");
#endif

LONG VLDGetLeaksCount()
{
    HMODULE vld_module = GetModuleHandle(sVld_dll);
    if (vld_module != NULL) {
        typedef UINT(*VLDAPI_func)();
        VLDAPI_func func = (VLDAPI_func)GetProcAddress(vld_module, "VLDGetLeaksCount");
        assert(func);
        if (func) {
            return func();
        }
    }
    return -1;
}

LONG VLDReportLeaks()
{
    HMODULE vld_module = GetModuleHandle(sVld_dll);
    if (vld_module != NULL)
    {
        typedef UINT(*VLDAPI_func)();
        VLDAPI_func func = (VLDAPI_func)GetProcAddress(vld_module, "VLDReportLeaks");
        assert(func);
        if (func)
        {
            return func();
        }
    }
    return -1;
}

void ExpectLeakCount(int expected, int actual)
{
    if (expected != actual)
    {
        VLDReportLeaks();
    }
    Assert::AreEqual(expected, actual);
}

HMODULE GetModuleFromAddress(LPCVOID pAddress)
{
    HMODULE hModule = NULL;
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((LPCVOID)pAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == sizeof(MEMORY_BASIC_INFORMATION)) {
        // the allocation base is the beginning of a PE file
        hModule = (HMODULE)mbi.AllocationBase;
    }
    return hModule;
}

namespace vldunload
{
	TEST_CLASS(vldunload)
	{
	public:
		
		TEST_METHOD(TestUnloadDlls1)
		{
            Assert::IsNull(GetModuleHandle(sVld_dll));
            HMODULE hModule1 = ::LoadLibrary(_T("vld_dll1.dll"));
            Assert::IsNotNull(hModule1);
            int w = VLDGetLeaksCount(); // vld is loaded and counts 1 memory leak
            ExpectLeakCount(1, w);
            ::FreeLibrary(hModule1);    // vld is unloaded here and reports the memory leak
            int x = VLDGetLeaksCount(); // vld is unloaded and cannot count any memory leaks
            ExpectLeakCount(-1, x);
        
            HMODULE hModule2 = ::LoadLibrary(_T("vld_dll2.dll"));
            Assert::IsNotNull(hModule2);
            int y = VLDGetLeaksCount(); // vld is loaded and counts 1 memory leak
            ExpectLeakCount(1, y);
            ::FreeLibrary(hModule2);    // vld is unloaded here and reports the memory leak
            int z = VLDGetLeaksCount(); // vld is unloaded and cannot count any memory leaks
            ExpectLeakCount(-1, z);
        }

        TEST_METHOD(TestUnloadDlls2)
        {
            Assert::IsNull(GetModuleHandle(sVld_dll));
            HMODULE hModule3 = ::LoadLibrary(_T("vld_dll1.dll"));
            Assert::IsNotNull(hModule3);
            int w = VLDGetLeaksCount(); // vld is loaded and counts 1 memory leak
            ExpectLeakCount(1, w);
            HMODULE hModule4 = ::LoadLibrary(_T("vld_dll2.dll"));
            Assert::IsNotNull(hModule4);
            int x = VLDGetLeaksCount(); // vld is still loaded and counts 2 memory leaks
            ExpectLeakCount(2, x);
            ::FreeLibrary(hModule4);    // vld is *not* unloaded here
            int y = VLDGetLeaksCount();
            ExpectLeakCount(2, y); // vld is still loaded and counts 2 memory leaks
            ::FreeLibrary(hModule3);    // vld is unloaded here and reports 2 memory leaks
            int z = VLDGetLeaksCount(); // vld is unloaded and cannot count any memory leaks
            ExpectLeakCount(-1, z);
        }

        TEST_METHOD(TestUnloadDlls3)
        {
            Assert::IsNull(GetModuleHandle(sVld_dll));
            HMODULE hModule5 = ::LoadLibrary(_T("vld_dll1.dll"));
            Assert::IsNotNull(hModule5);
            Logger::WriteMessage("BeforeGetLeaks");
            int w = VLDGetLeaksCount(); // vld is loaded and counts 1 memory leak
            Logger::WriteMessage("AfterGetLeaks");
            ExpectLeakCount(1, w);
            HMODULE hModule6 = ::LoadLibrary(_T("vld_dll2.dll"));
            Assert::IsNotNull(hModule6);
            int x = VLDGetLeaksCount(); // vld is still loaded and counts 2 memory leaks
            ExpectLeakCount(2, x);
            ::FreeLibrary(hModule5);    // vld is *not* unloaded here
            int y = VLDGetLeaksCount(); // vld is still loaded and counts 2 memory leaks
            ExpectLeakCount(2, y); // vld is still loaded and counts 2 memory leaks
            ::FreeLibrary(hModule6);    // vld is unloaded here and reports 2 memory leaks
            int z = VLDGetLeaksCount(); // vld is unloaded and cannot count any memory leaks
            ExpectLeakCount(-1, z);
        }

        TEST_METHOD(TestUnloadDlls4)
        {
            Assert::IsNull(GetModuleHandle(sVld_dll));
            typedef FARPROC(__stdcall* GetProcAddress_t) (HMODULE, LPCSTR);
        
            HMODULE kernel32 = GetModuleHandleW(L"KernelBase.dll");
            if (!kernel32) {
                kernel32 = GetModuleHandleW(L"kernel32.dll");
            }
        
            // pGetProcAddress1 resolves to kernel32!GetProcAddress()
            GetProcAddress_t pGetProcAddress1 = GetProcAddress;
        
            HMODULE hModule7 = ::LoadLibrary(_T("vld_dll1.dll"));
            Assert::IsNotNull(hModule7);
            int w = VLDGetLeaksCount(); // vld is loaded and counts 1 memory leak
            ExpectLeakCount(1, w);
            // pGetProcAddress2 resolves to vld_xXX.dll!VisualLeakDetector::_GetProcAddress()
            GetProcAddress_t pGetProcAddress2 = GetProcAddress;
            Assert::AreNotEqual((void*)pGetProcAddress1, (void*)pGetProcAddress2);

            ::FreeLibrary(hModule7);    // vld is unloaded here and reports the memory leak
            int x = VLDGetLeaksCount(); // vld is unloaded and cannot count any memory leaks
            ExpectLeakCount(-1, x);
        }
    };
}
