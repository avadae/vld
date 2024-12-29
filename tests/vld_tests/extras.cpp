#include <vld.h>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace {
    class some_class {
    public:
        std::unique_ptr<int> pPointer{ std::make_unique<int>() };
    };

	thread_local some_class s_some_class_thread_local;
    static some_class s_some_class_static;

    thread_local std::unique_ptr<int> pPointer_thread_local{ std::make_unique<int>() };
    static std::unique_ptr<int> pPointer{ std::make_unique<int>() };

	void test_thread_local()
	{
		Assert::IsNotNull(pPointer.get());
        Assert::IsNotNull(pPointer_thread_local.get());
        Assert::IsNotNull(s_some_class_static.pPointer.get());
        Assert::IsNotNull(s_some_class_thread_local.pPointer.get());
	}
}



namespace vld_tests
{
    /// <summary>
	/// Tests bases on cases reported by users.
    /// </summary>
    TEST_CLASS(extras)
    {
    public:

        TEST_METHOD_INITIALIZE(Setup)
        {
            VLDMarkAllLeaksAsReported();
        }

        TEST_METHOD(TestThreadLocal)
        {
            int leaks = static_cast<int>(VLDGetLeaksCount());
            Assert::AreEqual(0, leaks);

			test_thread_local();

            leaks = static_cast<int>(VLDGetLeaksCount());
            Assert::AreEqual(0, leaks);
        }

        TEST_METHOD_CLEANUP(TearDown)
        {
            // Check that callstack resolved without unresolved functions (required symbols for all dll's)
            Assert::AreEqual(0, VLDResolveCallstacks());
        }
    };
}
