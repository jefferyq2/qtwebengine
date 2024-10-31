// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "ozone_util_qt.h"

#include <QtGui/qguiapplication.h>
#include <qpa/qplatformnativeinterface.h>

#if QT_CONFIG(opengl)
#include <QtGui/qopenglcontext.h>
#endif

QT_BEGIN_NAMESPACE

namespace OzoneUtilQt {
void *getXDisplay()
{
#if QT_CONFIG(xcb)
    auto *x11Application = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
    if (x11Application)
        return x11Application->display();
#endif

    return nullptr;
}

QOpenGLContext *getQOpenGLContext()
{
#if QT_CONFIG(opengl)
    // We always expect global share context here because it is a requirement for direct GLX and
    // EGL rendering, see QtWebEngineCore::ensureShareContext().
    // TODO: Make sure a temporary context is created here as a fallback when we remove the global
    //       share context dependency of QtWebEngine.
    Q_ASSERT(QOpenGLContext::globalShareContext());
    if (auto *shareContext = QOpenGLContext::globalShareContext())
        return shareContext;
#endif

    return nullptr;
}

bool usingGLX()
{
    static std::optional<bool> result;

#if QT_CONFIG(opengl) && QT_CONFIG(xcb_glx_plugin)
    if (result.has_value())
        return result.value();

    QOpenGLContext *context = getQOpenGLContext();
    result = (context->nativeInterface<QNativeInterface::QGLXContext>() != nullptr);
#endif

    return result.value_or(false);
}

bool usingEGL()
{
    static std::optional<bool> result;

#if QT_CONFIG(opengl) && QT_CONFIG(egl)
    if (result.has_value())
        return result.value();

    QOpenGLContext *context = getQOpenGLContext();
    result = (context->nativeInterface<QNativeInterface::QEGLContext>() != nullptr);
#endif

    return result.value_or(false);
}
} // namespace OzoneUtilQt

QT_END_NAMESPACE
