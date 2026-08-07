#pragma once

#include "core/LosTree/LosTreeSitterNodeInfo/LosTreeSitterNodeInfo.h"
#include <QString>
#include <memory>

namespace LosCore
{
    /**
     * @brief
     */
    class LosTreeSitterDocument
    {
      public:
        // LosTreeSitterNodeInfo 就是 从 tree site 节点 到 自己组织的node
        using NodeVisitor = std::function<void(const LosTreeSitterNodeInfo &)>;


        explicit LosTreeSitterDocument();
        ~LosTreeSitterDocument();

        // 取消 构造函数
        LosTreeSitterDocument(const LosTreeSitterDocument &)            = delete;
        LosTreeSitterDocument &operator=(const LosTreeSitterDocument &) = delete;

      public: // tool get
        bool parse(const QString &text);
        // 增量解析
        void visitNodes(const NodeVisitor &visitor) const;
        bool update(const QString &text, int position, int chars_removed, int chars_added);
        bool hasSyntaxError() const;
        QString rootNodeType() const;
        bool isValid() const;

      private:
        class Impl;
        std::unique_ptr<Impl> L_impl;
    };
} // namespace LosCore
