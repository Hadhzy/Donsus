/*
Perform possible optimisations on the AST node before using llvm's IR
*/

#ifndef DONSUS_OPTIMISE_H
#define DONSUS_OPTIMISE_H
class OptimiseDonsus {
public:
  // all depth-search
  void loop();
  // https://compileroptimizations.com/category/constant_folding.htm
  virtual constant_folding() const;
  // https://compileroptimizations.com/category/constant_propagation.htm
  virtual constant_propagation() const;

private:
};

#endif