#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NGLScene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void keyPressEvent(QKeyEvent *_event);


public slots:
void resetCloth();




private:
  Ui::MainWindow *m_ui;
                /// @brief our openGL widget
                NGLScene *m_scene;

                 bool patterned = false;
                 bool pointy = false;

};

#endif // MAINWINDOW_H
