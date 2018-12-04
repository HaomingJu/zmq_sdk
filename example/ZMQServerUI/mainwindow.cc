#include "mainwindow.h"
#include <stdio.h>
#include <iostream>
#include "QtCore/QSize"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {}

MainWindow::~MainWindow() {}

int MainWindow::Init(int width, int height, const std::string &tcp_adder) {
  this->m_width = width;
  this->m_height = height;

  QSize show_sz(this->m_width, this->m_height);
  this->setFixedSize(show_sz);
  this->m_label.setFixedSize(show_sz);
  this->layout()->addWidget(&m_label);
  int ret = this->m_zmq_client.Init(tcp_adder.c_str());
  m_jpeg_buffer = new uint8_t[this->m_width * this->m_height * 3];

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &MainWindow::UpdateImage);
  m_timer->start(10);
  return ret;
}


void MainWindow::Fini() {
  delete[] m_jpeg_buffer;
  delete m_timer;
}

void MainWindow::UpdateImage() {
  std::cout << "----------------" << std::endl;
  int receive_data_sz = this->m_zmq_client.RecvData();
  std::vector<int8_t> tmp_buffer;
  tmp_buffer.reserve(receive_data_sz);

  if (receive_data_sz > 0) {
    memset(tmp_buffer.data(), 0x00, receive_data_sz);
    this->m_zmq_client.CopyRecvData(tmp_buffer.data(), receive_data_sz);
    uint32_t version;
    uint32_t head;
    uint32_t proto_len;
    uint32_t jpeg_len;

    ReadBin(tmp_buffer.data(), version, head, proto_len, nullptr, jpeg_len,
            this->m_jpeg_buffer);

    //显示
    if (jpeg_len != 0) {
      this->m_label.setPixmap(
          QPixmap::fromImage(QImage::fromData(this->m_jpeg_buffer, jpeg_len)));
    }
  }
}
