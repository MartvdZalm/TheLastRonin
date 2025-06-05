#include "SpotifyWindow.h"

#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>

SpotifyWindow::SpotifyWindow(QWidget* parent)
    : BaseWindow(parent)
{
    networkManager = new QNetworkAccessManager(this);
    localServer = new QTcpServer(this);

    setStyle();
    setupUI();
    setupConnections();
    setupEvents();
}

void SpotifyWindow::setStyle()
{

}

void SpotifyWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("Email");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");

    clientIdInput = new QLineEdit();
    clientIdInput->setPlaceholderText("Spotify Client ID");

    loginBtn = new QPushButton("Login");


    mainLayout->addWidget(emailInput);
    mainLayout->addWidget(passwordInput);
    mainLayout->addWidget(clientIdInput);
    mainLayout->addWidget(loginBtn);
}

void SpotifyWindow::setupConnections()
{
    connect(loginBtn, &QPushButton::clicked, this, &SpotifyWindow::startAuthorization);
}


void SpotifyWindow::setupEvents()
{

}

void SpotifyWindow::startAuthorization()
{
    QString clientId = clientIdInput->text().trimmed();
    if (clientId.isEmpty()) {
        qWarning() << "Client ID is required.";
        return;
    }

    codeVerifier = generateCodeVerifier();
    QString codeChallenge = generateCodeChallenge(codeVerifier);

    QUrl authUrl("https://accounts.spotify.com/authorize");
    QUrlQuery query;
    query.addQueryItem("client_id", clientId);
    query.addQueryItem("response_type", "code");
    query.addQueryItem("redirect_uri", "http://127.0.0.1:8080/callback");
    query.addQueryItem("code_challenge_method", "S256");
    query.addQueryItem("code_challenge", codeChallenge);
    query.addQueryItem("scope", "playlist-read-private");
    authUrl.setQuery(query);

    QDesktopServices::openUrl(authUrl);

    connect(localServer, &QTcpServer::newConnection, this, [=]() {
        QTcpSocket* socket = localServer->nextPendingConnection();
        socket->waitForReadyRead();
        QString request = socket->readAll();

        QRegularExpression rx("GET /callback\\?code=([^& ]+)");
        QRegularExpressionMatch match = rx.match(request);

        if (match.hasMatch()) {
            QString code = match.captured(1);
            exchangeCodeForToken(code);

            socket->write("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                          "<h1>Login successful. You can close this window.</h1>");
        } else {
            socket->write("HTTP/1.1 400 Bad Request\r\n\r\n");
        }

        socket->disconnectFromHost();
    });

    if (!localServer->listen(QHostAddress::Any, 8080)) {
        qWarning() << "Failed to listen on port 8080";
    }
}

void SpotifyWindow::exchangeCodeForToken(const QString& code)
{
    QString clientId = clientIdInput->text().trimmed();

    QUrl url("https://accounts.spotify.com/api/token");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("grant_type", "authorization_code");
    params.addQueryItem("code", code);
    params.addQueryItem("redirect_uri", "http://127.0.0.1:8080/callback");
    params.addQueryItem("client_id", clientId);
    params.addQueryItem("code_verifier", codeVerifier);

    QNetworkReply* reply = networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        qDebug() << "Token response:" << response;

        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = doc.object();
        QString accessToken = jsonObj["access_token"].toString();

        User newUser;
        newUser.email = emailInput->text().trimmed();
        newUser.password = passwordInput->text().trimmed();
        newUser.clientId = clientId;

        UserDAO userDao;
        if (userDao.insertUser(newUser)) {
            qDebug() << "User inserted successfully!";
        } else {
            qDebug() << "Failed to insert user.";
        }

        reply->deleteLater();
    });
}

QString SpotifyWindow::generateCodeVerifier()
{
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~";
    QString verifier;
    for (int i = 0; i < 64; ++i)
        verifier += chars.at(QRandomGenerator::global()->bounded(chars.length()));
    return verifier;
}

QString SpotifyWindow::generateCodeChallenge(const QString& verifier)
{
    QByteArray hash = QCryptographicHash::hash(verifier.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromUtf8(hash.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

