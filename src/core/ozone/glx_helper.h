// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef GLX_HELPER_H
#define GLX_HELPER_H

#include <QtCore/qscopedpointer.h>

#include <GL/glx.h>

#undef glXBindTexImageEXT
#undef glXReleaseTexImageEXT

QT_BEGIN_NAMESPACE

class GLXHelper
{
public:
    struct GLXFunctions
    {
        GLXFunctions();

        PFNGLXBINDTEXIMAGEEXTPROC glXBindTexImageEXT;
        PFNGLXRELEASETEXIMAGEEXTPROC glXReleaseTexImageEXT;
    };

    static GLXHelper *instance();

    Display *getXDisplay() const { return m_display; }
    GLXFunctions *functions() const { return m_functions.get(); }

    GLXFBConfig getFBConfig() const { return m_configs[0]; }
    GLXPixmap importBufferAsPixmap(int dmaBufFd, uint32_t size, uint16_t width, uint16_t height,
                                   uint16_t stride) const;

private:
    GLXHelper();

    QScopedPointer<GLXFunctions> m_functions;
    Display *m_display = nullptr;
    xcb_connection_t *m_connection = nullptr;
    GLXFBConfig *m_configs = nullptr;
};

QT_END_NAMESPACE

#endif // GLX_HELPER_H
