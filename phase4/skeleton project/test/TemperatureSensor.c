#include "unity.h"
#include "TemperatureSensor.h"

void
test_Temperature_Sensor(void)
{
    TEST_ASSERT_TRUE_MESSAGE(1 == 1, "The most basic test has failed, something is likely configured wrong.");
}

int 
main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Temperature_Sensor);
    return UNITY_END();
}
