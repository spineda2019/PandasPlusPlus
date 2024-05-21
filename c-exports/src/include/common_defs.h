#if defined _WIN32 || defined __CYGWIN__              // *
#ifdef __GNUC__                                       // **
#ifdef C_EXPORTS                                      // ***
#define C_API __attribute__((dllexport))              // ***
#else                                                 // ***
#define C_API __attribute__((dllimport))              // ***
#endif                                                // ***
#else                                                 // **
#ifdef C_EXPORTS                                      // ***
#define C_API __declspec(dllexport)                   // ***
#else                                                 // ***
#define C_API __declspec(dllimport)                   // ***
#endif                                                // ***
#endif                                                // **
#else                                                 // *
#if __GNUC__ >= 4                                     // **
#define C_API __attribute__((visibility("default")))  // **
#else                                                 // **
#define C_API                                         // **
#endif                                                // **
#endif                                                // *
