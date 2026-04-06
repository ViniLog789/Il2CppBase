#include "test.h"

void Test::TestFeatures1() // An in-game method call is required!
{
    if (!IsDebugging) return;
    if (!ForTestFlag) return;

    ForTestFlag = false;
    LOG_INFO("The test executed");
}