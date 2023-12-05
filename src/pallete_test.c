// vim: set colorcolumn=85
// vim: fdm=marker

// {{{ include
#include "koh.h"
#include "koh_pallete.h"
#include "koh_set.h"
#include "munit.h"
#include "raylib.h"
// }}}

/*static bool verbose = false;*/

static inline void _add(struct Pallete *p, koh_Set *s, Color c) {
    pallete_add(p, c);
    set_add(s, &c, sizeof(c));
}

static inline void _remove(struct Pallete *p, koh_Set *s, Color c) {
    pallete_remove(p, c);
    set_remove(s, &c, sizeof(c));
}

static MunitResult test_pallete_add_remove(
    const MunitParameter params[], void* data
) {
    koh_Set *set = set_new(NULL);
    struct Pallete p;
    pallete_init(&p, false);

    _add(&p, set, RED);
    _add(&p, set, GREEN);
    _add(&p, set, BLUE);
    _add(&p, set, BLACK);
    _add(&p, set, BLACK);

    _remove(&p, set, GREEN);

    for (int i = 0; i < p.num; i++) {
        Color c = p.colors[i];
        munit_assert(set_exist(set, &c, sizeof(c)));
    }

    pallete_shutdown(&p);
    set_free(set);

    return MUNIT_OK;
}

static MunitTest t_suite_common[] = {
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
    return munit_suite_main(&suite_root, (void*) "µnit", argc, argv);
}
