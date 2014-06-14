#include "baseclient.h"

#include <QString>

BaseClient::BaseClient(ClientId clientId,
        MocapSubjectList *subjectList,
        QPushButton *pushButton,
        QLineEdit *lineEditStatus,
        QObject *parent)
: QObject(parent)
, subjects(subjectList)
, button(pushButton)
, statusLine(lineEditStatus)
, connected(false)
, count(0)
, id(clientId)
{
    QObject::connect(button, SIGNAL(clicked()),                  this,   SLOT(handleButtonClick()));
    QObject::connect(this,   SIGNAL(stateConnecting()),          this,   SLOT(UIConnectingState()));
    QObject::connect(this,   SIGNAL(stateConnected()),           this,   SLOT(UIConnectedState()));
    QObject::connect(this,   SIGNAL(stateDisconnecting()),       this,   SLOT(UIDisconnectingState()));
    QObject::connect(this,   SIGNAL(stateDisconnected()),        this,   SLOT(UIDisconnectedState()));
    QObject::connect(this,   SIGNAL(outMessage_(QString)),       parent, SLOT(showMessage(QString)));
    QObject::connect(this,   SIGNAL(updateFrame(BaseClient::ClientId,uint)), parent, SLOT(processFrame(BaseClient::ClientId, uint)));
}

void BaseClient::tick()
{
    if(isRunning())
    {
        statusLine->setText(QString("%1").arg(count));
        count=0;
    }
    else
    {
        statusLine->setText(QString("Not Connected"));
    }
}

void BaseClient::outMessage(QString msg)
{
    emit outMessage_(QString("%1 - %2").arg(this->ClientStr()).arg(msg));
}

void BaseClient::newFrame(uint i)
{
    emit updateFrame(this->id, i);
}


void BaseClient::handleButtonClick()
{
    if(connected)
    {
        mocapStop();
    }
    else
    {
        mocapStart();
    }
}

void BaseClient::UIConnectingState()
{
    button->setEnabled(false);
    button->setText("Connecting");
}

void BaseClient::UIConnectedState()
{
    button->setEnabled(true);
    button->setText("Disconnect");
}

void BaseClient::UIDisconnectingState()
{
    button->setEnabled(false);
    button->setText("Disconnecting");
}

void BaseClient::UIDisconnectedState()
{
    button->setEnabled(true);
    button->setText("Connect");
}


