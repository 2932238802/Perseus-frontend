
#include "LosNet.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosRouter/LosRouter.h"
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qstringview.h>

namespace LosCore
{
    LosNet::LosNet(QObject *parent) : QObject{parent}
    {
        L_net = new QNetworkAccessManager(this);
    }


    void LosNet::requestPlugin()
    {
        request(LosCommon::LosNet_Constants::API::PLUGIN_API,
                [this](const QJsonDocument &doc) { this->dealPluginReply(doc); });
    }



    /**
    - 发送 request
    */
    void LosNet::request(const QString &api, std::function<void(const QJsonDocument &)> func)
    {
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + api);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        QNetworkReply *rep = L_net->get(req);
        conn(rep, func);
    }



    /**
     - 给 答复 进行连接
    */
    void LosNet::conn(QNetworkReply *reply, std::function<void(const QJsonDocument &)> func)
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
                    QJsonParseError parseErr;
                    QJsonDocument doc = QJsonDocument::fromJson(resData, &parseErr);

                    if (parseErr.error != QJsonParseError::NoError)
                    {
                        ERR("json 解析失败", "conn");
                        return;
                    }
                    func(doc);
                });
    }



    /**
    - 初始化链接
    */
    void LosNet::initConnect() {}



    /**
    - 专门 处理 plugin 请求
    */
    void LosNet::dealPluginReply(const QJsonDocument &doc)
    {
        if (!doc.isArray())
        {
            ERR("json 格式错误!", "dealPluginReply");
        }
        QJsonArray arr(doc.array());
        QList<LosCommon::LosNet_Constants::PluginInfo> infos;
        for (int i = 0; i < arr.size(); i++)
        {
            QJsonObject obj{arr[i].toObject()};
            LosCommon::LosNet_Constants::PluginInfo info{
                .L_id = obj["id"].toString(), .L_name = obj["name"].toString(), .L_version = obj["version"].toString()};
            infos.append(info);
        }
        emit LosRouter::instance()._cmd_net_pluginReply(infos);
    }
} // namespace LosCore