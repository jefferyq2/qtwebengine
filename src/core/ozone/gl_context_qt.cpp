// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "gl_context_qt.h"

#include "ui/gl/egl_util.h"
#include "ui/gl/gl_implementation.h"

#include <QGuiApplication>
#include <QOpenGLContext>
#include <QtCore/qglobal.h>
#include <QtGui/private/qtgui-config_p.h>
#include <qpa/qplatformnativeinterface.h>

#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface.h"

QT_BEGIN_NAMESPACE

Q_GUI_EXPORT QOpenGLContext *qt_gl_global_share_context();

namespace {

inline void *resourceForContext(const QByteArray &resource)
{
#if QT_CONFIG(opengl)
    QOpenGLContext *shareContext = qt_gl_global_share_context();
    if (!shareContext) {
        qFatal("QWebEngine: OpenGL resource sharing is not set up in QtQuick. Please make sure to "
               "call QtWebEngineQuick::initialize() in your main() function.");
    }
    return qApp->platformNativeInterface()->nativeResourceForContext(resource, shareContext);
#else
    return nullptr;
#endif
}

inline void *resourceForIntegration(const QByteArray &resource)
{
    return qApp->platformNativeInterface()->nativeResourceForIntegration(resource);
}

} // namespace

void* GLContextHelper::getEGLConfig()
{
    QByteArray resource = QByteArrayLiteral("eglconfig");
    return resourceForContext(resource);
}

void* GLContextHelper::getGlXConfig()
{
    QByteArray resource = QByteArrayLiteral("glxconfig");
    return resourceForContext(resource);
}

void* GLContextHelper::getNativeDisplay()
{
    return resourceForIntegration(QByteArrayLiteral("nativedisplay"));
}

QFunctionPointer GLContextHelper::getGlXGetProcAddress()
{
     QFunctionPointer get_proc_address = nullptr;
#if QT_CONFIG(xcb_glx)
    if (QOpenGLContext *context = qt_gl_global_share_context()) {
        get_proc_address = context->getProcAddress("glXGetProcAddress");
    }
#endif
    return get_proc_address;
}

QFunctionPointer GLContextHelper::getEglGetProcAddress()
{
     QFunctionPointer get_proc_address = nullptr;
#if QT_CONFIG(opengl)
    if (QOpenGLContext *context = qt_gl_global_share_context()) {
        get_proc_address = context->getProcAddress("eglGetProcAddress");
    }
#endif
    return get_proc_address;
}

bool GLContextHelper::isCreateContextRobustnessSupported()
{
    static std::optional<bool> robustness;

#if QT_CONFIG(opengl)
    if (robustness.has_value())
        return robustness.value();

    if (QGuiApplication::platformName() == QLatin1String("offscreen"))
        robustness = false;
    else if (QOpenGLContext *context = qt_gl_global_share_context())
        robustness = context->format().testOption(QSurfaceFormat::ResetNotification);
#endif

    return robustness.value_or(false);
}

QT_END_NAMESPACE
