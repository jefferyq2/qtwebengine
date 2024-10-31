// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "gl_helper.h"
#include "ozone_util_qt.h"

#include <QtGui/qopenglcontext.h>

QT_BEGIN_NAMESPACE

GLHelper::GLExtFunctions::GLExtFunctions()
{
    QOpenGLContext *context = OzoneUtilQt::getQOpenGLContext();

    glCreateMemoryObjectsEXT = reinterpret_cast<PFNGLCREATEMEMORYOBJECTSEXTPROC>(
            context->getProcAddress("glCreateMemoryObjectsEXT"));
    glDeleteMemoryObjectsEXT = reinterpret_cast<PFNGLDELETEMEMORYOBJECTSEXTPROC>(
            context->getProcAddress("glDeleteMemoryObjectsEXT"));
    glEGLImageTargetTexture2DOES = reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(
            context->getProcAddress("glEGLImageTargetTexture2DOES"));
    glImportMemoryFdEXT = reinterpret_cast<PFNGLIMPORTMEMORYFDEXTPROC>(
            context->getProcAddress("glImportMemoryFdEXT"));
    glTextureStorageMem2DEXT = reinterpret_cast<PFNGLTEXTURESTORAGEMEM2DEXTPROC>(
            context->getProcAddress("glTextureStorageMem2DEXT"));
}

GLHelper *GLHelper::instance()
{
    static GLHelper glHelper;
    return &glHelper;
}

GLHelper::GLHelper() : m_functions(new GLHelper::GLExtFunctions()) { }

QT_END_NAMESPACE
