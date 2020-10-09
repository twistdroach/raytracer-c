#include <unity.h>
#include <sphere.h>

#include "linkedlist.h"

void setUp() {}
void tearDown() {}

void test_create_linkedlist() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    TEST_ASSERT_NOT_NULL(l);
    LINKEDLIST_delete(l);
}

void test_linkedlist_is_empty_with_new_list() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    TEST_ASSERT_TRUE(LINKEDLIST_is_empty(l));
    LINKEDLIST_delete(l);
}

void test_linkedlist_add_item() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    LINKEDLIST_add(l, s);
    TEST_ASSERT_FALSE(LINKEDLIST_is_empty(l));
    LINKEDLIST_delete(l);
    SPHERE_delete(s);
}

void test_linkedlist_contains() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    TEST_ASSERT_FALSE(LINKEDLIST_contains(l, s));
    LINKEDLIST_add(l, s);
    TEST_ASSERT_TRUE(LINKEDLIST_contains(l, s));
    LINKEDLIST_delete(l);
    SPHERE_delete(s);
}

void test_linkedlist_remove() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    LINKEDLIST_add(l, s);
    LINKEDLIST_add(l, s2);
    TEST_ASSERT_TRUE(LINKEDLIST_contains(l, s));
    TEST_ASSERT_TRUE(LINKEDLIST_contains(l, s2));
    LINKEDLIST_remove(l, s);
    TEST_ASSERT_EQUAL(1, LINKEDLIST_item_count(l));
    TEST_ASSERT_FALSE(LINKEDLIST_contains(l, s));
    LINKEDLIST_remove(l, s2);
    TEST_ASSERT_FALSE(LINKEDLIST_contains(l, s2));
    TEST_ASSERT_TRUE(LINKEDLIST_is_empty(l));
    LINKEDLIST_delete(l);
    SPHERE_delete(s);
    SPHERE_delete(s2);
}

void test_linkedlist_last() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    LINKEDLIST_add(l, s);
    TEST_ASSERT_EQUAL_PTR(s, LINKEDLIST_last(l));
    LINKEDLIST_add(l, s2);
    TEST_ASSERT_EQUAL_PTR(s2, LINKEDLIST_last(l));
    LINKEDLIST_remove(l, s2);
    TEST_ASSERT_EQUAL_PTR(s, LINKEDLIST_last(l));
    LINKEDLIST_delete(l);
    SPHERE_delete(s);
    SPHERE_delete(s2);
}

void test_linkedlist_safe_get() {
    LINKEDLIST_List* l = LINKEDLIST_new();
    SPHERE_Sphere* s = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    LINKEDLIST_add(l, s);
    LINKEDLIST_add(l, s2);
    TEST_ASSERT_EQUAL_PTR(s, LINKEDLIST_safe_get(l, 0));
    LINKEDLIST_delete(l);
    SPHERE_delete(s);
    SPHERE_delete(s2);
}

void test_linkedlist_safe_get_should_throw_on_bad_index() {
    CEXCEPTION_T e;
    LINKEDLIST_List* l = LINKEDLIST_new();
    Try {
       LINKEDLIST_safe_get(l, 1);
       TEST_FAIL_MESSAGE("safe get should throw exception when given a bad index");
    }
    Catch(e) {
       TEST_ASSERT_EQUAL(E_INDEX_OUT_OF_BOUNDS, e);
    }
    LINKEDLIST_delete(l);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_linkedlist);
    RUN_TEST(test_linkedlist_is_empty_with_new_list);
    RUN_TEST(test_linkedlist_add_item);
    RUN_TEST(test_linkedlist_contains);
    RUN_TEST(test_linkedlist_remove);
    RUN_TEST(test_linkedlist_last);
    RUN_TEST(test_linkedlist_safe_get_should_throw_on_bad_index);
    RUN_TEST(test_linkedlist_safe_get);
    UNITY_END();
}
