// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef CONTENT_MAIN_DELEGATE_QT_H
#define CONTENT_MAIN_DELEGATE_QT_H

#include "content/public/app/content_main_delegate.h"

#include "content_browser_client_qt.h"
#include "content_client_qt.h"
#include "content_utility_client_qt.h"

#if BUILDFLAG(IS_OZONE)
#include "compositor/content_gpu_client_qt.h"
#endif

namespace QtWebEngineCore {

class ContentMainDelegateQt : public content::ContentMainDelegate
{
public:

    // This is where the embedder puts all of its startup code that needs to run
    // before the sandbox is engaged.
    void PreSandboxStartup() override;

    content::ContentClient *CreateContentClient() override;
    content::ContentBrowserClient* CreateContentBrowserClient() override;
#if BUILDFLAG(IS_OZONE)
    content::ContentGpuClient* CreateContentGpuClient() override;
#endif
    content::ContentRendererClient* CreateContentRendererClient() override;
    content::ContentUtilityClient* CreateContentUtilityClient() override;
    std::optional<int> BasicStartupComplete() override;

private:
    ContentClientQt m_contentClient;
    std::unique_ptr<ContentBrowserClientQt> m_browserClient;
#if BUILDFLAG(IS_OZONE)
    std::unique_ptr<ContentGpuClientQt> m_gpuClient;
#endif
    std::unique_ptr<ContentUtilityClientQt> m_utilityClient;
};

} // namespace QtWebEngineCore

#endif // CONTENT_MAIN_DELEGATE_QT_H
