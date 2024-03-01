#include "parser.h"

class DonsusCodeGenerator {
public:
  void compile_donsus_expr(DonsusParser::end_result &ast);
  void compile_donsus_expr_helper(DonsusParser::end_result &ast);
};

class DonsusCodegenEvaluator : public DonsusCodeGenerator {
public:
  void compile(DonsusParser::end_result &ast);
};