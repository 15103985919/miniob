/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/date_type.h"
#include "common/value.h"
#include "storage/common/column.h"

int DateType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::DATE, "left type is not date");
  ASSERT(right.attr_type() == AttrType::DATE, "right type is not date");
  return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);

}

int DateType::compare(const Column &left, const Column &right, int left_idx, int right_idx) const
{
  ASSERT(left.attr_type() == AttrType::DATE, "left type is not date");
  ASSERT(right.attr_type() == AttrType::DATE, "right type is not date");
  return common::compare_int((void *)&((int*)left.data())[left_idx],
      (void *)&((int*)right.data())[right_idx]);
}



RC DateType::set_value_from_str(Value &val, const string &data) const
{
  RC                rc = RC::SUCCESS;
  int year,month,day;
  bool is_valid = Value::str_to_date(data.c_str(),data.length(),year,month,day);

  int tmp=year*10000+month*100+day;
  if(is_valid){
    val.set_date(tmp);
  }else{
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  return rc;
}

RC DateType::to_string(const Value &val, string &result) const
{
  int tmp=val.value_.int_value_;
  int year=tmp/10000;
  int month=(tmp%10000)/100;
  int day=tmp%100;
  stringstream ss;
  ss << year <<"-"<<month<<"-"<<day;
  result = ss.str();
  return RC::SUCCESS;
}