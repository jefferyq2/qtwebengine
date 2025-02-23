// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>
#include <QtWebEngineCore/qtwebenginecoreglobal.h>

class tst_GetDomainAndRegistry final : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void getDomainAndRegistry();
};

void tst_GetDomainAndRegistry::getDomainAndRegistry() {
    QCOMPARE(qWebEngineGetDomainAndRegistry({"http://www.google.com/"}), QString("google.com"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"http://www.google.co.uk/"}), QString("google.co.uk"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"http://127.0.0.1/"}), QString());
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://qt.io/"}), QString("qt.io"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://download.qt.io/"}), QString("qt.io"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://foo.fr/"}), QString("foo.fr"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://foo.gouv.fr/"}), QString("foo.gouv.fr"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://bar.foo.gouv.fr/"}), QString("foo.gouv.fr"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://fr.wikipedia.org/wiki/%C3%89l%C3%A9phant"}), QString("wikipedia.org"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://foo.günstigbestellen.de/"}), QString("foo.xn--gnstigbestellen-zvb.de"));
    QCOMPARE(qWebEngineGetDomainAndRegistry({"https://foo.g\u00fcnstigbestellen.de/"}), QString("foo.xn--gnstigbestellen-zvb.de"));
}


QTEST_MAIN(tst_GetDomainAndRegistry)
#include "tst_getdomainandregistry.moc"
