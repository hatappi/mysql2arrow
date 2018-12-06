#include <mysql/mysql.h>

std::shared_ptr<arrow::RecordBatch> convert_arrow(MYSQL_RES *, int);
