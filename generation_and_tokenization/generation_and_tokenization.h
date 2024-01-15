#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCore5Compat/QTextCodec>
#include <QInputDialog>
#include "ui_generation_and_tokenization.h"
#include "Backend.h"

class generation_and_tokenization : public QMainWindow
{
    Q_OBJECT
public slots:
    void set_frag_num();
    void tokenize();
    void show_tokens();
    void generate();
    void tokenize_input();

public:
    generation_and_tokenization(QWidget *parent = nullptr);
    ~generation_and_tokenization();

private:
    Ui::generation_and_tokenizationClass ui;
    Backend* backend;
    int frag_num;
};
