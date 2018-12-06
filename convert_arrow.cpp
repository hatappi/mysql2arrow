#include "ruby.h"
#include <arrow/api.h>
#include <iostream>
#include <mysql/mysql.h>
#include <mysql2_ext.h>
#include <mysql_arrow.h>
#include <arrow-glib/record-batch.h>
#include <rbgobject.h>


// res => Mysql2::Result
VALUE convert_arrow (VALUE self) {
  mysql2_result_wrapper * wrapper;
  Data_Get_Struct(self, mysql2_result_wrapper, wrapper);

  auto batch = convertArrow(wrapper->result, 3);

  auto record_batch = GARROW_RECORD_BATCH(g_object_new(GARROW_TYPE_RECORD_BATCH,
                                     "record-batch", &batch,
                                     NULL));
  return GOBJ2RVAL(record_batch);
}

extern "C" {
  void Init_convert_arrow() {
    rb_define_method(rb_const_get(rb_define_module("Mysql2"), rb_intern("Result")), "to_record_batch", (VALUE(*)(...))convert_arrow, 0);
  }
}
