#include <criterion/criterion.h>

int my_function(int x, int y)
{
    return x + y;
}

Test(my_function, test_addition)
{
    cr_assert_eq(my_function(2, 3), 5);
    cr_assert_eq(my_function(-2, 3), 1);
    cr_assert_eq(my_function(0, 0), 0);
}