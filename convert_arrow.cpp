#include "ruby.h"
#include <arrow/api.h>
#include <iostream>
#include <mysql/mysql.h>
#include <mysql2_ext.h>
#include <mysql_arrow.h>
#include <arrow-glib/record-batch.h>
#include <rbgobject.h>


// res => Mysql2::Result
VALUE convert_arrow (int argc, VALUE * argv, VALUE self) {
  VALUE opts, block;

  auto size = 50;
  if (rb_scan_args(argc, argv, "01&", &opts, &block) > 0) {
    auto v = rb_hash_aref(opts, ID2SYM(rb_intern("size")));
    if (RTEST(v)) {
      size = NUM2INT(v);
    }
  }

  mysql2_result_wrapper * wrapper;
  Data_Get_Struct(self, mysql2_result_wrapper, wrapper);
  mysql_data_seek(wrapper->result, 0);

  std::shared_ptr<arrow::RecordBatch> batch;
  while(1) {
    batch = convertArrow(wrapper->result, size);
    if (batch->num_rows() == 0) {
      break;
    }
    auto record_batch = GARROW_RECORD_BATCH(g_object_new(GARROW_TYPE_RECORD_BATCH,
                                     "record-batch", &batch,
                                     NULL));
    rb_yield(GOBJ2RVAL(record_batch));
  }

  return Qnil;
}

extern "C" {
  void Init_mysql2_arrow() {
    rb_define_method(rb_const_get(rb_define_module("Mysql2"), rb_intern("Result")), "each_record_batch", (VALUE(*)(...))convert_arrow, -1);
  }
}
