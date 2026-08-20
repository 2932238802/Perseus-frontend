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


namespace
{
    // 解析 SSE 累积缓冲区里所有完整行 (每行以 \n 结尾), 消费掉已处理部分.
    // 剩下的不足一行内容保留在 pending 中, 下次 readyRead / finished 继续拼.
    void processAgentStreamLines(QByteArray &pending)
    {
        auto &router = LosCore::LosRouter::instance();
        int nl       = -1;
        while ((nl = pending.indexOf('\n')) >= 0)
        {
            QByteArray line = pending.left(nl);
            pending.remove(0, nl + 1);
            while (line.endsWith('\r'))
                line.chop(1);
            if (!line.startsWith("data:"))
                continue;
            QByteArray payload = line.mid(5);
            if (payload.startsWith(' '))
                payload.remove(0, 1);
            if (payload == "[DONE]")
            {
                router._cmd_agent_replyDone();
                continue;
            }
            QJsonParseError perr;
            QJsonDocument pdoc = QJsonDocument::fromJson(payload, &perr);
            if (perr.error != QJsonParseError::NoError || !pdoc.isObject())
                continue;
            const QString piece = pdoc.object().value("d").toString();
            if (!piece.isEmpty())
                router._cmd_agent_replyChunk(piece);
        }
    }
} // namespace


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
    void LosNet::requestAgentChat(const QString &msg, const QString &provider_name, const QString &model)
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QJsonObject body;
        body["message"]       = msg;
        body["provider_name"] = provider_name;
        body["model"]         = model;
        QByteArray data       = QJsonDocument(body).toJson(QJsonDocument::Compact);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_CHAT_API));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, data);
        conn(rep, [this](const QByteArray &doc) { this->dealAgentChatReply(doc); });
    }



    /**
     * @brief
     *
     * @param msg
     * @param provider_name
     * @param model
     */
    void LosNet::requestAgentChatStream(const QString &msg, const QString &provider_name, const QString &model)
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QJsonObject body;
        body["message"]       = msg;
        body["provider_name"] = provider_name;
        body["model"]         = model;
        QByteArray data       = QJsonDocument(body).toJson(QJsonDocument::Compact);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_CHAT_API));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, data);
        auto *pending      = new QByteArray;
        connect(rep, &QNetworkReply::readyRead, this,
                [this, rep, pending]()
                {
                    pending->append(rep->readAll());
                    processAgentStreamLines(*pending);
                });
        connect(rep, &QNetworkReply::finished, this,
                [this, rep, pending]()
                {
                    pending->append(rep->readAll());
                    if (!pending->isEmpty() && !pending->contains('\n'))
                    {
                        QByteArray line = *pending;
                        if (line.startsWith("data:"))
                        {
                            QByteArray payload = line.mid(5);
                            if (payload.startsWith(' '))
                                payload.remove(0, 1);
                            if (payload == "[DONE]")
                            {
                            }
                            else
                            {
                                QJsonParseError perr2;
                                QJsonDocument pdoc = QJsonDocument::fromJson(payload, &perr2);
                                if (perr2.error == QJsonParseError::NoError && pdoc.isObject())
                                {
                                    const QString piece = pdoc.object().value("d").toString();
                                    if (!piece.isEmpty())
                                        emit LosRouter::instance()._cmd_agent_replyChunk(piece);
                                }
                            }
                        }
                        pending->clear();
                    }
                    else
                    {
                        processAgentStreamLines(*pending);
                    }
                    const int status = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                    if (status >= 400 && !pending->isEmpty() && !pending->trimmed().isEmpty())
                    {
                        const QString body = QString::fromUtf8(pending->trimmed());
                        QString errMsg;
                        QJsonParseError perr;
                        const QJsonDocument edoc = QJsonDocument::fromJson(body.toUtf8(), &perr);
                        if (perr.error == QJsonParseError::NoError && edoc.isObject())
                        {
                            const QJsonObject eobj = edoc.object();
                            errMsg                 = eobj.value(QStringLiteral("message")).toString();
                            if (errMsg.isEmpty())
                                errMsg = eobj.value(QStringLiteral("error")).toObject().value(QStringLiteral("message")).toString();
                        }
                        if (errMsg.isEmpty())
                            errMsg = body.left(200);
                        emit LosRouter::instance()._cmd_agent_reply(false, errMsg);
                    }
                    emit LosRouter::instance()._cmd_agent_replyDone();
                    delete pending;
                    rep->deleteLater();
                });
    }



    /**
     * @brief
     *
     * @param base_url
     * @param api_key
     */
    void LosNet::requestAgentFetchModels(const QString &base_url, const QString &api_key)
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QJsonObject body;
        body["base_url"] = base_url;
        body["api_key"]  = api_key;
        QByteArray data  = QJsonDocument(body).toJson(QJsonDocument::Compact);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_LIST_MODELS));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, data);
        conn(rep, [this](const QByteArray &doc) { this->dealAgentFetchModelsReply(doc); });
    }



    /**
     * @brief 新增厂商
     * json
        pub struct AddAgentRequest {
            pub provider_name: String,
            pub base_url: String,
            pub models: Vec<String>,
            pub api_key: String,
        }
     * @param provider
     * @param baseUrl
     * @param apiKey
     * @param models
     */
    void LosNet::requestAgentAddProvider(const QString &provider, const QString &base_url, const QString &api_key, const QStringList &models)
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QJsonObject body;
        body["provider_name"] = provider;
        body["base_url"]      = base_url;
        QJsonArray modelArr;
        for (const auto &str : models)
        {
            modelArr.append(str);
        }
        body["models"]  = modelArr;
        body["api_key"] = api_key;
        QByteArray data = QJsonDocument(body).toJson(QJsonDocument::Compact);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_ADD_API));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, data);
        conn(rep, [this](const QByteArray &doc) { this->dealAgentAddProviderReply(doc); });
    }



    /**
     * @brief 删除厂商/模型
     * @param provider 厂商名
     * @param model    模型名; 为空则删除整个厂商(模型级联删除), 非空则只删除该模型
     */
    void LosNet::requestAgentDeleteProvider(const QString &provider, const QString &model)
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QJsonObject body;
        body["provider_name"] = provider;
        if (!model.isEmpty())
            body["model"] = model;
        QByteArray data = QJsonDocument(body).toJson(QJsonDocument::Compact);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_DELETE_API));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, data);
        conn(rep, [this](const QByteArray &doc) { this->dealAgentDeleteProviderReply(doc); });
    }



    /**
     * @brief requestListProviders
     * 申请 provider list
     */
    void LosNet::requestListProviders()
    {
        auto &state   = LosState::instance();
        QString token = state.get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
        QUrl url(LosCommon::LosNet_Constants::BASE_URL + QString(LosCommon::LosNet_Constants::API::AGENT_LIST_PROVIDERS));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, LosCommon::LosNet_Constants::HEADER_TYPE::JSON_TYPE);
        req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
        QNetworkReply *rep = L_net->post(req, QByteArray());
        conn(rep, [this](const QByteArray &doc) { this->dealListProvidersReply(doc); });
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
    void LosNet::dealAgentChatReply(const QByteArray &data)
    {
        // 处理 Agent 的答复
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_agent_reply(false, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        const bool suc = obj["success"].toBool();
        if (!suc)
        {
            // 失败时后端返回 message
            emit LosRouter::instance()._cmd_agent_reply(false, obj["message"].toString());
            return;
        }
        // 成功时回复内容在 reply 字段
        emit LosRouter::instance()._cmd_agent_reply(true, obj["reply"].toString());
    }



    /**
     * @brief dealAgentFetchModels 处理 返回的模型的答复
     *
     * @param data
     */
    void LosNet::dealAgentFetchModelsReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_agent_listModels_response(false, {}, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        const bool ok     = obj["success"].toBool();
        const QString msg = obj["message"].toString();
        if (!ok)
        {
            emit LosRouter::instance()._cmd_agent_listModels_response(false, {}, msg);
            return;
        }
        QStringList models;
        const QJsonArray arr = obj["models"].toArray();
        for (const QJsonValue &v : arr)
        {
            const QString m = v.toString().trimmed();
            if (!m.isEmpty())
                models.append(m);
        }
        emit LosRouter::instance()._cmd_agent_listModels_response(true, models, msg);
    }



    /**
     * @brief
     *
     * @param data
     */
    void LosNet::dealAgentAddProviderReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_agent_addProvider_response(false, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        const bool ok     = obj["success"].toBool();
        const QString msg = obj["message"].toString();
        emit LosRouter::instance()._cmd_agent_addProvider_response(ok, msg);
    }



    /**
     * @brief 处理删除厂商/模型的结果
     */
    void LosNet::dealAgentDeleteProviderReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_agent_deleteProvider_response(false, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        const bool ok     = obj["success"].toBool();
        const QString msg = obj["message"].toString();
        emit LosRouter::instance()._cmd_agent_deleteProvider_response(ok, msg);
    }



    /**
     * @brief
     *
     * @param data
     */
    void LosNet::dealListProvidersReply(const QByteArray &data)
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError)
        {
            ERR("解析失败", "LosNet");
            emit LosRouter::instance()._cmd_agent_listProviders_response(false, {}, QStringLiteral("响应解析失败"));
            return;
        }
        QJsonObject obj(doc.object());
        const bool ok     = obj["success"].toBool();
        const QString msg = obj["message"].toString();
        if (!ok)
        {
            emit LosRouter::instance()._cmd_agent_listProviders_response(false, {}, msg);
            return;
        }
        QMap<QString, QStringList> providerModels;
        const QJsonArray providers = obj["providers"].toArray();
        for (const QJsonValue &pv : providers)
        {
            const QJsonObject p = pv.toObject();
            const QString name  = p["provider_name"].toString();
            QStringList models;
            for (const QJsonValue &mv : p["models"].toArray())
            {
                const QString m = mv.toString().trimmed();
                if (!m.isEmpty())
                    models.append(m);
            }
            if (!name.isEmpty())
                providerModels.insert(name, models);
        }
        emit LosRouter::instance()._cmd_agent_listProviders_response(true, providerModels, msg);
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
                    const int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                    QByteArray resData   = reply->readAll();
                    if (httpStatus == 401)
                    {
                        emit LosRouter::instance()._cmd_needAuth();
                        return;
                    }
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
        connect(&router, &LosRouter::_cmd_agent_listModels_request, this,
                [this](const QString &baseUrl, const QString &apiKey) { this->requestAgentFetchModels(baseUrl, apiKey); });
        connect(&router, &LosRouter::_cmd_agent_addProvider_request, this,
                [this](const QString &p, const QString &b, const QString &k, const QStringList &m) { this->requestAgentAddProvider(p, b, k, m); });
        connect(&router, &LosRouter::_cmd_agent_deleteProvider_request, this,
                [this](const QString &p, const QString &m) { this->requestAgentDeleteProvider(p, m); });
        connect(&router, &LosRouter::_cmd_agent_listProviders_request, this, [this]() { this->requestListProviders(); });
    }

} // namespace LosCore