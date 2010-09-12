#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QList< QString > fileList;
    QList< bool > blUse;
    QList< bool > blColored;
    int curFile;

    void updateImg();

private slots:
    void on_btnCmpSaveNext_clicked();
    void on_editCur_editingFinished();
    void on_btnSaveNext_clicked();
    void on_actSaveFile_triggered();
    void on_chkUse_clicked(bool checked);
    void on_chkColor_clicked(bool checked);
    void on_btnNext_clicked();
    void on_btnPrev_clicked();
    void on_actOpenFile_triggered();
};

#endif // MAINWINDOW_H
