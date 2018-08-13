// Copyright (c) 2011-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_WEBWINDOW_H
#define BITCOIN_QT_WEBWINDOW_H

#include "walletmodel.h"

#include <QStackedWidget>
#include <QtWebKitWidgets/QWebView>
#include <QWidget>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <string>
class ClientModel;
class WalletModel;
class PlatformStyle;
class QString;
class QStringList;

namespace Ui {
    class WebWindow;
}

/**
 * A single entry in the dialog for sending bitcoins.
 * Stacked widget, with different UIs for payment requests
 * with a strong payee identity.
 */
class WebWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WebWindow(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~WebWindow();
    void setClientModel(ClientModel *clientModel);
    void setModel(WalletModel *model);


public Q_SLOTS:

Q_SIGNALS:
    void removeEntry(WebWindow *entry);
    void subtractFeeFromAmountChanged();
    // Fired when a message should be reported to the user
    void message(const QString &title, const QString &message, unsigned int style);

private Q_SLOTS:



private:
    SendCoinsRecipient recipient;
    Ui::WebWindow *ui;
    ClientModel *clientModel;
    WalletModel *model;
    const PlatformStyle *platformStyle;
    bool updateLabel(const QString &address);


};


#endif // BITCOIN_QT_WEBWINDOW_H
