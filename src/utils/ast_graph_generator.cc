#include <iostream>
#include <memory>
#include "../../Include/parser.h"
#include <fstream>


// Used to generate the dot file that will produce the AST in a graph form

class ASTVisualiserGraph {
    public: 
        ASTVisualiserGraph(): dotFileContent("") {}

        void donsus_generateGraph(std::unique_ptr<donsus_ast> ast) {
            // Reset the DOT file content
            dotFileContent.clear();

            // Start building the DOT file content
            dotFileContent += "digraph AST {\n";

            donsus_traverseAST(ast.get());

            dotFileContent += "}\n";
            
            donsus_writeToFile();


            int result = std::system("dot -Tsvg ast.dot");
        }

    private:
        void donsus_traverseAST(donsus_ast* node) {
        // Check if the node is a leaf (operand)
        if (!node->left && !node->right) {
            dotFileContent += "    " + (node->value.value) + ";\n";
        } else {
            dotFileContent += "    " + (de_get_name_from_token(node->value.kind)) + " ->";

            donsus_traverseAST(node->left.get());

            dotFileContent += "    " + (de_get_name_from_token(node->value.kind)) + " ->";

            donsus_traverseAST(node->right.get());
        }
    }

    void donsus_writeToFile() {
        // Open the DOT file for writing
        std::ofstream outFile("ast.dot");
        if (outFile.is_open()) {
            // Write the DOT file content to the file
            outFile << dotFileContent;
            outFile.close();
            std::cout << "DOT file generated: " << "SVG format of the AST can be accessed here: " << std::endl;
        } else {
            std::cerr << "Error: Unable to open DOT file for writing." << std::endl;
        }
    }
    
    std::string dotFileContent;
};