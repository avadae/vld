#include "pch.h"
#include <vld.h>

namespace my_string
{
    const std::string the_string("foobar");
}

const std::string string_global("xyz1234567");

class StaticStringTest : public ::testing::Test
{
    virtual void SetUp()
    {
        VLDMarkAllLeaksAsReported();
    }
    virtual void TearDown()
    {
        // Check that callstack resolved without unresolved functions (required symbols for all dll's)
        EXPECT_EQ(0, VLDResolveCallstacks());
    }
};

void access_strings()
{
    // Just do something with the string so it isn't optimized away
    std::string copied_string = my_string::the_string;
    printf("Copied string %s\n", copied_string.c_str());

    std::string copied_string2 = string_global;
    printf("Copied string %s\n", copied_string2.c_str());
}

TEST_F(StaticStringTest, StaticStringsSuccess)
{
    int leaks = static_cast<int>(VLDGetLeaksCount());
    ASSERT_EQ(0, leaks);

    access_strings();

    leaks = static_cast<int>(VLDGetLeaksCount());
    ASSERT_EQ(0, leaks);
}