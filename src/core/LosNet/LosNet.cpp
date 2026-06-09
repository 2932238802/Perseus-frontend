// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosNet.h"
#include "common/constants/ConstantsClass/LosNetClass.h"
#include "common/constants/ConstantsStr/LosNetStr.h"
#include "common/constants/ConstantsStr/LosStateStr.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include <QDir>
#include <QList>
#include <QNetworkAccessManager>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qstringview.h>
#include <qurl.h>


namespace LosCore
{
    LosNet::LosNet(QObject *parent) : QObject{parent}
    {
        L_net = new QNetworkAccessManager(this);
        initConnect();
        requestInit();
    }


    /**
     * @brief requestPlugin
     */
    void LosNet::requestPlugin()
    {
        requestGet(LosCommon::LosNet_Constants::API::PLUGIN_API, [this](const QByteArray &doc) { this->dealPluginReply(doc); });
    }


    /**
     * @brief requestRegister
     * - 拼 {"username","password"} JSON, POST 到注册接口
     */
    void LosNet::requestRegister(const QString &username, const QString &password)
    {
        QJsonObject body;
        body["username"] = username;
        body["password"] = password;
        LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_USERNAME, username);
        QByteArray data = QJsonDocument(body).toJson(QJsonDocument::Compact);
        requestPost(LosCommon::LosNet_Constants::API::REGISTER_API, data, [this](const QByteArray &doc) { this->dealRegisterReply(doc); });
    }



    /**
     * @brief requestLogin
     * - 拼 {"username","password"} JSON, POST 到登录接口
     */
    void LosNet::requestLogin(const QString &username, const QString &password)
    {
        QJsonObject body;
        body["username"] = username;
        body["password"] = password;
        LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_USERNAME, username);
        QByteArray data = QJsonDocument(body).toJson(QJsonDocument::Compact);
        requestPost(LosCommon::LosNet_Constants::API::LOGIN_API, data, [this](const QByteArray &doc) { this->dealLoginReply(doc); });
    }



    /**
     * @brief
     * @param token
     */
    void LosNet::requestAutoLogin(const QString &token)
    {
        QString authHeader = "Bearer " + token;
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AUTOLOGIN_API));
        QNetworkRequest req(url);
        req.setRawHeader("Authorization", authHeader.toUtf8());
        QNetworkReply *rep = L_net->post(req, QByteArray());
        conn(rep, [this](const QByteArray &doc) { this->dealAutoLoginReply(doc); });
    }



    /**
     * @brief Agent 发送信息
     *
     * @param msg
     */
    void LosNet::requestAgentChat(const QString &msg)
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QJsonObject body;
        body["message"]    = msg;
        body["agent_name"] = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AGENT_CUR_NAME);
        QByteArray data    = QJsonDocument(body).toJson(QJsonDocument::Compact);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_CHAT_API));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, data);
        conn(rep, [this](const QByteArray &doc) { this->dealAgentChatReply(doc); });
    }



    /**
     * @brief dealPluginReply
     * @param data
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



    /**
     * @brief
     * "success": true, "message": "注册成功"
     * @param data
     */
    void LosNet::dealRegisterReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_auth_response(false, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        bool suc       = obj["success"].toBool();
        QString msgMsg = obj["message"].toString();
        if (suc)
        {
            QString token = obj["token"].toString();
            LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN, token);
        }
        emit LosRouter::instance()._cmd_auth_response(suc, msgMsg);
    }



    /**
     * @brief
     *
     * @param data
     */
    void LosNet::dealLoginReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_auth_response(false, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        bool suc       = obj["success"].toBool();
        QString msgMsg = obj["message"].toString();
        if (suc)
        {
            QString token = obj["token"].toString();
            LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN, token);
        }
        emit LosRouter::instance()._cmd_auth_response(suc, msgMsg);
    }



    /**
     * @brief dealAutoLoginReply 处理自动登录的逻辑
     * @param data
     */
    void LosNet::dealAutoLoginReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_auth_response(false, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        bool suc       = obj["success"].toBool();
        QString msgMsg = obj["message"].toString();
        if (!suc)
        {
            emit LosRouter::instance()._cmd_auth_autoLogin_response(suc, msgMsg, "", "");
            return;
        }
        QString username = obj["username"].toString();
        QString user_id  = obj["user_id"].toString();
        emit LosRouter::instance()._cmd_auth_autoLogin_response(suc, msgMsg, username, user_id);
        emit LosRouter::instance()._cmd_auth_loginStateChanged(true);
    }



    /**
     * @brief 
     * 
     * @param data 
     */
    void LosNet::dealAgentChatReply(const QByteArray &data) {
        // 处理 Agent 的答复
    }



    /**
     * @brief
     *
     */
    void LosNet::requestInit()
    {
        requestGet(LosCommon::LosNet_Constants::API::ROOT_API, [this](const QByteArray &doc) { this->dealInitReply(doc); });
    }



    /**
     * @brief
     *
     * @param data
     */
    void LosNet::dealInitReply(const QByteArray &data)
    {
        QString reply = QString::fromUtf8(data);
        SUC("init net -> :" + reply, "LosNet");
    }



    /**
     * @brief
     *
     * @param download_url
     * @param savePath
     */
    void LosNet::downloadPlugin(const QString &download_url, const QString &savePath)
    {
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



    /**
     * @brief
     *
     * @param url_str
     * @param func
     */
    void LosNet::fetchPluginReadme(const QString &url_str, std::function<void(const QString &)> func)
    {
        if (url_str.isEmpty())
        {
            func(R"(
                _No description available from server._
                )");
            return;
        }
        QUrl url(url_str);
        QNetworkRequest req(url);
        QNetworkReply *rep = L_net->get(req);
        conn(rep, [func](const QByteArray &data) { func(QString::fromUtf8(data)); });
    }



    /**
     * @brief requestGet
     *
     * @param api
     * @param func
     */
    void LosNet::requestGet(const QString &api, std::function<void(const QByteArray &)> func)
    {
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + api);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        QNetworkReply *rep = L_net->get(req);
        conn(rep, func);
    }



    /**
     * @brief Post
     *
     * @param api
     * @param body
     * @param func
     */
    void LosNet::requestPost(const QString &api, const QByteArray &body, std::function<void(const QByteArray &)> func)
    {
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + api);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        QNetworkReply *rep = L_net->post(req, body);
        conn(rep, func);
    }



    /**
     * @brief conn
     *
     * @param reply
     * @param func
     */
    void LosNet::conn(QNetworkReply *reply, std::function<void(const QByteArray &)> func)
    {
        connect(reply, &QNetworkReply::finished, this,
                [=]()
                {
                    reply->deleteLater();

                    QByteArray resData = reply->readAll();

                    if (reply->error() != QNetworkReply::NoError)
                    {
                        ERR(reply->errorString(), "conn");
                        if (!resData.isEmpty())
                        {
                            func(resData);
                            return;
                        }
                        emit LosRouter::instance()._cmd_auth_response(false, QStringLiteral("网络请求失败"));
                        return;
                    }

                    func(resData);
                });
    }

    /**
     * @brief initConnect
     */
    void LosNet::initConnect()
    {
        auto &router = LosRouter::instance();
        connect(&router, &LosRouter::_cmd_auth_login_request, this,
                [this](const QString &username, const QString &password) { this->requestLogin(username, password); });
        connect(&router, &LosRouter::_cmd_auth_register_request, this,
                [this](const QString &username, const QString &password) { this->requestRegister(username, password); });
        connect(&router, &LosRouter::_cmd_auth_autoLogin_request, this, [this](const QString &token) { this->requestAutoLogin(token); });
    }

} // namespace LosCore