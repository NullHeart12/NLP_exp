#include "generation_and_tokenization.h"

generation_and_tokenization::generation_and_tokenization(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    backend = new Backend();
    frag_num = 1;

    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(set_frag_num()));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(tokenize()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(show_tokens()));
    connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(generate()));
    connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(tokenize_input()));
}

generation_and_tokenization::~generation_and_tokenization()
{}

void generation_and_tokenization::set_frag_num()
{
	int num = ui.spinBox->value();
    frag_num = num;
	if (backend->set_frag_num(num))
		QMessageBox::information(this, "inf", "The number of fragments were set successfully.");
    else
    {
        ui.spinBox->setValue(1);
        QMessageBox::warning(this, "warning", "The number of fragments were set unsuccessfully.Maybe input num less or equal than 0.Default num is 1.");
    }
}

void generation_and_tokenization::tokenize()
{
    if (backend->tokenize())
        QMessageBox::information(this, "inf", "The dictionary were loaded successfully.");
    else
        QMessageBox::warning(this, "warning", "The dictionary were loaded unsuccessfully.");
}

void generation_and_tokenization::show_tokens()
{
    ui.textEdit->clear();
    ui.textEdit_2->clear();
    if (frag_num >= 3)
    {
        QMessageBox::warning(this, "warning", "The number of fragments is too large to show the tokens.But they can also work in the backend.");
        return;
    }

    std::string ci_path = "../exp_data/ci_count.txt";
    std::string news_path = "../exp_data/news_count.txt";

    std::string ci_tokens = backend->show_tokens(ci_path);
    std::string news_tokens = backend->show_tokens(news_path);

    if (ci_tokens == "" || news_tokens == "")
        QMessageBox::warning(this, "warning", "The dictionary were loaded unsuccessfully.");

    QString Qci_tokens = QString::fromLocal8Bit(ci_tokens.data()).toUtf8().data();
    QString Qnews_tokens = QString::fromLocal8Bit(news_tokens.data()).toUtf8().data();

    ui.textEdit->setText(Qci_tokens);
    ui.textEdit_2->setText(Qnews_tokens);
}

void generation_and_tokenization::generate()
{
    QString prefix = ui.lineEdit->text();
    std::string prefix_str = prefix.toLocal8Bit().data();
    std::string gen_str = backend->generate(prefix_str);

    QString generation = QString::fromLocal8Bit(gen_str.data()).toUtf8().data();
    if (generation == "")
        QMessageBox::warning(this, "warning", "The generation were failed.");
    ui.textEdit_3->clear();
    ui.textEdit_3->setText(generation);
}

void generation_and_tokenization::tokenize_input()
{
    QInputDialog inputDialog;
    inputDialog.setOption(QInputDialog::NoButtons);
    bool ok;
    int choice = inputDialog.getInt(this, tr("Input"), tr("Please input the type of the tokenization:\n1.FMM\n2.BMM"), 1, 1, 2, 1, &ok);
    if (choice == 1)
        QMessageBox::information(this, "inf", "You choose FMM.");
    else if (choice == 2)
        QMessageBox::information(this, "inf", "You choose BMM.");

    QString sentence = ui.lineEdit_3->text();
    std::string sentence_str = sentence.toLocal8Bit().data();
    if (choice == 1)
        sentence_str = backend->FMM(sentence_str);
    else if (choice == 2)
        sentence_str = backend->BMM(sentence_str);
    else
    {
		QMessageBox::warning(this, "warning", "The tokenization were failed.");
		return;
	}

    QString tokenization = QString::fromLocal8Bit(sentence_str.data()).toUtf8().data();
    if (tokenization == "")
    {
        QMessageBox::warning(this, "warning", "The tokenization were failed.");
        return;
    }
    ui.textEdit_7->clear();
    ui.textEdit_7->setText(tokenization);
}