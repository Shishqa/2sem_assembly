#ifdef DEBUG
    #define ASSERT( cond );                                                                                                         \
    {                                                                                                                               \
        if (!(cond))                                                                                                                \
        {                                                                                                                           \
            printf ("\n\e[0;31m");                                                                                                  \
            printf ("Assertion failed: %s.\nFile %s,\nline %d, function %s()\n\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            printf ("\n\e[0m\n");                                                                                                   \
            abort ();                                                                                                               \
        }                                                                                                                           \
    }
    #define PRINTF printf
#else
    #define ASSERT( cond ); ;
    #define PRINTF if(0)printf
#endif // DEBUG
