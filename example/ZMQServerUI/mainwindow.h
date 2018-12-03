#ifndef __EXAMPLE_ZMQSERVERUI_MAINWINDOW_H__
#define __EXAMPLE_ZMQSERVERUI_MAINWINDOW_H__

#include <QtGui/QImage>
#include <QtGui/QPicture>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMainWindow>
#include <vector>
#include "HobotNetwork/HobotZmqClient.h"
#include "utils/CommunicationProtocol.h"

class MainWindow : public QMainWindow {
  // Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
  ~MainWindow();
  int Init(int width, int height, const std::string &tcp_adder);
  int Register();
  int Run();
  int Fini();

 private:  // functions
  int UpdateImage(uint8_t *data, int8_t sz);

 private:  // variable
  QLabel m_label;
  HobotZmqClient m_zmq_client;
  int m_width = 0;
  int m_height = 0;
  std::vector<int8_t> m_buffer;
  uint8_t *m_jpeg_buffer = nullptr;
};

#endif  // __EXAMPLE_ZMQSERVERUI_MAINWINDOW_H__
