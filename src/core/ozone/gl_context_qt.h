// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef GL_GL_CONTEXT_QT_H_
#define GL_GL_CONTEXT_QT_H_

#include <QtCore/qscopedpointer.h>
#include <QtGui/qtgui-config.h>

#include "ui/gl/gl_context.h"

#if QT_CONFIG(opengl)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

namespace gl {
class GLSurface;
}

QT_BEGIN_NAMESPACE

class QOffscreenSurface;

class GLContextHelper
{
public:
    static void* getEGLConfig();
    static void* getGlXConfig();
    static void* getEGLDisplay();
    static void* getNativeDisplay();
    static QFunctionPointer getGlXGetProcAddress();
    static QFunctionPointer getEglGetProcAddress();
    static bool isCreateContextRobustnessSupported();
};

#if QT_CONFIG(opengl)
#undef eglCreateImage
#undef eglDestroyImage
#undef eglExportDMABUFImageMESA
#undef eglExportDMABUFImageQueryMESA
#undef eglGetCurrentContext
#undef eglGetCurrentDisplay
#undef eglGetCurrentSurface
#undef eglGetError
#undef eglMakeCurrent
#undef eglQueryString

class EGLHelper
{
public:
    struct EGLFunctions
    {
        EGLFunctions();

        PFNEGLCREATEIMAGEPROC eglCreateImage;
        PFNEGLDESTROYIMAGEPROC eglDestroyImage;
        PFNEGLEXPORTDMABUFIMAGEMESAPROC eglExportDMABUFImageMESA;
        PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC eglExportDMABUFImageQueryMESA;
        PFNEGLGETCURRENTCONTEXTPROC eglGetCurrentContext;
        PFNEGLGETCURRENTDISPLAYPROC eglGetCurrentDisplay;
        PFNEGLGETCURRENTSURFACEPROC eglGetCurrentSurface;
        PFNEGLGETERRORPROC eglGetError;
        PFNEGLMAKECURRENTPROC eglMakeCurrent;
        PFNEGLQUERYSTRINGPROC eglQueryString;
    };

    static EGLHelper *instance();

    EGLFunctions *functions() const { return m_functions.get(); }
    void queryDmaBuf(const int width, const int height, int *fd, int *stride, int *offset,
                     uint64_t *modifiers);
    bool isDmaBufSupported();

private:
    EGLHelper();

    QScopedPointer<EGLFunctions> m_functions;
    QScopedPointer<QOffscreenSurface> m_offscreenSurface;
    bool m_isDmaBufSupported = false;
};
#endif // QT_CONFIG(opengl)

QT_END_NAMESPACE

#endif

