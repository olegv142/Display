#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_FULL_ASSERT
#define BUG_ON_ENABLE
#endif

#define BUILD_BUG_ON(cond) extern void __build_bug_on_dummy(char a[1 - 2*!!(cond)])

#ifdef BUG_ON_ENABLE

void assertion_failed(const char* file, unsigned line);

#define BUG_ON(expr) do { if (expr) assertion_failed(__FILE__, __LINE__); } while (0)
#define BUG() do { assertion_failed(__FILE__, __LINE__); } while (0)

#else

#define BUG_ON(expr) do {} while (0)
#define BUG()        do {} while (0)

#endif

#ifdef __cplusplus
}
#endif
