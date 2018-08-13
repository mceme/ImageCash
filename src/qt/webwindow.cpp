// Copyright (c) 2011-2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "webwindow.h"
#include "ui_webwindow.h"
#include "addressbookpage.h"
#include "addresstablemodel.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "walletmodel.h"
#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QtWebKitWidgets/QWebView>
#include <string>


WebWindow::WebWindow(const PlatformStyle *platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebWindow),
	clientModel(0),
    model(0),
    platformStyle(platformStyle)
{
   // ui->setupUi(this);



	 //connect(ui->addressBookButton, SIGNAL(clicked()), this, SLOT(on_addressBookButton_clicked()));
    //connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
   // connect(ui->deleteButton_is, SIGNAL(clicked()), this, SLOT(deleteClicked()));
   // connect(ui->deleteButton_s, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}



void WebWindow::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;

    if (clientModel) {
    	//  connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
    }
}

void WebWindow::setModel(WalletModel *model)
{
    this->model = model;

    //if (model && model->getOptionsModel())
      //  connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));


}

WebWindow::~WebWindow()
{
    delete ui;
}

