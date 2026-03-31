
#include "LosEditorTabUi.h"
#include "common/constants/ConstantsClass.h"
#include "common/util/CheckLang.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"


namespace LosView
{

LosEditorTabUi::LosEditorTabUi(QTabWidget *tab_widget, QWidget *parent) : L_tabWidget{tab_widget}, QWidget{parent}
{
    initConnect();
}

LosEditorTabUi::~LosEditorTabUi() {}

/**
关闭标签页
*/
void LosEditorTabUi::closeTab(int index) {}



/**
- 保存标签页
*/
void LosEditorTabUi::saveTab()
{
    if (L_tabWidget == nullptr)
    {
        return;
    }
    auto widget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
    if (!widget || !widget->isDirty())
    {
        return;
    }
    if (!widget->save())
    {
        ERR("failed to save file", "LosEditorTabUi");
        return;
    }
}



/**
保存所有的标签页
*/
void LosEditorTabUi::saveAllTabs()
{
    if (nullptr == L_tabWidget)
    {
        ERR("error in saveTab: nullptr", "LosEditorTabUi");
        return;
    }
    for (int i = 0; i < L_tabWidget->count(); i++)
    {
        auto a = qobject_cast<LosEditorUi *>(L_tabWidget->widget(i));
        if (!a || !a->isDirty())
        {
            continue;
        }
        if (!a->save())
        {
            WAR("error in saveAllTabs: save file [" + LOS_pathToUi.key(a) + "] failed!", "LosEditorTabUi")
        }
    }
}



/**
- tool
- 打开文件
- 只能打开 非 二进制文件
*/
void LosEditorTabUi::openFile(const LosModel::LosFilePath &file)
{
    // 呼叫 解释器 呼叫 运行其
    if (file.isBinary())
    {
        return;
    }
    auto filePath = file.getFilePath();
    checkLspAnsFormat(filePath);

    if (LOS_pathToUi.contains(filePath))
    {
        LosEditorUi *editor = LOS_pathToUi.value(filePath);
        L_tabWidget->setCurrentWidget(editor);
        return;
    }
    LosEditorUi *editor = new LosEditorUi(this);
    auto contextCopy    = QSharedPointer<LosModel::LosFileContext>::create();
    contextCopy->load(filePath);
    auto fileCopy = QSharedPointer<LosModel::LosFilePath>::create(filePath);
    editor->loadContextAndPath(contextCopy, fileCopy);
    L_tabWidget->addTab(editor, QFileInfo(filePath).fileName());
    LOS_pathToUi.insert(filePath, editor);
    L_tabWidget->setCurrentWidget(editor);
}



void LosEditorTabUi::formatTab()
{
    if (getCurEditor())
        getCurEditor()->format();
}



/**
get
获取当前编辑器
*/
LosEditorUi *LosEditorTabUi::getCurEditor()
{
    if (L_tabWidget != nullptr)
    {
        return qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
    }
    return nullptr;
}



/**
get
*/
int LosEditorTabUi::tabCount() const
{
    return L_tabWidget->count();
}



/**
get
*/
QString LosEditorTabUi::getCurFilePath() const
{
    auto widget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
    return widget != nullptr ? LOS_pathToUi.key(widget) : "";
}



QStringList LosEditorTabUi::getOpenFiles() const
{
    return LOS_pathToUi.keys();
}


/**
关闭 ui 点击
*/
void LosEditorTabUi::onTabCloseRequested(int index)
{
    QWidget *wi = L_tabWidget->widget(index);
    if (!wi)
        return;
    LosEditorUi *editor = qobject_cast<LosEditorUi *>(wi);

    // 补充
    if (editor->isDirty())
    {
        QString fileName = L_tabWidget->tabText(index);
        fileName.replace(" *", "");
        QMessageBox::StandardButton res =
            QMessageBox::warning(this, "save tips", QString("file '%1' has been modified. save changes?").arg(fileName),
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if (res == QMessageBox::Save)
        {
            if (!editor->save())
            {
                ERR(QStringLiteral("error") + "save failed, unable to close the tab!", "LosEditorTabUi");
                return;
            }
        }
        else if (res == QMessageBox::Cancel)
        {
            return;
        }
    }
    QString filePath = LOS_pathToUi.key(editor);
    if (!filePath.isEmpty())
        LOS_pathToUi.remove(filePath);
    L_tabWidget->removeTab(index);
    editor->deleteLater();
}

/**
如果编辑器 修改
*/
void LosEditorTabUi::onEditDirty(const QString &file_path, bool is_dirty)
{
    if (nullptr == LOS_pathToUi[file_path])
        return;

    int index = L_tabWidget->indexOf(LOS_pathToUi[file_path]);
    if (index == -1)
        return;
    QString currentTitle = L_tabWidget->tabText(index);


    if (is_dirty && !currentTitle.endsWith("*"))
    {
        L_tabWidget->setTabText(index, currentTitle + " *");
    }
    else if (!is_dirty && currentTitle.endsWith("*"))
    {
        L_tabWidget->setTabText(index, currentTitle.left(currentTitle.length() - 2));
    }
}



/**
 */
void LosEditorTabUi::onDefineResult(const QString &file_path, int line)
{
    openFile(file_path);
    if (nullptr != getCurEditor())
    {
        getCurEditor()->gotoLine(line);
    }
}



/**
双击
*/
void LosEditorTabUi::onDoubleClickedOnIssue(const QString &file_path, int line)
{
    openFile(file_path);
    auto editor = getCurEditor();
    if (editor)
    {
        editor->gotoLine(line);
    }
}



/**
- 重新 检查
*/
void LosEditorTabUi::onResetCheck(LosCommon::LosToolChain_Constants::LosLanguage lan, const QString &curFile)
{
    if (L_checkedLanguage.contains(lan))
    {
        L_checkedLanguage.remove(lan);
        if (!curFile.isEmpty())
        {
            checkLspAnsFormat(curFile);
        }
    }
}



/**
初始化
*/
void LosEditorTabUi::initConnect()
{
    connect(L_tabWidget, &QTabWidget::tabCloseRequested, this, &LosEditorTabUi::onTabCloseRequested);
    // 收到 定义 结果 就去处理
    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_lsp_result_definition, this,
            &LosEditorTabUi::onDefineResult);

    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_gotoFile, this,
            &LosEditorTabUi::onDoubleClickedOnIssue);

    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_codeFormat, this, &LosEditorTabUi::formatTab);

    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_fileDirty, this, &LosEditorTabUi::onEditDirty);
}



/**
检查 语法 和 自行
*/
void LosEditorTabUi::checkLspAnsFormat(const QString &file_path)
{
    auto lang = LosCommon::CheckLang(file_path);
    if (L_checkedLanguage.contains(lang))
        return;

    switch (lang)
    {
    case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
    {
        emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
            lang, LosCommon::LosToolChain_Constants::LosTool::CLANGD);
        emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
            lang, LosCommon::LosToolChain_Constants::LosTool::CLANG_FORMAT);
        L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::CXX);
        return;
    }
    default:
        break;
    }

    if (LosModel::LosFilePath(file_path).getFileName() == "CMakeLists.txt")
    {
        emit LosCore::LosRouter::instance()._cmd_checkSingleTool(
            LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP);
    }
}


} // namespace LosView
