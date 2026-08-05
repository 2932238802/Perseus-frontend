#include <tree_sitter/api.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

extern "C" const TSLanguage* tree_sitter_cpp();

int main()
{
    const std::string source = R"cpp(
#include <iostream>

namespace demo
{
    class Person
    {
    public:
        void sayHello()
        {
            std::cout << "Hello";
        }
    };

    int add(int a, int b)
    {
        if (a > 0)
        {
            return a + b;
        }

        return b;
    }
}
)cpp";

    /*
     * 1. 创建解析器
     */
    TSParser* parser = ts_parser_new();

    if (parser == nullptr)
    {
        std::fprintf(stderr, "Failed to create parser\n");
        return 1;
    }

    /*
     * 2. 设置 C++ 语言
     */
    const TSLanguage* language = tree_sitter_cpp();

    if (language == nullptr)
    {
        std::fprintf(stderr, "Failed to get C++ language\n");
        ts_parser_delete(parser);
        return 1;
    }

    if (!ts_parser_set_language(parser, language))
    {
        std::fprintf(stderr, "Failed to set C++ language\n");
        ts_parser_delete(parser);
        return 1;
    }

    /*
     * 3. 解析源代码
     *
     * 这里使用 UTF-8。
     * source.data() 是普通 char*。
     */
    TSTree* tree = ts_parser_parse_string(
        parser,
        nullptr,
        source.data(),
        static_cast<uint32_t>(source.size())
    );

    if (tree == nullptr)
    {
        std::fprintf(stderr, "Failed to parse source\n");
        ts_parser_delete(parser);
        return 1;
    }

    /*
     * 4. 获取根节点
     */
    const TSNode root = ts_tree_root_node(tree);

    /*
     * 5. 打印整棵语法树
     */
    char* treeString = ts_node_string(root);

    if (treeString != nullptr)
    {
        std::printf("%s\n", treeString);
        std::free(treeString);
    }

    /*
     * 6. 检查语法错误
     */
    if (ts_node_has_error(root))
    {
        std::printf("The syntax tree contains errors\n");
    }
    else
    {
        std::printf("The syntax tree has no errors\n");
    }

    /*
     * 7. 释放资源
     */
    ts_tree_delete(tree);
    ts_parser_delete(parser);

    return 0;
}
