#include <unity.h>
#include <sphere.h>

#include "arrlist.h"

void setUp() {}
void tearDown() {}

void test_create_arrlist() {
    ARRLIST_List* l = ARRLIST_new();
    TEST_ASSERT_NOT_NULL(l);
    ARRLIST_delete(l);
}

void test_arrlist_is_empty_with_new_list() {
    ARRLIST_List* l = ARRLIST_new();
    TEST_ASSERT_TRUE(ARRLIST_is_empty(l));
    ARRLIST_delete(l);
}

void test_arrlist_add_item() {
    ARRLIST_List* l = ARRLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    ARRLIST_add(l, s);
    TEST_ASSERT_FALSE(ARRLIST_is_empty(l));
    ARRLIST_delete(l);
    SPHERE_delete(s);
}

void test_arrlist_contains() {
    ARRLIST_List* l = ARRLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    TEST_ASSERT_FALSE(ARRLIST_contains(l, s));
    ARRLIST_add(l, s);
    TEST_ASSERT_TRUE(ARRLIST_contains(l, s));
    ARRLIST_delete(l);
    SPHERE_delete(s);
}

void test_arrlist_remove() {
    ARRLIST_List* l = ARRLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    ARRLIST_add(l, s);
    ARRLIST_add(l, s2);
    TEST_ASSERT_TRUE(ARRLIST_contains(l, s));
    TEST_ASSERT_TRUE(ARRLIST_contains(l, s2));
    ARRLIST_remove(l, s);
    TEST_ASSERT_FALSE(ARRLIST_contains(l, s));
    ARRLIST_remove(l, s2);
    TEST_ASSERT_FALSE(ARRLIST_contains(l, s2));
    TEST_ASSERT_TRUE(ARRLIST_is_empty(l));
    ARRLIST_delete(l);
    SPHERE_delete(s);
    SPHERE_delete(s2);
}

void test_arrlist_last() {
    ARRLIST_List* l = ARRLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    ARRLIST_add(l, s);
    TEST_ASSERT_EQUAL_PTR(s, ARRLIST_last(l));
    ARRLIST_add(l, s2);
    TEST_ASSERT_EQUAL_PTR(s2, ARRLIST_last(l));
    ARRLIST_remove(l, s2);
    TEST_ASSERT_EQUAL_PTR(s, ARRLIST_last(l));
    ARRLIST_delete(l);
    SPHERE_delete(s);
    SPHERE_delete(s2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_arrlist);
    RUN_TEST(test_arrlist_is_empty_with_new_list);
    RUN_TEST(test_arrlist_add_item);
    RUN_TEST(test_arrlist_contains);
    RUN_TEST(test_arrlist_remove);
    RUN_TEST(test_arrlist_last);
    UNITY_END();
}
