#include <iostream>
#include "mainwindow.h"
#define IMAGE_W 1280
#define IMAGE_H 720

int main(int argc, char **argv) {
  std::string tcp_adder;
  switch (argc) {
    case 1:
      tcp_adder = "tcp://localhost:5555";
      break;
    case 3:
      tcp_adder = "tcp://" + std::string(argv[1]) + ":" + std::string(argv[2]);
      break;
    default:
      break;
  }
  QApplication app(argc, argv);
  MainWindow mainWin;
  mainWin.Init(IMAGE_W, IMAGE_H, tcp_adder);
  mainWin.show();
  return app.exec();
}
