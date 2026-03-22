#pragma once
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QDir>
#include <QList>
#include <QString>
#include <memory>

namespace LosModel {
class LosFileNode {
public:
  ~LosFileNode();

public: // tool
  [[nodiscard]] static LosFileNode* create(const QString &filePath, LosFileNode *parent);

public: // set
  void appendChild(LosFileNode *node);
  void clear();
  void setParentNode(LosFileNode *node);
  static void build(LosFileNode *root, const QString &cur);

public: // get
  int getChildCount() const;
  LosFileNode *getChild(int index) const;
  LosFileNode *getParent() const;
  int row() const;
  const QString& getFileName() const;
  const QString& getFilePath() const;
  LosCommon::LOS_ENUM_FileType getFileType() const;

private:
  explicit LosFileNode(const QString &filePath, LosFileNode *parent = nullptr);

private:
  LosFilePath LOS_filePath;
  QList<LosFileNode *> LOS_childrens;
  LosFileNode *LOS_parent = nullptr;
};
}; // namespace LosModel