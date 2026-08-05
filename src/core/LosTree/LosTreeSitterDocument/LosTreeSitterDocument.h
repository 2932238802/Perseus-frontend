#pragma once

#include <QString>

#include <memory>

namespace LosCore
{
    class LosTreeSitterDocument
    {
      public:
        explicit LosTreeSitterDocument();
        ~LosTreeSitterDocument();

        // 取消 构造函数
        LosTreeSitterDocument(const LosTreeSitterDocument &)            = delete;
        LosTreeSitterDocument &operator=(const LosTreeSitterDocument &) = delete;

      public: // tool get
        bool parse(const QString &text);
        // 增量解析
        bool update(const QString &text, int position, int chars_removed, int chars_added);
        bool hasSyntaxError() const;
        QString rootNodeType() const;
        bool isValid() const;

      private:
        class Impl;
        std::unique_ptr<Impl> L_impl;
    };
} // namespace LosCore
