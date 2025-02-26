#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "GetInsertIndex.h"  // GetInsertIndex başlık dosyası

// Basit bir test fonksiyonu
static void test_get_insert_index(void **state) {
    LinkedListInt arr[3] = {{5, NULL, NULL}, {10, NULL, NULL}, {15, NULL, NULL}};
    size_t size = 3;
    
    size_t index = GetInsertIndex(arr, size, 10);
    assert_int_equal(index, 1);
}

// Ana test fonksiyonu
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_insert_index),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}