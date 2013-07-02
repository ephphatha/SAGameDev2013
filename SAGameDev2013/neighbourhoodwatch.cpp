/* Copyright (c) 2013 Andrew James <ephphatha@thelettereph.com>
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.

** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
*/

#include "neighbourhoodwatch.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

NeighbourhoodWatch::NeighbourhoodWatch(QWindow *parent)
  : QWindow(parent)
  , updatePending(false)
  , animating(false)
  , context(0)
  , device()
{
  this->setSurfaceType(QWindow::OpenGLSurface);
}

NeighbourhoodWatch::~NeighbourhoodWatch()
{

}

void NeighbourhoodWatch::render()
{
  if (!this->device)
  {
    this->device.reset(new QOpenGLPaintDevice);
  }
  
  glViewport(0, 0, this->width(), this->height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  this->device->setSize(this->size());

  QPainter painter(this->device.get());
  this->render(&painter);
}

void NeighbourhoodWatch::render(QPainter *painter)
{
#ifdef _DEBUG
  // Nice green background for development, lets you see leakages easily.
  painter->setBackground(QBrush(Qt::green));
#else
  painter->setBackground(QBrush(Qt::black));
#endif

  painter->fillRect(painter->viewport(), painter->background());
}

void NeighbourhoodWatch::initialize()
{
}

void NeighbourhoodWatch::setAnimating(bool animating)
{
  this->animating = animating;

  if (this->animating)
  {
    this->renderLater();
  }
}

void NeighbourhoodWatch::renderLater()
{
  if (!this->updatePending)
  {
    this->updatePending = true;
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
}

void NeighbourhoodWatch::renderNow()
{
  if (!this->isExposed())
  {
    return;
  }

  this->updatePending = false;

  bool needsInitialisation = false;

  if (!this->context)
  {
    this->context = new QOpenGLContext(this);
    this->context->setFormat(this->requestedFormat());
    this->context->create();

    needsInitialisation = true;
  }

  this->context->makeCurrent(this);

  if (needsInitialisation)
  {
    this->initializeOpenGLFunctions();
    this->initialize();
  }

  this->render();

  this->context->swapBuffers(this);

  if (this->animating)
  {
    this->renderLater();
  }
}

bool NeighbourhoodWatch::event(QEvent *event)
{
  switch (event->type())
  {
  case QEvent::UpdateRequest:
    this->renderNow();
    return true;

  default:
    return QWindow::event(event);
  }
}

void NeighbourhoodWatch::exposeEvent(QExposeEvent *event)
{
  if (this->isExposed())
  {
    this->renderNow();
  }
}

void NeighbourhoodWatch::resizeEvent(QResizeEvent *event)
{
  if (this->isExposed())
  {
    this->renderNow();
  }
}
