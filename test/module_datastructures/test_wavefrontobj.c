#include <unity.h>
#include <string.h>
#include <tuples.h>
#include <exceptions.h>
#include <group.h>
#include <triangle.h>

#include "wavefrontobj.h"

#include "testutils.h"

void setUp() {}
void tearDown() {}

WAVEFRONTOBJ_Obj* from_string(char* str) {
    TEST_ASSERT_NOT_NULL(str);
    FILE* stream = fmemopen(str, strlen(str), "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(stream, "fmemopen failed");
    WAVEFRONTOBJ_Obj* obj = WAVEFRONTOBJ_parse_obj_stream(stream);
    fclose(stream);
    TEST_ASSERT_NOT_NULL(obj);
    return obj;
}

void test_ignore_unrecognized_lines() {
    char* gibberish = "Test line 1\n"
                      "Test line 2\n"
                      "Another test line\n";
    WAVEFRONTOBJ_Obj* obj = from_string(gibberish);
    TEST_ASSERT_EQUAL(3, obj->ignored_lines);
    WAVEFRONTOBJ_delete(obj);
}

void check_point(TUPLES_Point* p, double x, double y, double z) {
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_TRUE(TUPLES_is_point(p));
    TEST_ASSERT_EQUAL_DOUBLE(x, p->x);
    TEST_ASSERT_EQUAL_DOUBLE(y, p->y);
    TEST_ASSERT_EQUAL_DOUBLE(z, p->z);
}

void check_vector(TUPLES_Vector* v, double x, double y, double z) {
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_TRUE(TUPLES_is_vector(v));
    TEST_ASSERT_EQUAL_DOUBLE(x, v->x);
    TEST_ASSERT_EQUAL_DOUBLE(y, v->y);
    TEST_ASSERT_EQUAL_DOUBLE(z, v->z);
}

void test_parse_vertex() {
    char* vertexes = "\n"
                     "v -1 1 0\n"
                     "v -1.0000 0.5000 0.0000\n"
                     "v 1 0 0\n"
                     "v 1 1 0\n"
                     "\n";
    WAVEFRONTOBJ_Obj* obj = from_string(vertexes);
    CEXCEPTION_T e;
    Try {
        check_point(WAVEFRONTOBJ_get_vertex(obj, 1), -1, 1, 0);
        check_point(WAVEFRONTOBJ_get_vertex(obj, 2), -1, 0.5, 0);
        check_point(WAVEFRONTOBJ_get_vertex(obj, 3), 1, 0, 0);
        check_point(WAVEFRONTOBJ_get_vertex(obj, 4), 1, 1, 0);
    }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
    WAVEFRONTOBJ_delete(obj);
}

void test_parse_triangle_face() {
    char* data = "\n"
                 "v -1 1 0\n"
                 "v -1 0 0\n"
                 "v 1 0 0\n"
                 "v 1 1 0\n"
                 "\n"
                 "f 1 2 3\n"
                 "f 1 3 4\n"
                 "\n";

    CEXCEPTION_T e;
    Try {
        WAVEFRONTOBJ_Obj* obj = from_string(data);
        GROUP_Group* g = WAVEFRONTOBJ_get_default_group(obj);
        TRIANGLE_Triangle* t1 = (TRIANGLE_Triangle*)GROUP_get_child(g, 0);
        TRIANGLE_Triangle* t2 = (TRIANGLE_Triangle*)GROUP_get_child(g, 1);
        test_tuples(&t1->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
        test_tuples(&t1->p2, WAVEFRONTOBJ_get_vertex(obj, 2));
        test_tuples(&t1->p3, WAVEFRONTOBJ_get_vertex(obj, 3));
        test_tuples(&t2->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
        test_tuples(&t2->p2, WAVEFRONTOBJ_get_vertex(obj, 3));
        test_tuples(&t2->p3, WAVEFRONTOBJ_get_vertex(obj, 4));
        WAVEFRONTOBJ_delete(obj);
    }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
}

void test_triangulating_polygons() {
    char* data = "\n"
                 "v -1 1 0\n"
                 "v -1 0 0\n"
                 "v 1 0 0\n"
                 "v 1 1 0\n"
                 "v 0 2 0\n"
                 "\n"
                 "f 1 2 3 4 5\n"
                 "\n";
    CEXCEPTION_T e;
    Try {
                WAVEFRONTOBJ_Obj* obj = from_string(data);
                GROUP_Group* g = WAVEFRONTOBJ_get_default_group(obj);
                TRIANGLE_Triangle* t1 = (TRIANGLE_Triangle*)GROUP_get_child(g, 0);
                TRIANGLE_Triangle* t2 = (TRIANGLE_Triangle*)GROUP_get_child(g, 1);
                TRIANGLE_Triangle* t3 = (TRIANGLE_Triangle*)GROUP_get_child(g, 2);
                test_tuples(&t1->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
                test_tuples(&t1->p2, WAVEFRONTOBJ_get_vertex(obj, 2));
                test_tuples(&t1->p3, WAVEFRONTOBJ_get_vertex(obj, 3));
                test_tuples(&t2->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
                test_tuples(&t2->p2, WAVEFRONTOBJ_get_vertex(obj, 3));
                test_tuples(&t2->p3, WAVEFRONTOBJ_get_vertex(obj, 4));
                test_tuples(&t3->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
                test_tuples(&t3->p2, WAVEFRONTOBJ_get_vertex(obj, 4));
                test_tuples(&t3->p3, WAVEFRONTOBJ_get_vertex(obj, 5));
                WAVEFRONTOBJ_delete(obj);
            }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
}

void test_parse_from_file() {
    WAVEFRONTOBJ_Obj* obj;
    CEXCEPTION_T e;
    Try {
                obj = WAVEFRONTOBJ_parse_file_by_name("triangles.obj");
    }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
    WAVEFRONTOBJ_delete(obj);
}

void test_triangles_in_groups() {
    WAVEFRONTOBJ_Obj* obj;
    CEXCEPTION_T e;
    Try {
        obj = WAVEFRONTOBJ_parse_file_by_name("triangles.obj");
        GROUP_Group* def_group = WAVEFRONTOBJ_get_default_group(obj);
        GROUP_Group* g1 = GROUP_get_child(def_group, 0);
        GROUP_Group* g2 = GROUP_get_child(def_group, 1);
        TRIANGLE_Triangle* t1 = GROUP_get_child(g1, 0);
        TRIANGLE_Triangle* t2 = GROUP_get_child(g2, 0);
        test_tuples(&t1->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
        test_tuples(&t1->p2, WAVEFRONTOBJ_get_vertex(obj, 2));
        test_tuples(&t1->p3, WAVEFRONTOBJ_get_vertex(obj, 3));
        test_tuples(&t2->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
        test_tuples(&t2->p2, WAVEFRONTOBJ_get_vertex(obj, 3));
        test_tuples(&t2->p3, WAVEFRONTOBJ_get_vertex(obj, 4));
    }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
    WAVEFRONTOBJ_delete(obj);
}

void test_parse_many_poly_teapot() {
    WAVEFRONTOBJ_Obj* obj;
    CEXCEPTION_T e;
    Try {
                obj = WAVEFRONTOBJ_parse_file_by_name("teapot.obj");
                TEST_ASSERT_EQUAL(2256, obj->face_count);
            }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
    WAVEFRONTOBJ_delete(obj);
}

void test_vertex_normal_records() {
    char* data = "\n"
                 "vn 0 0 1\n"
                 "vn 0.707 0 -0.707\n"
                 "vn 1 2 3\n"
                 "\n";

    CEXCEPTION_T e;
    Try {
                WAVEFRONTOBJ_Obj *obj = from_string(data);
                check_vector(WAVEFRONTOBJ_get_normal(obj, 1), 0, 0, 1);
                check_vector(WAVEFRONTOBJ_get_normal(obj, 2), 0.707, 0, -0.707);
                check_vector(WAVEFRONTOBJ_get_normal(obj, 3), 1, 2, 3);
                WAVEFRONTOBJ_delete(obj);
    }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
}

void test_faces_with_normals() {
    char* data = "\n"
                 "v 0 1 0\n"
                 "v -1 0 0\n"
                 "v 1 0 0\n"
                 "\n"
                 "vn -1 0 0\n"
                 "vn 1 0 0\n"
                 "vn 0 1 0\n"
                 "\n"
                 "f 1//3 2//1 3//2\n"
                 "f 1/0/3 2/102/1 3/14/2\n"
                 "\n";

    CEXCEPTION_T e;
    Try {
                WAVEFRONTOBJ_Obj *obj = from_string(data);
                GROUP_Group* g = WAVEFRONTOBJ_get_default_group(obj);
                TRIANGLE_SmoothTriangle* t1 = (TRIANGLE_SmoothTriangle*)GROUP_get_child(g, 0);
                TRIANGLE_SmoothTriangle* t2 = (TRIANGLE_SmoothTriangle*)GROUP_get_child(g, 1);
                test_tuples(&t1->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
                test_tuples(&t1->p2, WAVEFRONTOBJ_get_vertex(obj, 2));
                test_tuples(&t1->p3, WAVEFRONTOBJ_get_vertex(obj, 3));
                test_tuples(&t1->n1, WAVEFRONTOBJ_get_normal(obj, 3));
                test_tuples(&t1->n2, WAVEFRONTOBJ_get_normal(obj, 1));
                test_tuples(&t1->n3, WAVEFRONTOBJ_get_normal(obj, 2));
                test_tuples(&t2->p1, WAVEFRONTOBJ_get_vertex(obj, 1));
                test_tuples(&t2->p2, WAVEFRONTOBJ_get_vertex(obj, 2));
                test_tuples(&t2->p3, WAVEFRONTOBJ_get_vertex(obj, 3));
                test_tuples(&t2->n1, WAVEFRONTOBJ_get_normal(obj, 3));
                test_tuples(&t2->n2, WAVEFRONTOBJ_get_normal(obj, 1));
                test_tuples(&t2->n3, WAVEFRONTOBJ_get_normal(obj, 2));
                WAVEFRONTOBJ_delete(obj);
            }
    Catch(e) {
        printf("Exception: %s\n", EXCEPTIONS_strings[e]);
        TEST_FAIL_MESSAGE("Caught exception");
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ignore_unrecognized_lines);
    RUN_TEST(test_parse_vertex);
    RUN_TEST(test_parse_triangle_face);
    RUN_TEST(test_triangulating_polygons);
    RUN_TEST(test_parse_from_file);
    RUN_TEST(test_triangles_in_groups);
    RUN_TEST(test_parse_many_poly_teapot);
    RUN_TEST(test_vertex_normal_records);
    RUN_TEST(test_faces_with_normals);
    UNITY_END();
}
