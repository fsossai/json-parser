#pragma once

namespace JSON {

enum Token {
  ARRAY_OPEN,
  ARRAY_CLOSE,
  OBJ_OPEN,
  OBJ_CLOSE,
  COMMA,
  COLON,
  STRING,
  INT,
  FLOAT,
  UNKNOWN,
  END
};

}