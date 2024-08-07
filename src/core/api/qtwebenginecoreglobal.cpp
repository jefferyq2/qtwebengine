// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qtwebenginecoreglobal_p.h"

#include "web_engine_context.h"
#include "web_engine_library_info.h"

#include "base/base_paths.h"
#include "base/i18n/icu_util.h"
#include "base/path_service.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"

#include <QUrl>

#if QT_CONFIG(opengl) && defined(Q_OS_LINUX)
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
Q_GUI_EXPORT void qt_gl_set_global_share_context(QOpenGLContext *context);
Q_GUI_EXPORT QOpenGLContext *qt_gl_global_share_context();
QT_END_NAMESPACE
#endif

namespace QtWebEngineCore {
#if QT_CONFIG(opengl) && defined(Q_OS_LINUX)
static QOpenGLContext *shareContext;

static void deleteShareContext()
{
    if (qt_gl_global_share_context() == shareContext)
        qt_gl_set_global_share_context(nullptr);
    delete shareContext;
    shareContext = 0;
}

static void ensureShareContext()
{
    // No need to override the shared context if QApplication already set one (e.g with Qt::AA_ShareOpenGLContexts).
    if (qt_gl_global_share_context())
        return;

    QCoreApplication *app = QCoreApplication::instance();
    if (!app) {
        qFatal("QtWebEngineQuick::initialize() but no core application instance.");
        return;
    }

    // Bail out silently if the user did not construct a QGuiApplication.
    if (!qobject_cast<QGuiApplication *>(app))
        return;

    if (app->thread() != QThread::currentThread()) {
        qFatal("QtWebEngineQuick::initialize() must be called from the Qt gui thread.");
        return;
    }

    if (shareContext)
        return;

    shareContext = new QOpenGLContext;
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();

    shareContext->setFormat(format);
    shareContext->create();
    qAddPostRoutine(deleteShareContext);
    qt_gl_set_global_share_context(shareContext);

    // Classes like QOpenGLWidget check for the attribute
    app->setAttribute(Qt::AA_ShareOpenGLContexts);
}
#endif // QT_CONFIG(opengl) && defined(Q_OS_LINUX)

Q_WEBENGINECORE_EXPORT void initialize()
{
#if QT_CONFIG(opengl) && defined(Q_OS_LINUX)
    auto api = QQuickWindow::graphicsApi();
    if (api != QSGRendererInterface::OpenGL && api != QSGRendererInterface::Vulkan)
        QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    ensureShareContext();
#endif
}

bool closingDown()
{
    return WebEngineContext::closingDown();
}

} // namespace QtWebEngineCore

#if defined(Q_OS_WIN)
namespace QtWebEngineSandbox {
sandbox::SandboxInterfaceInfo *staticSandboxInterfaceInfo(sandbox::SandboxInterfaceInfo *info)
{
    static sandbox::SandboxInterfaceInfo *g_info = nullptr;
    if (info) {
        Q_ASSERT(g_info == nullptr);
        g_info = info;
    }
    return g_info;
}
} //namespace
#endif

static void initialize()
{
#if QT_CONFIG(opengl) && defined(Q_OS_LINUX)
    // QCoreApplication is not yet instantiated, ensuring the call will be deferred
    qAddPreRoutine(QtWebEngineCore::initialize);
#endif
}

QT_BEGIN_NAMESPACE

QString qWebEngineGetDomainAndRegistry(const QUrl &url) {
    base::FilePath icuDataPath;
    // Let's assume that ICU is already initialized if DIR_QT_LIBRARY_DATA is set.
    if (!base::PathService::Get(base::DIR_QT_LIBRARY_DATA, &icuDataPath)) {
        icuDataPath = WebEngineLibraryInfo::getPath(base::DIR_QT_LIBRARY_DATA);
        if (!base::PathService::OverrideAndCreateIfNeeded(base::DIR_QT_LIBRARY_DATA, icuDataPath, false, false))
            qWarning("Failed to set ICU data path.");
        base::i18n::InitializeICU();
    }

    const QString host = url.host();
    const std::string domain = net::registry_controlled_domains::GetDomainAndRegistry(host.toStdString(), net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
    return QString::fromStdString(domain);
}

QT_END_NAMESPACE

Q_CONSTRUCTOR_FUNCTION(initialize)
