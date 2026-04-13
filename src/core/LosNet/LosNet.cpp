
#include "LosNet.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosRouter/LosRouter.h"
#include <qfileinfo.h>
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qstringview.h>
#include <qurl.h>

namespace LosCore
{
    LosNet::LosNet(QObject *parent) : QObject{parent}
    {
        L_net = new QNetworkAccessManager(this);
        requestInit();
    }


    /*
     * - 发送 Plugin的请求
     */
    void LosNet::requestPlugin()
    {
        request(LosCommon::LosNet_Constants::API::PLUGIN_API,
                [this](const QByteArray &doc) { this->dealPluginReply(doc); });
    }


    /*
     * - 专门 处理 plugin 请求
     */
    void LosNet::dealPluginReply(const QByteArray &data)
    {
        QJsonParseError parseErr;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
        if (parseErr.error != QJsonParseError::NoError)
        {
            ERR("json 解析失败", "conn");
            return;
        }
        if (!doc.isArray())
        {
            ERR("json 格式错误!", "dealPluginReply");
            return;
        }
        QJsonArray arr(doc.array());
        QList<LosCommon::LosNet_Constants::PluginInfo> infos;
        for (int i = 0; i < arr.size(); i++)
        {
            QJsonObject obj{arr[i].toObject()};
            LosCommon::LosNet_Constants::PluginInfo info{.L_id          = obj["id"].toString(),
                                                         .L_name        = obj["name"].toString(),
                                                         .L_version     = obj["version"].toString(),
                                                         .L_downloadUrl = obj["download_url"].toString(),
                                                         .L_readmeUrl   = obj["readme_url"].toString()};
            infos.append(info);
        }
        emit LosRouter::instance()._cmd_net_pluginReply(infos);
    }



    /*
     * - 初始化 请求
     */
    void LosNet::requestInit()
    {
        request(LosCommon::LosNet_Constants::API::ROOT_API,
                [this](const QByteArray &doc) { this->dealInitReply(doc); });
    }
    void LosNet::dealInitReply(const QByteArray &data)
    {
        QString reply = QString::fromUtf8(data);
        SUC("init net -> :" + reply, "LosNet");
    }


    /*
     * - 下载插件
     */
    void LosNet::downloadPlugin(const QString &download_url, const QString &savePath)
    {
        /*
         * 直接 get 就行 不用 content
         */
        QUrl url(download_url);
        QNetworkRequest req(url);
        QNetworkReply *rep = L_net->get(req);

        conn(rep,
             [savePath](const QByteArray &data)
             {
                 QFileInfo info(savePath);
                 QDir dir = info.absoluteDir();
                 if (!dir.exists())
                 {
                     dir.mkpath(".");
                 }
                 QFile file(savePath);
                 if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                     return;
                 file.write(data);
                 file.close();
                 SUC("download plugin suc" + savePath, "LosNet");
                 emit LosCore::LosRouter::instance()._cmd_net_downloadFinished(savePath);
             });
    }



    /*
     * - 获取 readme 界面
     */
    void LosNet::fetchPluginReadme(const QString &url_str, std::function<void(const QString &)> func)
    {
        if (url_str.isEmpty())
        {
            func(R"(
                <p style='color:#888;'> No des ... </p>
                )");
            return;
        }
        QUrl url(url_str);
        QNetworkRequest req(url);
        QNetworkReply *rep = L_net->get(req);
        conn(rep, [func](const QByteArray &data) { func(QString::fromUtf8(data)); });
    }



    /*
     * - 发送 request
     */
    void LosNet::request(const QString &api, std::function<void(const QByteArray &)> func)
    {
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + api);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        QNetworkReply *rep = L_net->get(req);
        conn(rep, func);
    }



    /*
     * - 给 答复 进行连接
     */
    void LosNet::conn(QNetworkReply *reply, std::function<void(const QByteArray &)> func)
    {
        connect(reply, &QNetworkReply::finished, this,
                [=]()
                {
                    reply->deleteLater();
                    if (reply->error() != QNetworkReply::NoError)
                    {
                        ERR(reply->errorString(), "conn");
                        return;
                    }
                    QByteArray resData = reply->readAll();
                    func(resData);
                });
    }



    /*
     * - 初始化链接
     */
    void LosNet::initConnect() {}



} /* namespace LosCore */