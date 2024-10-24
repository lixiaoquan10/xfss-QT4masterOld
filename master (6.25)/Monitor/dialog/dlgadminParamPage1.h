#ifndef DLGADMINPARAMPAGE_H
#define DLGADMINPARAMPAGE_H

#include "cglobal.h"

namespace Ui {
class dlgadminParamPage;
}

class dlgadminParamPage : public QDialog
{
    Q_OBJECT

public:
    explicit dlgadminParamPage(QWidget *parent = 0);
    ~dlgadminParamPage();

private:
    Ui::dlgadminParamPage *ui;
};


#endif // DLGADMINPARAMPAGE_H
