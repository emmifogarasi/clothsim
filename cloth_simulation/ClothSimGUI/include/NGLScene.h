#ifndef NGLSCENE_H_
#define NGLSCENE_H_
//#include <QtGui/QOpenGLContext>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <QBasicTimer>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <ngl/Transformation.h>
#include "Constraint.h"
#include "Cloth.h"
#include "particle.h"
#include "WindowParams.h"

  #define BUFFER_OFFSET(i) ((char *)NULL + (i))

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class allows the use NGL to draw OpenGL
/// Modified from:
/// Jonathan Macey - 10/9/13
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWidget

{
  Q_OBJECT
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for our NGL drawing class
  /// @param [in] parent the parent window to the class
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(QWidget *_parent);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor must close down ngl and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~NGLScene() override = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the initialize class is called once when the window is created and we have a valid GL context
  /// use this to setup any default GL stuff
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(int _width, int _height) override;
  void keyPressEvent(QKeyEvent *_event) override;
  void draw();
  bool m_transformLight=false;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the windows params such as mouse and rotations etc
  //----------------------------------------------------------------------------------------------------------------------
  WinParams m_win;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the global mouse transforms
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model position for mouse movement
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the view matrix for camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_view;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the projection matrix for camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_projection;

  Cloth cloth1; // one Cloth object of the Cloth class
  ngl::Vec3 ball_pos;
  float ball_radius = 0.5;
  const int cloth_n = 20;
  const int cloth_m = 20;
  QBasicTimer timer;

  std::vector<int> indices;
  std::vector<Cloth::Vertex> vertexData;

  float gravity = 0.2;
  float windX = 0.5;
  float windZ = 0.5;

  void getIndices();

  bool isPatterned = false;
  bool isPointy = false;
  bool isFilled = true;

  bool isBlack = false;
  bool isBlue = true;
  bool isOrange = false;

  bool isBall = true;

  bool isGrid = true;



protected:
      void timerEvent(QTimerEvent *) override;

public slots :
      void setPatternTrue();
      void setPatternFalse();
      void setFilled();
      void setPoints();
      void setGravitySlider(int g);
      void setWindXSlider(int wx);
      void setWindZSlider(int wz);
      void setIterationSlider(int it);
      void resetCloth();
      void backgroundColourBlue();
      void backgroundColourBlack();
      void backgroundColourOrange();
      void setBallCollision();
      void setNoBallCollision();
      void setGrid();
      void setNoGrid();


private:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent (QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent ( QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent ( QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent( QWheelEvent *_event) override;



};


#endif
