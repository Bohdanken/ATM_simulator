#pragma once
#ifndef ASSERT_UTILS_H
#define ASSERT_UTILS_H

#include <cassert>

#define assert_fail(expr)\
try { expr; assert(false); }\
catch (...) { assert(true); }

#endif // !ASSERT_UTILS_H
