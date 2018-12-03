// TODO(Haoming): UI设计

#include <stdio.h>
#include <QtGui/QImage>
#include <QtGui/QPicture>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>


int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QDialog *window = new QDialog;

  // 窗口初始化
  window->setWindowTitle("zmq receive ui");
  int h = 720, w = 1280;
  window->setFixedWidth(w);
  window->setFixedHeight(h);


  QLabel image_show_label;
  QVBoxLayout main_layout;
  window->setLayout(&main_layout);
  main_layout.addWidget(&image_show_label);


  // 打开文件
  FILE *jpeg_handle = fopen("/home/haoming/code/zmq_sdk/test/client_receive_ui/data/test.jpeg","rb");
  unsigned char *jpeg_buffer = new unsigned char[199616];
  fread(jpeg_buffer, 199616, 1, jpeg_handle);

  image_show_label.setPixmap(
      QPixmap::fromImage(QImage::fromData(jpeg_buffer, 199616)));

  window->show();
  delete[] jpeg_buffer;
  return app.exec();
}
