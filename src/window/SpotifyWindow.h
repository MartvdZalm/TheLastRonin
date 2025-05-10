#ifndef SPOTIFYWINDOW_H
#define SPOTIFYWINDOW_H

#include "BaseWindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QTcpServer>
#include <QTcpSocket>
#include "../dao/UserDAO.h"

class SpotifyWindow : public BaseWindow
{
public:
    explicit SpotifyWindow(QWidget* parent = nullptr);

    void setupUI() override;
    void setupConnections() override;
    void setStyle() override;
    void setupEvents() override;

private:
    void startAuthorization();
    void exchangeCodeForToken(const QString& code);

    QString generateCodeVerifier();
    QString generateCodeChallenge(const QString& verifier);

    QLineEdit* emailInput = nullptr;
    QLineEdit* passwordInput = nullptr;
    QLineEdit* clientIdInput = nullptr;
    QPushButton* loginBtn = nullptr;

    QString codeVerifier;
    QNetworkAccessManager* networkManager;
    QTcpServer* localServer;

    UserDAO userDAO;
};


#endif // SPOTIFYWINDOW_H
