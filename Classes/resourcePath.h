//
//  resourcePath.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/28.
//
//

#ifndef resourcePath_h
#define resourcePath_h

#define LINE_SPACE         80

enum
{
    TEST_CONTACT_SOLVER = 0,
    TEST_BROADPHASE = 1,
    TEST_NARROWPHASE = 2,
    TEST_DEMO = 3,
    
    // last one
    TESTS_COUNT,
};

const std::string g_aTestNames[TESTS_COUNT] = {
    "ContactSolverTest",
    "BroadphaseTest",
    "NarrowphaseTest",
    "SimpleGameDemo",
};

static const char s_pPathClose[]          = "Images/close.png";

#endif /* resourcePath_h */
