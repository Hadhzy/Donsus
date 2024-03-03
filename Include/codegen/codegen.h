#include "../parser.h"

class DonsusCodeGenerator {
  // destination driven code generating system
public:
  void compile_donsus_expr(DonsusParser::end_result &ast);
};

class DonsusCodegenEvaluator : public DonsusCodeGenerator {
public:
  void compile(DonsusParser::end_result &ast);
};