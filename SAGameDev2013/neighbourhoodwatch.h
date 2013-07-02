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

#ifndef NEIGHBOURHOODWATCH_H
#define NEIGHBOURHOODWATCH_H

#include <memory>

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>

class NeighbourhoodWatch : public QWindow, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  NeighbourhoodWatch(QWindow *parent = 0);
  ~NeighbourhoodWatch();

  virtual void render(QPainter *painter);
  virtual void render();

  virtual void initialize();

  void setAnimating(bool animating);

public slots:
  void renderLater();
  void renderNow();

protected:
  bool event(QEvent *event);

  void exposeEvent(QExposeEvent *event);
  void resizeEvent(QResizeEvent *event);

private:
  bool updatePending;
  bool animating;

  QOpenGLContext *context;
  std::unique_ptr<QOpenGLPaintDevice> device;
};

#endif // NEIGHBOURHOODWATCH_H
