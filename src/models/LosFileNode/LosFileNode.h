#pragma once
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QDir>
#include <QFuture>
#include <QFutureWatcher>
#include <QList>
#include <QString>
#include <QtConcurrent>
#include <memory>

namespace LosModel
{
class LosFileNode
{
  public:
    ~LosFileNode();

  public: // tool
    [[nodiscard]] static LosFileNode *create(const QString &filePath, LosFileNode *parent);

  public: // set
    void appendChild(LosFileNode *node);
    void clear();
    void setParentNode(LosFileNode *node);
    static void build(LosFileNode *root, const QString &cur, std::function<void()> on_finished);
    static void buildImpl(LosFileNode *, const QString &);

  public: // get
    int getChildCount() const;
    LosFileNode *getChild(int index) const;
    LosFileNode *getParent() const;
    int row() const;
    const LosModel::LosFilePath &getFile() const;
    LosCommon::LOS_ENUM_FileType getFileType() const;

  private:
    explicit LosFileNode(const QString &filePath, LosFileNode *parent = nullptr);

  private:
    LosFilePath LOS_filePath;
    QList<LosFileNode *> LOS_childrens;
    LosFileNode *LOS_parent = nullptr;
};
}; // namespace LosModel