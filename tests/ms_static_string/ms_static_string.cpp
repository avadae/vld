#include <vld.h>
#include "CppUnitTest.h"
#include "Allocs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace my_string
{
    const std::string the_string("foobar");
}

const std::string string_global("xyz1234567");

void access_strings()
{
    // Just do something with the string so it isn't optimized away
    std::string copied_string = my_string::the_string;
    printf("Copied string %s\n", copied_string.c_str());

    std::string copied_string2 = string_global;
    printf("Copied string %s\n", copied_string2.c_str());
}

namespace msstaticstring
{
	TEST_CLASS(msstaticstring)
	{
	public:

        TEST_METHOD_INITIALIZE(Setup)
        {
            VLDMarkAllLeaksAsReported();
        }
		
		TEST_METHOD(TestStaticStrings)
		{
            int leaks = static_cast<int>(VLDGetLeaksCount());
            Assert::AreEqual(0, leaks);

            access_strings();

            leaks = static_cast<int>(VLDGetLeaksCount());
            Assert::AreEqual(0, leaks);
		}

        //TEST_METHOD(TestMalloc)
        //{
        //    bool bFreeMemory = true; // todo: this used to be a parameter
        //    int prev = static_cast<int>(VLDGetLeaksCount());
        //    LeakMemoryMalloc(repeats, bFreeMemory);
        //    int total = static_cast<int>(VLDGetLeaksCount());
        //    int leaks = total - prev;
        //    int correctLeaks = bFreeMemory ? 0 : repeats * 2;
        //    Assert::AreEqual(correctLeaks, leaks);
        //}

        TEST_METHOD_CLEANUP(TearDown)
        {
            // Check that callstack resolved without unresolved functions (required symbols for all dll's)
            Assert::AreEqual(0, VLDResolveCallstacks());
        }
	};
}
