// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef PROFILE_IO_DATA_QT_H
#define PROFILE_IO_DATA_QT_H

#include "content/public/browser/browsing_data_remover.h"
#include "content/public/browser/storage_partition.h"
#include "extensions/buildflags/buildflags.h"

#include "net/proxy_config_monitor.h"
#include "profile_adapter.h"

#include <QtCore/QString>
#include <QtCore/QPointer>
#include <QtCore/QRecursiveMutex>

namespace cert_verifier {
namespace mojom {
class CertVerifierCreationParams;
}}

namespace net {
class ClientCertStore;
}

namespace QtWebEngineCore {

struct ClientCertificateStoreData;
class CookieMonsterDelegateQt;
class ProfileIODataQt;
class ProfileQt;

class BrowsingDataRemoverObserverQt : public content::BrowsingDataRemover::Observer {
public:
    BrowsingDataRemoverObserverQt(ProfileIODataQt *profileIOData);

    void OnBrowsingDataRemoverDone(uint64_t) override;

private:
    ProfileIODataQt *m_profileIOData;
};

// ProfileIOData contains data that lives on the IOthread
// we still use shared memebers and use mutex which breaks
// idea for this object, but this is wip.

class ProfileIODataQt : public content::StoragePartition::NetworkContextCreatedObserver {

public:
    enum ClearHttpCacheState {
        Completed = 0,
        Removing,
        Resetting
    };

    ProfileIODataQt(ProfileQt *profile); // runs on ui thread
    virtual ~ProfileIODataQt();

    QPointer<ProfileAdapter> profileAdapter();

    void initializeOnUIThread(); // runs on ui thread
    void shutdownOnUIThread(); // runs on ui thread

    bool canGetCookies(const QUrl &firstPartyUrl, const QUrl &url) const;

    void setFullConfiguration(); // runs on ui thread
    void resetNetworkContext(); // runs on ui thread

    void clearHttpCache(); // runs on ui thread
    bool isClearHttpCacheInProgress() const { return m_clearHttpCacheState != Completed; }

    void ConfigureNetworkContextParams(bool in_memory,
                                       const base::FilePath &relative_partition_path,
                                       network::mojom::NetworkContextParams *network_context_params,
                                       cert_verifier::mojom::CertVerifierCreationParams *cert_verifier_creation_params);

#if QT_CONFIG(ssl)
    ClientCertificateStoreData *clientCertificateStoreData();
#endif
    std::unique_ptr<net::ClientCertStore> CreateClientCertStore();
    static ProfileIODataQt *FromBrowserContext(content::BrowserContext *browser_context);

    base::WeakPtr<ProfileIODataQt> getWeakPtrOnIOThread();

    CookieMonsterDelegateQt *cookieDelegate() const { return m_cookieDelegate.get(); }

    // content::StoragePartition::NetworkContextCreatedObserver overrides:
    void OnNetworkContextCreated(content::StoragePartition *storage) override; // runs on ui thread

private:
    void removeBrowsingDataRemoverObserver();

    ProfileQt *m_profile;
    scoped_refptr<CookieMonsterDelegateQt> m_cookieDelegate;
    QPointer<ProfileAdapter> m_profileAdapter; // never dereferenced in IO thread and it is passed by qpointer
    ProfileAdapter::PersistentCookiesPolicy m_persistentCookiesPolicy;
    std::unique_ptr<ProxyConfigMonitor> m_proxyConfigMonitor;

#if QT_CONFIG(ssl)
    ClientCertificateStoreData *m_clientCertificateStoreData;
#endif
    QString m_httpAcceptLanguage;
    QString m_httpUserAgent;
    ProfileAdapter::HttpCacheType m_httpCacheType;
    QString m_httpCachePath;
    QString m_storageName;
    bool m_inMemoryOnly;
    QRecursiveMutex m_mutex;
    int m_httpCacheMaxSize = 0;
    BrowsingDataRemoverObserverQt m_removerObserver;
    QString m_dataPath;
    ClearHttpCacheState m_clearHttpCacheState = Completed;
    base::WeakPtrFactory<ProfileIODataQt> m_weakPtrFactory; // this should be always the last member

    friend class BrowsingDataRemoverObserverQt;
};
} // namespace QtWebEngineCore

#endif // PROFILE_IO_DATA_QT_H
