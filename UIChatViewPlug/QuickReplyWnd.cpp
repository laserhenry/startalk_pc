//
// Created by lihaibin on 2019-07-03.
//

#include <QHBoxLayout>
#include "QuickReplyWnd.h"
//#include "../LogicManager/LogicManager.h"
#include "ChatViewMainPanel.h"
#include <QScrollBar>
#include <QToolButton>
#include <QSplitter>

extern ChatViewMainPanel *g_pMainPanel;
QuickReplyWnd::QuickReplyWnd(QWidget *parent)
        : UShadowDialog(parent, true, true)
{

#ifndef _WINDOWS
    setWindowFlag(Qt::Popup);
#endif // !_WINDOWS

    auto* titleFrm = new QFrame(this);
    titleFrm->setObjectName("titleFrm");
    titleFrm->setFixedHeight(50);

    auto* titleLabel = new QLabel(tr("快捷回复"), this);
    titleLabel->setAlignment(Qt::AlignCenter);

    auto * titleLay = new QHBoxLayout(titleFrm);
    titleLay->setSpacing(0);

    auto * closeBtn = new QToolButton();

#ifdef _MACOS
    closeBtn->setFixedSize(10, 10);
    titleLay->addWidget(closeBtn);
    closeBtn->setObjectName("gmCloseBtn");
#else
    closeBtn->setFixedSize(20, 20);
    closeBtn->setObjectName("gwCloseBtn");
    titleLay->addWidget(closeBtn);
#endif
    titleLay->addWidget(titleLabel);

    _tagListWidget = new QListWidget(this);
    _tagListWidget->setObjectName("QuickReplyWndList");
    _tagListWidget->setFrameShape(QFrame::NoFrame);
    _tagListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _tagListWidget->verticalScrollBar()->setSingleStep(15);
    _tagListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tagListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _contentListWidget = new QListWidget(this);
    _contentListWidget->setObjectName("QuickReplyWndList");
    _contentListWidget->setFrameShape(QFrame::NoFrame);
    _contentListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _contentListWidget->verticalScrollBar()->setSingleStep(15);
    _contentListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _contentListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto *splt = new QSplitter;
    splt->setHandleWidth(1);
    splt->addWidget(_tagListWidget);
    splt->addWidget(_contentListWidget);
    splt->setStretchFactor(1,2);
    splt->setCollapsible(0, false);
    splt->setCollapsible(1, false);

    auto* mainFrm = new QFrame(this);
    auto* mainlay = new QVBoxLayout(mainFrm);
    mainlay->setMargin(0);
    mainlay->setSpacing(0);
    mainlay->addWidget(titleFrm);
    mainlay->addWidget(splt);

    auto* lay = new QVBoxLayout(_pCenternWgt);
    lay->setMargin(0);
    lay->addWidget(mainFrm);
    lay->setSpacing(0);

    setMoverAble(true, titleFrm);
    resize(700, 480);

    addTagItem();

    connect(closeBtn, &QToolButton::clicked, [this](){this->setVisible(false);});

    QObject::connect(_tagListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclicked(QListWidgetItem*)));
    QObject::connect(_contentListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*)));
}

void QuickReplyWnd::addTagItem() {
    std::vector<QTalk::Entity::ImQRgroup> groups;
    if(g_pMainPanel ){
        ChatMsgManager::getQuickGroups(groups);
    }
    int id = -1;
    for(const QTalk::Entity::ImQRgroup& group :groups){
        auto *item = new QListWidgetItem;
        item->setSizeHint({150, 30});
        if(id == -1){
            id = group.sid;
        }
        item->setText(QString::fromStdString(group.groupname));
        item->setWhatsThis(QString::number(group.sid));
        _tagListWidget->addItem(item);
    }
    if(id != -1)
        addContentItem(id);
}

void QuickReplyWnd::addContentItem(int id) {
    _contentListWidget->clear();
    std::vector<QTalk::Entity::IMQRContent> contents;
    if(g_pMainPanel ){
        ChatMsgManager::getQuickContentByGroup(contents,id);
    }
    for(const QTalk::Entity::IMQRContent& content :contents){
        auto *item = new QListWidgetItem;
        item->setSizeHint({550, 30});
        item->setText(QString::fromStdString(content.content));
        item->setToolTip(QString::fromStdString(content.content));
        _contentListWidget->addItem(item);
    }
}

void QuickReplyWnd::singleclicked(QListWidgetItem *item) {
    int sid = item->whatsThis().toInt();
    addContentItem(sid);
}

void QuickReplyWnd::doubleclicked(QListWidgetItem *item) {
    QString text = item->text();
    emit sendQuickReply(text.toStdString());
}