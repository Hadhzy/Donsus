#include "../../Include/codegen.h"

void DonsusCodeGenerator::compile_donsus_expr(DonsusParser::end_result &ast) {
  compile_donsus_expr_helper(ast);
  // stack operation
};
void DonsusCodeGenerator::compile_donsus_expr_helper(
    DonsusParser::end_result &ast) {
  // setup the register
  // loop through the nodes

  for (const auto &node : ast->get_nodes()) {
    // fix this so the types are not stacked onto each other
    switch (node->type.type) {
    case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
      // make use of the stack pointer
    }
    }
  }
};

void DonsusCodegenEvaluator::compile(DonsusParser::end_result &ast) {
  // compile expression by passing it down to the DonsusCodegenerator
  // setup the stack as well so that we can push into it
  compile_donsus_expr(ast);
}
