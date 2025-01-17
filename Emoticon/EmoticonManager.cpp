﻿#include "EmoticonManager.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include "LocalEmoticon.h"
#include "NetEmoticon.h"
#include "../CustomUi/TitleBar.h"

#define DEM_FIXED_WIDTH 630
#define DEM_FIXED_HEIGHT 560

//
EmoticonManager::EmoticonManager(QMap<QString, StEmoticon>& mapEmo, QWidget* parent)
	: UShadowDialog(parent, true)
	, _pLocalEmoManager(nullptr)
	, _pNetEmoManager(nullptr)
	, _pBtnLocalEmo(nullptr)
	, _pBtnNetEmo(nullptr)
	, _mapEmos(mapEmo)
{
	initUi();
}

EmoticonManager::~EmoticonManager()
= default;

void EmoticonManager::initUi()
{
	setFixedSize(DEM_FIXED_WIDTH, DEM_FIXED_HEIGHT);
	// top
    auto * topFrame = new TitleBar(tr("管理表情"), this,this);
	setMoverAble(true, topFrame);
	//
	auto* midFrame = new QFrame(this);
	midFrame->setObjectName("EmoticonManagerMidFrame");
    auto * midLayout = new QHBoxLayout(midFrame);
	_pBtnLocalEmo = new QPushButton(tr("本地表情"));
	_pBtnNetEmo = new QPushButton(tr("服务器表情"));
    _pBtnLocalEmo->setFlat(true);
    _pBtnNetEmo->setFlat(true);
	_pBtnLocalEmo->setObjectName("EmoticonManagerLocalEmo");
	_pBtnNetEmo->setObjectName("EmoticonManagerNetEmo");
	_pBtnLocalEmo->setCheckable(true);
	_pBtnNetEmo->setCheckable(true);
	midLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
	midLayout->addWidget(_pBtnLocalEmo);
	midLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum));
	midLayout->addWidget(_pBtnNetEmo);
	midLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
	//
    auto* bottomFrame = new QFrame(this);
    bottomFrame->setObjectName("EmoticonManagerbottomFrame");
    auto * bottomLayout = new QHBoxLayout(bottomFrame);
	_pLocalEmoManager = new LocalEmoticon(_mapEmos, this);
	_pNetEmoManager = new NetEmoticon(_mapEmos, this);
	bottomLayout->addWidget(_pLocalEmoManager);
	bottomLayout->addWidget(_pNetEmoManager);
	//
    auto * layout = new QVBoxLayout(_pCenternWgt);
	layout->setMargin(0);
    layout->setSpacing(0);
	layout->addWidget(topFrame);
	layout->addWidget(midFrame);
	layout->addWidget(bottomFrame);
	layout->setStretch(2, 1);

	connect(_pBtnLocalEmo, &QPushButton::clicked, this, &EmoticonManager::onBtnClicked);
	connect(_pBtnNetEmo, &QPushButton::clicked, this, &EmoticonManager::onBtnClicked);
	// 默认选中
	_pBtnLocalEmo->click();
}

/**
  * @函数名   onBtnClicked
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/10/19
  */
void EmoticonManager::onBtnClicked()
{
	auto* btn = (QPushButton*)sender();
	if (btn)
	{
		bool isLocal = btn == _pBtnLocalEmo;
		_pBtnLocalEmo->setChecked(isLocal);
		_pBtnNetEmo->setChecked(!isLocal);
		_pLocalEmoManager->setVisible(isLocal);
		_pNetEmoManager->setVisible(!isLocal);
	}
}
