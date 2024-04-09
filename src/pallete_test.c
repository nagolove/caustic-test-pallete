// vim: set colorcolumn=85
// vim: fdm=marker

// {{{ include
#include "koh_pallete.h"
#include <stdbool.h>
#include "koh_common.h"
#include "koh_pallete.h"
#include "koh_set.h"
#include "munit.h"
#include "raylib.h"
// }}}

static bool verbose = false;

static inline void _add(struct Pallete *p, koh_Set *s, Color c) {
    pallete_add(p, c);
    set_add(s, &c, sizeof(c));
}

static inline void _remove(struct Pallete *p, koh_Set *s, Color c) {
    pallete_remove(p, c);
    set_remove(s, &c, sizeof(c));
}

static MunitResult test_pallete_compare(
    const MunitParameter params[], void* data
) {

    {
        struct Pallete p1, p2;

        pallete_init(&p1, true);
        pallete_init(&p2, true);

        pallete_add(&p1, BLUE);
        pallete_add(&p1, RED);

        munit_assert(!pallete_compare(&p1, &p2));

        pallete_shutdown(&p2);
        pallete_shutdown(&p1);
    }

    {
        struct Pallete p1, p2;

        pallete_init(&p1, true);
        pallete_init(&p2, true);

        pallete_add(&p1, BLUE);
        pallete_add(&p1, RED);
        pallete_add(&p1, MAROON);

        pallete_add(&p2, RED);
        pallete_add(&p2, MAROON);
        pallete_add(&p2, BLUE);

        munit_assert(pallete_compare(&p1, &p2));

        pallete_shutdown(&p2);
        pallete_shutdown(&p1);
    }

    {
        struct Pallete p1, p2;

        pallete_init(&p1, true);
        pallete_init(&p2, true);

        munit_assert(pallete_compare(&p1, &p2));

        pallete_shutdown(&p2);
        pallete_shutdown(&p1);
    }

    return MUNIT_OK;
}

static MunitResult test_pallete_add_remove(
    const MunitParameter params[], void* data
) {
    koh_Set *set = set_new(NULL);
    struct Pallete p;
    pallete_init(&p, true);

    _add(&p, set, RED);
    _add(&p, set, GREEN);
    _add(&p, set, BLUE);
    _add(&p, set, BLACK);
    _add(&p, set, BLACK);

    _remove(&p, set, GREEN);

    munit_assert_int(p.num, ==, 3);
    for (int i = 0; i < p.num; i++) {
        Color c = p.colors[i];
        if (verbose)
            printf("c %s\n", color2str(c));
        munit_assert(set_exist(set, &c, sizeof(c)));
    }

    pallete_shutdown(&p);
    set_free(set);
    return MUNIT_OK;
}

static MunitResult test_pallete_init_shutdown(
    const MunitParameter params[], void* data
) {
    struct Pallete p;
    pallete_init(&p, true);
    munit_assert_int(p.num, ==, 0);
    pallete_shutdown(&p);
    return MUNIT_OK;
}

static MunitTest t_suite_common[] = {

    {
        .name =  (char*) "/test_pallete_init_shutdown",
        .test = test_pallete_init_shutdown,
        .setup = NULL,
        .tear_down = NULL,
        .options = MUNIT_TEST_OPTION_NONE,
        .parameters = NULL,
    },

    {
        .name =  (char*) "/test_pallete_compare",
        .test = test_pallete_compare,
        .setup = NULL,
        .tear_down = NULL,
        .options = MUNIT_TEST_OPTION_NONE,
        .parameters = NULL,
    },

    {
        .name =  (char*) "/test_pallete_add_remove",
        .test = test_pallete_add_remove,
        .setup = NULL,
        .tear_down = NULL,
        .options = MUNIT_TEST_OPTION_NONE,
        .parameters = NULL,
    },

    {
        .name =  NULL,
        .test = NULL,
        .setup = NULL,
        .tear_down = NULL,
        .options = MUNIT_TEST_OPTION_NONE,
        .parameters = NULL,
    },
};


static const MunitSuite suite_root = {
    .prefix = (char*) "pallete",
    .tests =  t_suite_common,
    /*.suites = suite_nested,*/
    .suites = NULL,
    .iterations = 1,
    .options = MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, char **argv) {
    koh_hashers_init();
    return munit_suite_main(&suite_root, (void*) "µnit", argc, argv);
}
