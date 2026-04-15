#include "LosSettingsUi.h"
#include "ui_LosSettingsUi.h"

namespace LosView
{
    /*
     * - construct
     */
    LosSettingsUi::LosSettingsUi(QWidget *parent) : QDialog(parent), ui(new Ui::LosSettingsUi)
    {
        ui->setupUi(this);
        initStyle();
        initConnect();
    }
    LosSettingsUi::~LosSettingsUi()
    {
        delete ui;
    }



    /*
     * 设置样式
     * - windowFlags
     *   - 获取当前 窗口已经设置的所有样式
     * - WindowContextHelpButtonHint
     *   - 右上角的 问号
     *   - ~ 就是按位取反
     */
    void LosSettingsUi::initStyle()
    {
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    }



    /*
     * - 初始化 连接
     */
    void LosSettingsUi::initConnect()
    {
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() { accept(); });
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [this]() { reject(); });
        QPushButton *applyBtn = ui->buttonBox->button(QDialogButtonBox::Apply);
        if (applyBtn)
        {
            connect(applyBtn, &QPushButton::clicked, this, &LosSettingsUi::onSaveSettings);
        }
    }



    /*
     * - 保存 设置
     */
    void LosSettingsUi::onSaveSettings()
    {
        INF("保存", "LosSettingsUi");
    }

} /* namespace LosView */