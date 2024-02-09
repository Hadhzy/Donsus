#ifndef DONSUS_AST_VISUALISER_GRAPH_H
#define DONSUS_AST_VISUALISER_GRAPH_H
#include <iostream>
#include <memory>
#include "../../Include/parser.h"
#include <fstream>

class ASTVisualiserGraph {
public:
    ASTVisualiserGraph();

    void donsus_generateGraph(donsus_ast* ast);

private:
    void donsus_traverseAST(donsus_ast* node);

    void donsus_writeToFile();

    std::string dotFileContent;
};

#endif // DONSUS_AST_VISUALISER_GRAPH_H
