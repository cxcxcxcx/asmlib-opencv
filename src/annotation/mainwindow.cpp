#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "cv.h"
#include "highgui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->allowAddPoints = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::updateImg()
{
    QString ptsName = fileList[curFile];
    QString imgName = ptsName.left(ptsName.lastIndexOf('.'));
//    cv::Mat m = cv::imread(imgName.toStdString());
//    qDebug(imgName.toStdString().c_str());
//    imshow("m", m);
//    cvWaitKey(0);
//    imwrite("tmp.jpg", m);
    ui->lblFilename->setText(imgName);
    QImage p(imgName);
    //qDebug("c: %d", p.isGrayscale());
    ui->graphicsView->setImage(QPixmap::fromImage(p));
    ui->viewEdit->setImage(imgName);
    //ui->graphicsView->setImage(imgName);
    ui->chkUse->setChecked(blUse[curFile]);
    ui->chkColor->setChecked(this->blColored[curFile]);

    FILE *fp=fopen(ptsName.toLocal8Bit(), "r");
    QList<QPoint> ql;
    if (fp){
        int n;
        int a,b;
        fscanf(fp, "%d", &n);
        for (int i=0;i<n;i++){
            fscanf(fp, "%d %d", &a,&b);
            ql.push_back(QPoint(a, b));
        }
        fclose(fp);
    }
    ui->graphicsView->setPointList(QList<QPoint>(), ql);
    ui->viewEdit->setPointList(ql);

    QString s;
    ui->editCur->setText(s.setNum(curFile));
    ui->lblTotal->setText(s.setNum(fileList.size()));
}

void MainWindow::on_actOpenFile_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
               tr("Open List file"), "./", tr("List Files (*.txt);;All Files (*.*)"));
    FILE *fp = fopen(fileName.toStdString().data(), "r");
    FILE *fp2 = fopen((fileName.left(fileName.lastIndexOf('.'))+".use").toStdString().data(), "r");
    FILE *flist = fopen((fileName.left(fileName.lastIndexOf('.'))+".list").toStdString().data(), "w");
    char s[200];
    while (!feof(fp)){
        fscanf(fp, "%s\n", s);
        this->fileList.push_back(fileName.left(fileName.lastIndexOf('/')+1)+s);
            if (fp2){
                char c[8];
                fscanf(fp2, "%c", c);
                this->blUse.push_back(c[0]=='y');
                this->blColored.push_back(false);

                if (c[0]=='y' && fileList.size()<241)
                    fprintf(flist, "%s\n", fileList.back().toStdString().data());
            }
            else {
                this->blUse.push_back(true);
                this->blColored.push_back(false);
            }
    }
    fclose(flist);
    if (fp2)
        fclose(fp2);
    fclose(fp);
    this->curFile = 0;
    ui->btnPrev->setEnabled(false);
    if (curFile==fileList.size()-1) ui->btnNext->setEnabled(false);
    updateImg();
}

void MainWindow::on_btnPrev_clicked()
{
    curFile--;
    if (curFile<0) curFile=0;
    if (curFile==0) ui->btnPrev->setEnabled(false);
    if (curFile<fileList.size()-1) ui->btnNext->setEnabled(true);
    updateImg();
}

void MainWindow::on_btnNext_clicked()
{
    curFile++;
    if (curFile>=fileList.size()) curFile=fileList.size()-1;
    if (curFile>0) ui->btnPrev->setEnabled(true);
    if (curFile==fileList.size()-1) ui->btnNext->setEnabled(false);
    updateImg();
}

void MainWindow::on_chkColor_clicked(bool checked)
{
    this->blColored[curFile] = checked;
}

void MainWindow::on_chkUse_clicked(bool checked)
{
    blUse[curFile] = checked;
}

void MainWindow::on_actSaveFile_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
               tr("Save List file"), "./", tr("use Files (*.use);;All Files (*.*)"));
    FILE *fp = fopen(fileName.toStdString().data(), "w");
    if (fp){
        for (int i=0; i<fileList.size(); i++){
            fprintf(fp, "%c", blUse[i]?'y':'n');
        }
    }
    fclose(fp);
}

void MainWindow::on_btnSaveNext_clicked()
{
    QString ptsName = fileList[curFile];

    FILE *fp=fopen(ptsName.toLocal8Bit(), "w");
    const QList<QPoint> &ql = ui->graphicsView->dstPointPaint.getPointList();
    int n = ql.size();
    fprintf(fp, "%d\n", n);
    for (int i=0;i<n;i++){
        fprintf(fp, "%d %d\n", ql[i].x(),ql[i].y());
    }
    fclose(fp);
    this->on_btnNext_clicked();
}

void MainWindow::on_editCur_editingFinished()
{
    int a = ui->editCur->text().toInt();
    if (a>=0 && a<fileList.size())
        curFile = a;
    this->updateImg();
}

void MainWindow::on_btnCmpSaveNext_clicked()
{
    QString ptsName = fileList[curFile];

    FILE *fp=fopen(ptsName.toLocal8Bit(), "w");
    const QList<QPoint> &ql = ui->viewEdit->getPointList();
    int n = ql.size();
    fprintf(fp, "%d\n", n);
    for (int i=0;i<n;i++){
        fprintf(fp, "%d %d\n", ql[i].x(),ql[i].y());
    }
    fclose(fp);
    this->on_btnNext_clicked();
}
