
/**
 * Members of forth_object_t.
 *
 * Add this to `forth.h` near the comment saying "add type here":
 * ```c
 * my_data_type_t my_var_name;
 * ```
 *
 * Add this to `forth.h` near the comment saying "add include here":
 * ```c
 * #define TYPE my_var_name
 * #define NAME my_data_type_t
 * #include "type.inc"
 * ```
 */
#if !defined(NAME)
#error not defined: NAME
#endif

#if !defined(TYPE)
#error not defined: TYPE
#endif

// only once
#if !defined(FORTH_TYPE_SETUP_DONE)
#define FORTH_TYPE_SETUP_DONE

/// private impl of CAT(x, y)
#define PREPROC_CAT_(x, y) x ## y

/// private impl
static inline uint8_t forth_base_(void) {
    return __COUNTER__;
}
#endif

/// joins args
#define CAT(x, y) PREPROC_CAT_(x, y)

static inline uint8_t CAT(forth_type_, NAME)(void) {
    return __COUNTER__ - forth_base_();
}
static inline bool CAT(forth_is_, NAME)(forth_object_t obj) {
    return obj.tag == CAT(forth_type_, NAME)();
}

#define is_void_void 1

#if CAT(is_void_, TYPE)
// TYPE is void

static inline forth_object_t CAT(forth_of_, NAME)(void) {
    return (forth_object_t) {
        .tag = CAT(forth_type_, NAME)(),
    };
}

#else
// TYPE is not void

static inline forth_object_t CAT(forth_of_, NAME)(TYPE NAME) {
    return (forth_object_t) {
        .tag = CAT(forth_type_, NAME)(),
        .data.NAME = NAME,
    };
}
static inline TYPE CAT(forth_to_, NAME)(forth_object_t obj) {
    return obj.data.NAME;
}
static inline void CAT(forth_push_, NAME)(forth_context_t *ctx, TYPE NAME) {
    forth_push(ctx, CAT(forth_of_, NAME)(NAME));
}
static inline  TYPE CAT(forth_pop_, NAME)(forth_context_t *ctx) {
    return CAT(forth_to_, NAME)(forth_pop(ctx));
}
static inline void CAT(forth_define_, NAME)(forth_context_t *ctx, const char *name, TYPE NAME) {
    forth_define(ctx, name, CAT(forth_of_, NAME)(NAME));
}

#endif

#undef is_void_void
#undef TYPE
#undef NAME
#undef CAT
