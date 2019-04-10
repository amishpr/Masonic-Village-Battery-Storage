#include "unity.h"
#include "TrivialTest.h"

void
test_UnitTests_Are_Working(void)
{
    TEST_ASSERT_TRUE_MESSAGE(1 == 1, "The most basic test has failed, something is likely configured wrong.");
}

int 
main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_UnitTests_Are_Working);
    return UNITY_END();
}
