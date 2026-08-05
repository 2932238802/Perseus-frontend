#include "LosTreeSitterDocument.h"
#include "core/LosLog/LosLog.h"

#include <qglobal.h>
#include <tree-sitter-cpp.h>
#include <tree_sitter/api.h>

// 匿名 工具 函数
namespace
{
    /**
     * @brief 根据 文本 位置 获得 对应的 TSPoint
     *
     * @param text
     * @param position
     * @return TSPoint
     */
    TSPoint AtPoint(const QString &text, int position)
    {
        // 修正位置
        int positionModify = qBound(0, position, text.size());
        int row            = 0;
        int rowBeginIndex  = 0;
        for (int i = 0; i < positionModify; i++)
        {
            if (text.at(i) == '\n')
            {
                row++;
                rowBeginIndex = i + 1;
            }
        }
        TSPoint nodePoint;
        const QByteArray data = text.mid(rowBeginIndex, positionModify - rowBeginIndex).toUtf8();
        nodePoint.column      = static_cast<uint32_t>(data.size());
        nodePoint.row         = static_cast<uint32_t>(row);
        return nodePoint;
    }



    /**
     * @brief 获得 当前 位置 之前的 字节个数
     *
     * @param text
     * @param position
     * @return uint32_t
     */
    uint32_t ByteOffsetAt(const QString &text, int position)
    {
        const int positionModify = qBound(0, position, text.size());
        return static_cast<uint32_t>(text.left(positionModify).toUtf8().size());
    }

} // namespace


namespace LosCore
{
    // PIMPL
    class LosTreeSitterDocument::Impl
    {
      public:
        TSParser *L_parser = nullptr;
        TSTree *L_tree     = nullptr;
        QString L_text;
        bool L_hasSyntaxError = false;
    };



    /**
     * @brief Construct a new Los Tree Sitter Document:: Los Tree Sitter Document object
     */
    LosTreeSitterDocument::LosTreeSitterDocument() : L_impl(std::make_unique<Impl>())
    {
        // 创建一个全新的解析器
        L_impl->L_parser = ts_parser_new();
        if (L_impl->L_parser == nullptr)
        {
            WAR("解析器 创建失败~", "LosTreeSitterDocument::LosTreeSitterDocument");
            return;
        }
        // 设置语言
        // tree_sitter_cpp const TSLanguage*
        bool languageSet = ts_parser_set_language(L_impl->L_parser, tree_sitter_cpp());
        if (!languageSet)
        {
            ERR("设置 C++ grammar 失败", "LosTreeSitterDocument::LosTreeSitterDocument");
            ts_parser_delete(L_impl->L_parser);
            L_impl->L_parser = nullptr;
        }
    }



    /**
     * @brief Destroy the Los Tree Sitter Document:: Los Tree Sitter Document object
     * 析构函数
     */
    LosTreeSitterDocument::~LosTreeSitterDocument()
    {
        if (L_impl == nullptr)
        {
            return;
        }
        if (L_impl->L_tree != nullptr)
        {
            ts_tree_delete(L_impl->L_tree);
            L_impl->L_tree = nullptr;
        }
        if (L_impl->L_parser != nullptr)
        {
            ts_parser_delete(L_impl->L_parser);
            L_impl->L_parser = nullptr;
        }
    }



    /**
     * @brief
     *
     * @param text
     * @return true
     * @return false
     */
    bool LosTreeSitterDocument::parse(const QString &text)
    {
        if (!isValid())
        {
            ERR("解析失败 当前解析器或者内部服务出现问题", "LosTreeSitterDocument::parse");
            return false;
        }
        const QByteArray source = text.toUtf8();
        TSTree *newTree         = ts_parser_parse_string(L_impl->L_parser, nullptr, source.constData(), static_cast<uint32_t>(source.size()));
        if (newTree == nullptr)
        {
            return false;
        }
        if (L_impl->L_tree != nullptr)
        {
            ts_tree_delete(L_impl->L_tree);
        }
        L_impl->L_tree           = newTree;
        L_impl->L_text           = text;
        const TSNode root        = ts_tree_root_node(L_impl->L_tree);
        L_impl->L_hasSyntaxError = ts_node_has_error(root);
        return true;
    }



    /**
     * @brief 重建树
     *
     * @param text
     * @param position
     * @param chars_removed
     * @param chars_added
     * @return true
     * @return false
     */
    bool LosTreeSitterDocument::update(const QString &text, int position, int charsRemoved, int charsAdded)
    {
        if (!isValid())
        {
            ERR("更新失败 Tree-sitter parser 无效", "LosTreeSitterDocument::update");
            return false;
        }
        if (L_impl->L_tree == nullptr)
        {
            return parse(text);
        }
        if (position < 0 || charsRemoved < 0 || charsAdded < 0)
        {
            ERR("更新失败：编辑参数小于零", "LosTreeSitterDocument::update");
            return false;
        }
        if (position > L_impl->L_text.size() || position + charsRemoved > L_impl->L_text.size())
        {
            ERR("更新失败：编辑范围超出旧文本", "LosTreeSitterDocument::update");
            return false;
        }
        const QString oldText = L_impl->L_text;
        const int oldEndPosition = position + charsRemoved;
        const int newEndPosition = position + charsAdded;
        TSInputEdit edit{};
        edit.start_byte    = ByteOffsetAt(oldText, position);
        edit.old_end_byte  = ByteOffsetAt(oldText, oldEndPosition);
        edit.new_end_byte  = ByteOffsetAt(text, newEndPosition);
        edit.start_point   = AtPoint(oldText, position);
        edit.old_end_point = AtPoint(oldText, oldEndPosition);
        edit.new_end_point = AtPoint(text, newEndPosition);
        ts_tree_edit(L_impl->L_tree, &edit);
        const QByteArray source = text.toUtf8();
        TSTree *newTree         = ts_parser_parse_string(L_impl->L_parser, L_impl->L_tree, source.constData(), static_cast<uint32_t>(source.size()));
        if (newTree == nullptr)
        {
            ERR("Tree-sitter 增量解析失败", "LosTreeSitterDocument::update");
            return false;
        }
        ts_tree_delete(L_impl->L_tree);
        L_impl->L_tree = newTree;
        L_impl->L_text = text;
        const TSNode root = ts_tree_root_node(L_impl->L_tree);
        L_impl->L_hasSyntaxError = ts_node_has_error(root);
        return true;
    }



    /**
     * @brief hasSyntaxError
     *
     * @return true
     * @return false
     */
    bool LosTreeSitterDocument::hasSyntaxError() const
    {
        return L_impl != nullptr && L_impl->L_hasSyntaxError;
    }



    /**
     * @brief rootNodeType
     *
     * @return QString
     */
    QString LosTreeSitterDocument::rootNodeType() const
    {
        if (L_impl == nullptr || L_impl->L_tree == nullptr)
        {
            return {};
        }
        const TSNode root = ts_tree_root_node(L_impl->L_tree);
        // const char *ts_node_type(TSNode self);
        // 一个 TSNode 结构体
        // 输出 一个 C 字符串（const char*），描述该节点的语法类型
        return QString::fromUtf8(ts_node_type(root));
    }



    /**
     * @brief 返回是不是 有效的
     *
     * @return true
     * @return false
     */
    bool LosTreeSitterDocument::isValid() const
    {
        return L_impl != nullptr && L_impl->L_parser != nullptr;
    }
} // namespace LosCore
