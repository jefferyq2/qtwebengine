// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef GL_GL_CONTEXT_QT_H_
#define GL_GL_CONTEXT_QT_H_

#include <QFunctionPointer>

#include "ui/gl/gl_context.h"

QT_BEGIN_NAMESPACE

class GLContextHelper
{
public:
    static void* getEGLConfig();
    static void* getGlXConfig();
    static void* getNativeDisplay();
    static QFunctionPointer getGlXGetProcAddress();
    static QFunctionPointer getEglGetProcAddress();
    static bool isCreateContextRobustnessSupported();
};

QT_END_NAMESPACE

#endif // GL_GL_CONTEXT_QT_H_
