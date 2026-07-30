// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosToolBarUi.h"
#include "common/constants/ConstantsNum/PerseusNum.h"
#include "common/constants/ConstantsStr/LosPanelManagerStr.h"
#include "core/LosRouter/LosRouter.h"
#include <QHBoxLayout>
#include <QSizePolicy>


namespace LosView
{
    /*
     * Construct
     * - 由 .ui 提升 (promote) 到这个类: <widget class="LosView::LosToolBarUi" .../>
     * - 构造函数里完成所有控件创建 + 布局 + 行为绑定
     */
    LosToolBarUi::LosToolBarUi(QWidget *parent) : QWidget(parent)
    {
        initStyle();
        initViewBtn();
        initFilesBtn();
    }
    LosToolBarUi::~LosToolBarUi() = default;



    /**
     * @brief initStyle
     * 创建并布局所有按钮
     * objectName 必须和 perseus_style.qss 里的 #xxx 选择器一致
     *
     * 尺寸策略:
     * - 不再使用 setFixedSize 锁死按钮宽高 (会导致字体放大后内容被截断)
     * - 改用 setMinimumSize + sizePolicy::Preferred, 让按钮根据字体大小自适应
     */
    void LosToolBarUi::initStyle()
    {
        auto *layout = new QHBoxLayout(this);
        layout->setSpacing(5);
        layout->setContentsMargins(3, 0, 0, 0);

        LOS_iconBtn = new QPushButton("P", this);
        LOS_iconBtn->setObjectName("icon_btn");
        LOS_iconBtn->setMinimumSize(36, 36);
        LOS_iconBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layout->addWidget(LOS_iconBtn);
        LOS_filesBtn = new LosView::LosDropdownButtonUi(this);
        LOS_filesBtn->setObjectName("files_btn");
        LOS_filesBtn->setMinimumWidth(80);
        LOS_filesBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        LOS_filesBtn->setText("File");
        layout->addWidget(LOS_filesBtn);
        LOS_viewBtn = new LosView::LosDropdownButtonUi(this);
        LOS_viewBtn->setObjectName("view_btn");
        LOS_viewBtn->setMinimumWidth(80);
        LOS_viewBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        LOS_viewBtn->setText("View");
        layout->addWidget(LOS_viewBtn);
        LOS_settingBtn = new QPushButton("Set", this);
        LOS_settingBtn->setObjectName("setting_btn");
        LOS_settingBtn->setMinimumWidth(80);
        LOS_settingBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layout->addWidget(LOS_settingBtn);
        LOS_runBtn = new QPushButton("Run", this);
        LOS_runBtn->setObjectName("run_singleFile_btn");
        LOS_runBtn->setMinimumWidth(80);
        LOS_runBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layout->addWidget(LOS_runBtn);
        LOS_projectBtn = new QRadioButton("CMake Pro?", this);
        LOS_projectBtn->setObjectName("project_btn");
        layout->addWidget(LOS_projectBtn);
        layout->addStretch();
        LOS_projectDirLabel = new QLabel("project dir", this);
        LOS_projectDirLabel->setObjectName("project_dir_label");
        layout->addWidget(LOS_projectDirLabel);

        /*
         * 行为绑定: 全部 emit 到 Router, 由 Perseus 监听
         * setting / run / project 是简单按钮, click → emit
         * files 的下拉项在 initFilesBtn() 里挂
         * view 的下拉项由 Perseus 注册 (因为它要操作 ui->bottom_tabwidget)
         */
        connect(LOS_settingBtn, &QPushButton::clicked, this, [] { emit LosCore::LosRouter::instance()._cmd_settingBtnClick(); });
        connect(LOS_runBtn, &QPushButton::clicked, this, [] { emit LosCore::LosRouter::instance()._cmd_runBtnClick(); });
        connect(LOS_projectBtn, &QRadioButton::toggled, this,
                [](bool checked) { emit LosCore::LosRouter::instance()._cmd_projectBtnToggled(checked); });
    }



    /**
     * @brief initFilesBtn
     */
    void LosToolBarUi::initFilesBtn()
    {
        LOS_filesBtn->addOption("choose a file", [] { emit LosCore::LosRouter::instance()._cmd_chooseFileBtnClick(); });
        LOS_filesBtn->addOption("choose a dir", [] { emit LosCore::LosRouter::instance()._cmd_chooseDirBtnClick(); });
        LOS_filesBtn->addSeparator();
        LOS_filesBtn->addOption("version",
                                [this]()
                                {
                                    QString versionInfo = QString("<h3>Perseus IDE</h3>"
                                                                  "<p><b>Version:</b> 1.106 (Build: %1)</p>"
                                                                  "<p><b>Qt version:</b> Qt %2</p>"
                                                                  "<hr>"
                                                                  "<p>Copyright &copy; 2026 LosAngelous</p>")
                                                              .arg(__DATE__)
                                                              .arg(QT_VERSION_STR);
                                    QMessageBox::about(this, tr("About Perseus"), versionInfo);
                                });
    }



    /**
     * @brief initViewBtn
     *
     */
    void LosToolBarUi::initViewBtn()
    {
        registerToggle("Output", LosCommon::Perseus_Constants::OUTPUT);
        registerToggle("Issues", LosCommon::Perseus_Constants::ISSUES);
        registerToggle("Terminal", LosCommon::Perseus_Constants::TERMINAL);
    }



    /**
     * @brief
     *
     * @param title
     * @param idx
     */
    void LosToolBarUi::registerToggle(const QString &title, int idx)
    {
        QAction *act = LOS_viewBtn->addOption(title, []() {});
        act->setCheckable(true);
        act->setChecked(true);
        connect(act, &QAction::toggled, this,
                [this, idx](bool visible) { emit LosCore::LosRouter::instance()._cmd_bottomTabVisibilityChanged(idx, visible); });
    }

} // namespace LosView
