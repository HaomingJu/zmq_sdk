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
  this->m_buffer.reserve(this->m_width * this->m_height * 2);

  QSize show_sz(this->m_width, this->m_height);
  this->setFixedSize(show_sz);
  this->m_label.setFixedSize(show_sz);
  this->layout()->addWidget(&m_label);
  int ret = this->m_zmq_client.Init(tcp_adder.c_str());
  m_jpeg_buffer = new uint8_t[this->m_width * this->m_height * 3];
  return ret;
}


int MainWindow::Register() {}

int MainWindow::Run() {
  while (true) {
    // 取得数据
    /*
    int receive_data_sz = this->m_zmq_client.RecvData();
    if(this->m_buffer.size() < receive_data_sz)
    {
        this->m_buffer.resize(receive_data_sz);
    }
    memset(this->m_buffer.data(), 0x00, m_buffer.size());
    this->m_zmq_client.CopyRecvData(this->m_buffer.data(), receive_data_sz);
    //TODO(Haoming): 协议解析
    std::cout << "2222" << std::endl;

    uint32_t version;
    uint32_t head;
    uint32_t proto_len;
    uint32_t jpeg_len;

    ReadBin(this->m_buffer.data(), version, head, proto_len, nullptr, jpeg_len,
    this->m_jpeg_buffer);

    //显示
    this->UpdateImage(this->m_jpeg_buffer, jpeg_len);
    */
  }
}

int MainWindow::Fini() {}

int MainWindow::UpdateImage(uint8_t *data, int8_t sz) {
  this->m_label.setPixmap(QPixmap::fromImage(QImage::fromData(data, sz)));
}
