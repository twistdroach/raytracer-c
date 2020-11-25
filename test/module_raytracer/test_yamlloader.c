#include <unity.h>
#include "../testutils.h"

#include <yamlloader.h>

#include <tuples.h>

void setUp() {}
void tearDown() {}


//void YAMLLOADER_parse_map_entries(char *data, void (*process_entry)(char *key, char *value, void *context), void *context);
static int parse_map_entries;
void validate_key_value(char *key, char *value, void *context) {
  UNUSED(context);
  TEST_ASSERT_EQUAL_STRING("akey", key);
  TEST_ASSERT_EQUAL_STRING("[the value]", value);
  parse_map_entries++;
}

void test_parse_map_entries() {
  parse_map_entries = 0;
  char data[] = "\nakey:[the value]\n\n";
  YAMLLOADER_parse_map_entries(data, validate_key_value, NULL);
  TEST_ASSERT_EQUAL(1, parse_map_entries);
}

void test_parse_color() {
  TUPLES_Color result, expected;
  TUPLES_init_color(&expected, 100.5, 22.75, 4.123);
  char data[] = "[100.5, 22.75,    4.123]";
  YAMLLOADER_parse_color(data, &result);
  test_tuples(&expected, &result);
}

void test_parse_vector() {
  TUPLES_Vector result, expected;
  TUPLES_init_vector(&expected, 1.5, 2.75, 4);
  char data[] = "[1.5, 2.75,4]";
  YAMLLOADER_parse_vector(data, &result);
  test_tuples(&expected, &result);
}

void test_parse_point() {
  TUPLES_Point result, expected;
  TUPLES_init_point(&expected, .5, 7.75, 0.4);
  char data[] = "[.5, 7.75, 0.4]";
  YAMLLOADER_parse_point(data, &result);
  test_tuples(&expected, &result);
}

void test_parse_bool() {
  bool result = false;
  YAMLLOADER_parse_bool("true", &result);
  TEST_ASSERT_TRUE(result);
  YAMLLOADER_parse_bool("false", &result);
  TEST_ASSERT_FALSE(result);
}

void test_parse_uint() {
  unsigned int result = 0;
  YAMLLOADER_parse_uint("5", &result);
  TEST_ASSERT_EQUAL_UINT(5, result);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_parse_bool);
  RUN_TEST(test_parse_uint);
  RUN_TEST(test_parse_point);
  RUN_TEST(test_parse_vector);
  RUN_TEST(test_parse_color);
  RUN_TEST(test_parse_map_entries);
  return UNITY_END();
}
