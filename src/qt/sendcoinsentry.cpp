#include "sendcoinsentry.h"
#include "ui_sendcoinsentry.h"
#include "guiutil.h"
#include "bitcoinunits.h"
#include "addressbookpage.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "addresstablemodel.h"

#include <QApplication>
#include <QClipboard>

SendCoinsEntry::SendCoinsEntry(QWidget* parent)
            : QFrame(parent)
            , ui(new Ui::SendCoinsEntry)
            , model(nullptr)
{
    ui->setupUi(this);

    ui->payToLayout->setSpacing(4);

    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(ui->payTo);

    GUIUtil::setupAddressWidget(ui->payTo, this);
}

SendCoinsEntry::~SendCoinsEntry()
{
    delete ui;
}

void SendCoinsEntry::on_pasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->payTo->setText(QApplication::clipboard()->text());
}

void SendCoinsEntry::on_addressBookButton_clicked()
{
    if(!model)
        return;
    AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::SendingTab, this);
    dlg.setModel(model->getAddressTableModel());
    if(dlg.exec())
    {
        ui->payTo->setText(dlg.getReturnValue());
        ui->payAmount->setFocus();
    }
}

void SendCoinsEntry::on_payTo_textChanged(const QString &address)
{
    if(!model)
        return;
    // Fill in label from address book, if address has an associated label
    QString associatedLabel = model->getAddressTableModel()->labelForAddress(address);
    if(!associatedLabel.isEmpty())
        ui->addAsLabel->setText(associatedLabel);
}

void SendCoinsEntry::setModel(WalletModel *model)
{
    this->model = model;

    if(model && model->getOptionsModel())
    {
        connect(model->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &SendCoinsEntry::updateDisplayUnit);
        connect(model->getOptionsModel(), &OptionsModel::walletStylesheetChanged, this, &SendCoinsEntry::updateIcons);
        // set the icons according to the style options
        updateIcons();
    }
    connect(ui->payAmount, &BitcoinAmountField::textChanged, this, &SendCoinsEntry::payAmountChanged);

    clear();
}

void SendCoinsEntry::setRemoveEnabled(bool enabled)
{
    ui->deleteButton->setEnabled(enabled);
}

void SendCoinsEntry::setMessageEnabled(bool enabled)
{
    ui->messageLabel->setVisible(enabled);
    ui->messageText->setVisible(enabled);

    if (!enabled) {
        ui->messageText->clear();
    }
}

void SendCoinsEntry::clear()
{
    ui->payTo->clear();
    ui->addAsLabel->clear();
    ui->payAmount->clear();
    ui->payTo->setFocus();
    ui->messageText->clear();
    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void SendCoinsEntry::on_deleteButton_clicked()
{
    emit removeEntry(this);
}

bool SendCoinsEntry::validate()
{
    // Check input validity
    bool retval = true;

    if(!ui->payAmount->validate())
    {
        retval = false;
    }
    else
    {
        if(ui->payAmount->value() <= 0)
        {
            // Cannot send 0 coins or less
            ui->payAmount->setValid(false);
            retval = false;
        }
    }

    if(!ui->payTo->hasAcceptableInput() ||
       (model && !model->validateAddress(ui->payTo->text())))
    {
        ui->payTo->setValid(false);
        retval = false;
    }

    return retval;
}

SendCoinsRecipient SendCoinsEntry::getValue()
{
    SendCoinsRecipient rv;

    rv.address = ui->payTo->text();
    rv.label = ui->addAsLabel->text();
    rv.amount = ui->payAmount->value();
	rv.Message = ui->messageText->text();
    return rv;
}

QWidget *SendCoinsEntry::setupTabChain(QWidget *prev)
{
    QWidget::setTabOrder(prev, ui->payTo);
    QWidget::setTabOrder(ui->payTo, ui->addressBookButton);
    QWidget::setTabOrder(ui->addressBookButton, ui->pasteButton);
    QWidget::setTabOrder(ui->pasteButton, ui->deleteButton);
    QWidget::setTabOrder(ui->deleteButton, ui->addAsLabel);
    return ui->payAmount->setupTabChain(ui->addAsLabel);
}

void SendCoinsEntry::setValue(const SendCoinsRecipient &value)
{
    ui->payTo->setText(value.address);
    ui->addAsLabel->setText(value.label);
    ui->payAmount->setValue(value.amount);
	ui->messageText->setText(value.Message);
}

bool SendCoinsEntry::isClear()
{
    return ui->payTo->text().isEmpty();
}

void SendCoinsEntry::setFocus()
{
    ui->payTo->setFocus();
}

void SendCoinsEntry::updateDisplayUnit()
{
    if(model && model->getOptionsModel())
    {
        // Update payAmount with the current unit
        ui->payAmount->setDisplayUnit(model->getOptionsModel()->getDisplayUnit());
    }
}

void SendCoinsEntry::updateIcons()
{
    if(model && model->getOptionsModel())
    {
        ui->addressBookButton->setIcon(QIcon(":/icons/address-book_"+model->getOptionsModel()->getCurrentStyle()));
    }
}
