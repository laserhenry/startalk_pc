//
// Created by QITMAC000260 on 2019/11/07.
//

#ifndef QTALK_V2_MESSAGEANALYSIS_H
#define QTALK_V2_MESSAGEANALYSIS_H

#include "SearchItemWgt.h"

namespace QTalk
{
    // text
    void analysisTextMessage(const QString& content, const QString& extendInfo, std::vector<StTextMessage>& messages);
    // link
    void analysisLinkMessage(const QString &text, std::vector<StTextMessage>& messages);
    // image
    void analysisImageMessage(const QString& content, QString& imageLink, qreal& width, qreal& height);
    void analysisEmoticonMessage(const QString& content, QString& pkgid, QString& shortCut);
    // file
    void analysisFileMessage(const QString& content, const QString& extendInfo, StNetSearchResult& ret);
    // CommonTrd
    void analysisCommonTrdMessage(const QString& content, const QString& extendInfo, StNetSearchResult& ret);
    // source code
    void analysisCodeMessage(const QString& content, const QString& extendInfo, StNetSearchResult& ret);
    // video item
    void analysisVideoMessage(const QString& content, const QString& extendInfo, StNetSearchResult& ret);
}


#endif //QTALK_V2_MESSAGEANALYSIS_H
