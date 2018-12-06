#include "ruby.h"
#include <arrow/api.h>
#include <iostream>
#include <mysql/mysql.h>
#include <mysql2_ext.h>
#include <mysql_arrow.h>
#include <arrow-glib/record-batch.h>

// res => Mysql2::Result
GArrowRecordBatch * convert_arrow (VALUE self, VALUE res) {
  mysql2_result_wrapper * wrapper;
  Data_Get_Struct(res, mysql2_result_wrapper, wrapper);

  auto batch = convertArrow(wrapper->result, 3);

  auto record_batch = GARROW_RECORD_BATCH(g_object_new(GARROW_TYPE_RECORD_BATCH,
                                     "record-batch", &batch,
                                     NULL));
  return record_batch;

}

extern "C" {
  void Init_convert_arrow() {
    rb_define_global_function ("convert_arrow", (VALUE(*)(...))convert_arrow, 1);
  }
}
