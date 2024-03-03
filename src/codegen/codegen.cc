#include "../../Include/codegen/codegen.h"

void DonsusCodeGenerator::compile_donsus_expr(DonsusParser::end_result &ast) {

  for (const auto &node : ast->get_nodes()) {
    // fix this so the types are not stacked onto each other
    switch (node->type.type) {
    case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
      // add codegeneration for var decl using the library
    }
    }
  }
};

void DonsusCodegenEvaluator::compile(DonsusParser::end_result &ast) {
  // compile expression by passing it down to the DonsusCodegenerator

  compile_donsus_expr(ast);
}
