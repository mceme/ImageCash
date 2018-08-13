// Copyright (c) 2011-2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "encryptdecryptdialog.h"
#include "ui_encryptdecryptdialog.h"
#include "addressbookpage.h"
#include "addresstablemodel.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "walletmodel.h"
#include "ecdsa.h"
#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <string>
#include <chrono>
#include <thread>


ecdsa ecdsa;
QStringList fileNames;

EncryptDecryptDialog::EncryptDecryptDialog(const PlatformStyle *platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncryptDecryptDialog),
	clientModel(0),
    model(0),
    platformStyle(platformStyle)
{
    ui->setupUi(this);

    //setCurrentWidget(ui->EncryptDecryptDialog);

    //if (platformStyle->getUseExtraSpacing())
     //   ui->payToLayout->setSpacing(4);
#if QT_VERSION >= 0x040700
    ui->addAsLabel->setPlaceholderText(tr("******"));
#endif

    QString theme = GUIUtil::getThemeName();

    // These icons are needed on Mac also!
   // ui->addressBookButton->setIcon(QIcon(":/icons/" + theme + "/address-book"));
    ui->pasteButton->setIcon(QIcon(":/icons/" + theme + "/editpaste"));
    //ui->deleteButton->setIcon(QIcon(":/icons/" + theme + "/remove"));
    //ui->deleteButton_is->setIcon(QIcon(":/icons/" + theme + "/remove"));
    //ui->deleteButton_s->setIcon(QIcon(":/icons/" + theme + "/remove"));
      
    // normal dash address field
    GUIUtil::setupAddressWidget(ui->payTo, this);
    // just a label for displaying dash address(es)
    ui->payTo->setFont(GUIUtil::fixedPitchFont());

    // Connect signals

    connect(ui->chooserButton, SIGNAL(clicked()), this, SLOT(on_chooserButton_clicked()));

    connect(ui->encryptButton, SIGNAL(clicked()), this, SLOT(on_EncryptButton_clicked()));

    connect(ui->decryptButton, SIGNAL(clicked()), this, SLOT(on_DecryptButton_clicked()));


    ui->FileNamesTxt->setReadOnly(true);

	 //connect(ui->addressBookButton, SIGNAL(clicked()), this, SLOT(on_addressBookButton_clicked()));
    //connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
   // connect(ui->deleteButton_is, SIGNAL(clicked()), this, SLOT(deleteClicked()));
   // connect(ui->deleteButton_s, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}



void EncryptDecryptDialog::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;

    if (clientModel) {
    	//  connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
    }
}

void EncryptDecryptDialog::setModel(WalletModel *model)
{
    this->model = model;

    //if (model && model->getOptionsModel())
      //  connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

    clear();
}

EncryptDecryptDialog::~EncryptDecryptDialog()
{
    delete ui;
}


void EncryptDecryptDialog::on_chooserButton_clicked()
{
    // Paste text from clipboard into recipient field
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    dialog.setViewMode(QFileDialog::List);
    dialog.setOption(QFileDialog::DontUseNativeDialog, false);

    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
        QString fnames;
        QString f ;
        for(int i=0;i<fileNames.size();++i)
         {
        	  f = fileNames[i];
              fnames+=f + ";";
         }
           ui->FileNamesTxt->setText(fnames);
       }
}

void EncryptDecryptDialog::on_pasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->payTo->setText(QApplication::clipboard()->text());
}


void EncryptDecryptDialog::on_EncryptButton_clicked()
{
	if(!this->validate()) return;

CAmount amount = model->getBalance();
   if(amount<20){
	   ui->MessageBox->setText("Need least 20 IMG balance!");
	   return;
   }

  encrypt();

}


void EncryptDecryptDialog::on_DecryptButton_clicked()
{
	if(!this->validate()) return;
	CAmount amount = model->getBalance();
	 if(amount<20){
		   ui->MessageBox->setText("Need least 20 IMG balance!");
		   return;
	   }
  decrypt();

}

void EncryptDecryptDialog::on_addressBookButton_clicked()
{
    if(!model)
        return;
    AddressBookPage dlg(platformStyle, AddressBookPage::ForSelection, AddressBookPage::SendingTab, this);
    dlg.setModel(model->getAddressTableModel());
    if(dlg.exec())
    {
        //ui->FileNamesTxt->setText(dlg.getReturnValue());

        //ui->addAsLabel->setText(dlg.getReturnValue());
        //ui->payAmount->setFocus();
    }
}

void EncryptDecryptDialog::on_payTo_textChanged(const QString &address)
{
	 WalletModel::EncryptionStatus encStatus = model->getEncryptionStatus();
		    if(encStatus == model->Locked || encStatus == model->UnlockedForMixingOnly)
		    {
		        WalletModel::UnlockContext ctx(model->requestUnlock());
		        if(!ctx.isValid())
		        {
		            // Unlock wallet was cancelled

		            return;
		        }
		        updateLabel(address);
		        return;
		    }

		    updateLabel(address);

}



void EncryptDecryptDialog::clear()
{
    // clear UI elements for normal payment
    ui->FileNamesTxt->clear();
    ui->addAsLabel->clear();
    ui->payTo->clear();
    ui->MessageBox->clear();


    // update the display unit, to not use the default ("BTC")
    //updateDisplayUnit();
}

void EncryptDecryptDialog::deleteClicked()
{
    Q_EMIT removeEntry(this);
}

bool EncryptDecryptDialog::validate()
{
    if (!model)
        return false;

    // Check input validity
    bool retval = true;

    // Skip checks for payment request
    if (recipient.paymentRequest.IsInitialized())
        return retval;

    if (!model->validateAddress(ui->payTo->text()))
    {
        ui->payTo->setValid(false);
        retval = false;
    }

    if(ui->FileNamesTxt->text().isEmpty())
    {
	   ui->FileNamesTxt->setStyleSheet("QLineEdit { background: rgb(220, 20, 60); selection-background-color: rgb(233, 99, 0); }");

    	  retval = false;
    }

    if(ui->addAsLabel->text().isEmpty())
    {
        ui->addAsLabel->setStyleSheet("QLineEdit { background: rgb(255, 128, 128); selection-background-color: rgb(255, 128, 128); }");
          retval = false;
       }
    else {
    	   ui->addAsLabel->setStyleSheet("QLineEdit { background: rgb(255,255,255); selection-background-color: rgb(255,255,255); }");

    }



    // Reject dust outputs:
    //if (retval && GUIUtil::isDust(ui->payTo->text(), ui->payAmount->value())) {
    //    ui->payAmount->setValid(false);
    //    retval = false;
    //}

    return retval;
}

SendCoinsRecipient EncryptDecryptDialog::getValue()
{
    // Payment request
    if (recipient.paymentRequest.IsInitialized())
        return recipient;

    // Normal payment
    recipient.address = ui->payTo->text();
    recipient.label = ui->addAsLabel->text();
    //recipient.amount = ui->payAmount->value();
    //recipient.message = ui->messageTextLabel->text();
    //recipient.fSubtractFeeFromAmount = (ui->checkboxSubtractFeeFromAmount->checkState() == Qt::Checked);

    return recipient;
}

QWidget *EncryptDecryptDialog::setupTabChain(QWidget *prev)
{
    QWidget::setTabOrder(prev, ui->FileNamesTxt);
    QWidget::setTabOrder(ui->FileNamesTxt, ui->addAsLabel);
    //QWidget *w = ui->payAmount->setupTabChain(ui->addAsLabel);
    //QWidget::setTabOrder(w, ui->checkboxSubtractFeeFromAmount);
    //QWidget::setTabOrder(ui->checkboxSubtractFeeFromAmount, ui->addressBookButton);
    //QWidget::setTabOrder(ui->addressBookButton, ui->pasteButton);
    //QWidget::setTabOrder(ui->pasteButton, ui->deleteButton);
    return ui->pasteButton;
}

void EncryptDecryptDialog::setValue(const SendCoinsRecipient &value)
{
    recipient = value;

    if (recipient.paymentRequest.IsInitialized()) // payment request
    {
        if (recipient.authenticatedMerchant.isEmpty()) // unauthenticated
        {
            ui->payTo->setText(recipient.address);
            ui->addAsLabel->clear();

           // ui->memoTextLabel_is->setText(recipient.message);
           // ui->payAmount_is->setValue(recipient.amount);

            ui->chooserButton->setEnabled(false);
           // setCurrentWidget(ui->SendCoins_UnauthenticatedPaymentRequest);
        }
        else // authenticated
        {

            ui->chooserButton->setEnabled(true);
            //ui->memoTextLabel_s->setText(recipient.message);
            //ui->payAmount_s->setValue(recipient.amount);
            //ui->payAmount_s->setReadOnly(true);
           // setCurrentWidget(ui->SendCoins_AuthenticatedPaymentRequest);
        }
    }
    else // normal payment
    {
        // message
        //ui->messageTextLabel->setText(recipient.message);
        //ui->messageTextLabel->setVisible(!recipient.message.isEmpty());
        //ui->messageLabel->setVisible(!recipient.message.isEmpty());

        ui->addAsLabel->clear();
        ui->payTo->setText(recipient.address); // this may set a label from addressbook
       // if (!recipient.label.isEmpty()) // if a label had been set from the addressbook, don't overwrite with an empty label
       //     ui->addAsLabel->setText(recipient.label);
       // ui->payAmount->setValue(recipient.amount);
    }
}

void EncryptDecryptDialog::setAddress(const QString &address)
{
    ui->payTo->setText(address);

}

bool EncryptDecryptDialog::isClear()
{
    return ui->FileNamesTxt->text().isEmpty() && ui->addAsLabel->text().isEmpty() &&  ui->payTo->text().isEmpty();
}

void EncryptDecryptDialog::setFocus()
{
    ui->FileNamesTxt->setFocus();
}

void EncryptDecryptDialog::updateDisplayUnit()
{
    if(model && model->getOptionsModel())
    {
        // Update payAmount with the current unit
       // ui->payAmount->setDisplayUnit(model->getOptionsModel()->getDisplayUnit());
       // ui->payAmount_is->setDisplayUnit(model->getOptionsModel()->getDisplayUnit());
       // ui->payAmount_s->setDisplayUnit(model->getOptionsModel()->getDisplayUnit());
    }
}

bool EncryptDecryptDialog::updateLabel(const QString &strAddress)
{
    if(!model)
        return false;


    std::string srt=strAddress.toUtf8().constData();
    std::string key=model->dumpprivkey(srt, false);

    // Fill in label from address book, if address has an associated label
    //QString associatedLabel = model->getAddressTableModel()->labelForAddress(address);
    if(key != "")
    {
        //ui->addAsLabel->setText(associatedLabel);

    	QString Qkey = QString::fromLocal8Bit(key.c_str());
    	 ui->addAsLabel->setText(Qkey);
    	 ui->MessageBox->clear();
        return true;
    }
    else {

    	 ui->MessageBox->setText("Cannot resolve key, wallet is unlocked?");
    	 ui->addAsLabel->clear();
    }

    return false;
}


void EncryptDecryptDialog::encrypt()
{

	QString key = ui->addAsLabel->text();

	std::string filestr;
	std::string keystr;
	bool status=false;
	 for(int i=0; i<fileNames.size(); ++i)
	         {
		      ui->MessageBox->setText("Encrypting file: "+fileNames[i]);
	        	  QString file = fileNames[i];
	        	  filestr = file.toUtf8().constData();
		          keystr = key.toUtf8().constData();
                  ecdsa.encrypt(filestr, keystr, status);


                  int f = 0;
                     while(status==false && f < 60){

                   	    int64_t timenano=1000;
                   	     std::this_thread::sleep_for(std::chrono::nanoseconds(timenano));
                   	     f++;
                     }
	         }

	 clear();
	 ui->MessageBox->setText("Encrypt Complete!");


}

void EncryptDecryptDialog::decrypt()
{

	QString key = ui->addAsLabel->text();
	std::string filestr;
	std::string keystr;
	bool status=false;
	 for(int i=0; i<fileNames.size(); ++i)
	         {
		      ui->MessageBox->setText("Decrypting file: "+fileNames[i]);
	        	  QString file = fileNames[i];
	        	  filestr = file.toUtf8().constData();
		          keystr = key.toUtf8().constData();
                  ecdsa.decrypt(filestr, keystr, status);

                   int f = 0;
                  while(status==false && f < 60){

                	    int64_t timenano=1000;
                	     std::this_thread::sleep_for(std::chrono::nanoseconds(timenano));
                	     f++;
                  }
	         }

	 clear();
	 ui->MessageBox->setText("Decrypt Complete!");

}
