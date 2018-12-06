#include "ruby.h"
#include <arrow/api.h>
#include <iostream>
#include <mysql/mysql.h>
#include <mysql2_ext.h>
#include <mysql_arrow.h>
#include <arrow-glib/record-batch.h>
#include <rbgobject.h>


// res => Mysql2::Result
void convert_arrow (VALUE self) {
  mysql2_result_wrapper * wrapper;
  Data_Get_Struct(self, mysql2_result_wrapper, wrapper);

  std::shared_ptr<arrow::RecordBatch> batch;
  while(1) {
    batch = convertArrow(wrapper->result, 2);
    if (batch->num_rows() == 0) {
      break;
    }
    auto record_batch = GARROW_RECORD_BATCH(g_object_new(GARROW_TYPE_RECORD_BATCH,
                                     "record-batch", &batch,
                                     NULL));
    rb_yield(GOBJ2RVAL(record_batch));
  }

  mysql_data_seek(wrapper->result, 0);
}

extern "C" {
  void Init_convert_arrow() {
    rb_define_method(rb_const_get(rb_define_module("Mysql2"), rb_intern("Result")), "to_record_batch", (VALUE(*)(...))convert_arrow, 0);
  }
}
